#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <json/json.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/file.h>
#include <csignal>

namespace fs = std::filesystem;

const std::string output_path = "/home/jason9308/robot_ws/command_jason/output.json";
const std::string origin_path = "/home/jason9308/robot_ws/command_jason/origin.json";

// 比較兩個 JSON 檔案是否一致
bool compare_json_files(const std::string& path1, const std::string& path2) {
    std::ifstream f1(path1);
    std::ifstream f2(path2);
    if (!f1 || !f2) return false;

    Json::Value json1, json2;
    f1 >> json1;
    f2 >> json2;

    return json1 == json2;
}

// 複製 output.json -> origin.json
bool copy_file(const std::string& from, const std::string& to) {
    try {
        fs::copy_file(from, to, fs::copy_options::overwrite_existing);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "複製檔案失敗: " << e.what() << std::endl;
        return false;
    }
}

// 嘗試以 flock 鎖住檔案
bool try_lock_file(const std::string& path, int& fd) {
    fd = open(path.c_str(), O_RDONLY);
    if (fd == -1) return false;
    if (flock(fd, LOCK_SH | LOCK_NB) == -1) {
        close(fd);
        return false;  // 被其他程式鎖住了
    }
    return true;
}

// 關閉指定的 Python 節點（例如 speech_recognition_node.py）
void kill_python_node(const std::string& keyword) {
    std::string cmd = "pkill -f " + keyword;
    std::system(cmd.c_str());
}

int main() {
    std::cout << "[JSON Watcher] 開始監控 JSON 檔案...\n";

    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // 每秒檢查一次

        int fd;
        if (!try_lock_file(output_path, fd)) {
            std::cout << "[JSON Watcher] output.json 正在被寫入，稍後再試...\n";
            continue;
        }

        bool is_same = compare_json_files(output_path, origin_path);
        flock(fd, LOCK_UN);
        close(fd);

        if (!is_same) {
            std::cout << "[JSON Watcher] 偵測到 JSON 不一致，複製並結束節點...\n";

            if (copy_file(output_path, origin_path)) {
                kill_python_node("speech_recognition_node.py");  // 關掉語音節點
                kill_python_node("json_file_watcher");            // 關掉自己
                break;
            }
        } else {
            std::cout << "[JSON Watcher] JSON 檔案一致，繼續監控...\n";
        }
    }

    return 0;
}
