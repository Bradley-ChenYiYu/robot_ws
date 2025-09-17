# 醫院多功能送藥機器人 


## Demo影片
[影片連結](https://youtu.be/UA9hmdtO8JE?si=tcyx1zMHwtMPFO8q)

## 一、專題簡介
近年來隨著人口高齡化與護理師人力流失的加劇，醫療與長照體系面臨嚴重人力不足的挑戰。
本專題設計一台智慧長照機器人，能自主導航到患者房間，透過臉部辨識驗證身份，並利用機械手臂遞送藥物。同時支援語音控制、聊天互動與視訊通話，以協助醫護人員減輕工作負擔，提升長照服務品質。

---

## 二、應用情景
![image](https://hackmd.io/_uploads/BJj7wMDogg.png)


---

## 三、系統架構
本系統整合多模態輸入（語音與網頁）、中央控制節點、導航模組、臉部辨識模組、夾藥模組，以及聊天與視訊功能，達成藥物配送與長照服務的完整流程。
![image](https://hackmd.io/_uploads/rJMQIGDjxe.png)

---

## 四、硬體配置
* Tracer：自走車
* xArm Lite6：6自由度機械手臂，用於藥物抓取與遞送，也負責調整相機朝向
* Intel RealSense D435i：RGB-D 相機，用於人臉辨識、SLAM以及藥包位置偵測
* Velodyne LiDAR (VLP-16)：3D 環境感測，用於 SLAM 與導航
* Pill Box + ArUco 標記：標記藥物存放位置，協助手眼協作抓取
![image](https://hackmd.io/_uploads/BJHpvGvjgx.png)

---

## 五、模組介紹

1. **語音辨識模組**  
   使用 **Google Speech-to-Text** 將語音轉換成文字，並透過 **OpenAI API** 解析成固定格式的 JSON 指令。  
   讓使用者能以自然語音操作，支援送藥、聊天、視訊等功能。  

2. **網頁控制模組**  
   以 **LineBot + Web 前端 (HTML/CSS/JS)** 作為輸入界面，醫護人員可直接下達或修改指令。  
   提供任務排程、狀態查詢及導航地圖顯示。  
   ![image](https://hackmd.io/_uploads/HJxsCGwslx.png)


3. **導航模組**  
   結合 **RTAB-Map (SLAM)** 與 **Nav2**，先利用 Velodyne LiDAR 與 RealSense 深度相機建立環境地圖。
導航時會根據環境即時更新 **global costmap** 與 **local costmap**，用於路徑規劃與避障。
能在室內環境中自主導航至病房或指定地點。  

    | 圖片 |  |
    |------|----------|
    | 11 樓地圖 | ![11樓地圖](https://hackmd.io/_uploads/rkawJmPsee.png) |
    | 11 樓導航畫面  | ![image](https://hackmd.io/_uploads/B1gDFnwoex.png)<br>淺藍色區域代表**障礙物代價（costmap）**，會影響路徑規劃 <br> 深藍色線條代表planner規劃出來的導航路徑|




4. **臉部辨識模組**  
   採用 **YOLOv10** (以 WIDER FACE 資料集自行訓練）進行人臉偵測，並以 **Dlib** 做五點對齊，最後利用 **InceptionResnet V1 (FaceNet)** 進行身份驗證。  
   確保藥物正確遞送給對應病人。  
   
    | 圖片 |  |
    |------|----------|
    | yolov10訓練 | ![results](https://hackmd.io/_uploads/rJ9b7CDiel.png) |
    | 人臉辨識通過  | ![image](https://hackmd.io/_uploads/Bk7UTaPjlg.png) |


5. **夾藥模組（機械手臂控制）** 
    使用 xArm Lite6 搭配 RealSense D435 完成 [手眼校正](https://youtu.be/MsT9OtA3d_w?si=70Rc7QNobzVqoryr)，將相機座標轉換至機械手臂的基座座標系 (base frame)，確保抓取位置準確。
系統會偵測 ArUco 標記以計算藥盒座標，並根據轉換結果控制手臂抓取藥包。
同時，透過 YOLOv10（自行訓練）進行手部偵測，辨識患者的手部位置，確保機械手臂能將藥包正確放置到患者手上，完成安全交付。 
   
    ![image](https://hackmd.io/_uploads/BJjm_Rvseg.png)

6. **視訊通話模組**  
   透過 **Selenium + LineBot** 自動建立 **Google Meet** 連線。  
   方便醫護人員遠端視訊，提升照護的即時性。  

7. **聊天機器人模組**  
   整合 **OpenAI GPT、Google STT 與 OpenAI TTS**，提供自然語音對話以及聊天介面顯示。  
   陪伴患者進行簡單交談，增進互動與陪伴感。
   ![image](https://hackmd.io/_uploads/r1p2o6Diex.png)


   

---

## 六、結語
本系統透過自主導航、臉部辨識、語音互動與機械手臂協作，展現了長照服務機器人的可行性，期望能有效減輕醫護人員的負擔並提升照護品質。
未來期望朝以下方向發展：
*    整合更多醫療感測功能（如心率、血壓、跌倒偵測）
*    與醫院資訊系統（HIS）進行串接
*    強化情感互動，提供更自然的語音對話體驗
