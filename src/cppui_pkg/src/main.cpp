#include <QApplication>
#include <QVBoxLayout>
#include <QWidget>

#include <rclcpp/rclcpp.hpp>
#include <rviz_common/render_panel.hpp>
#include <rviz_common/visualization_manager.hpp>
#include <rviz_common/ros_integration/ros_node_abstraction.hpp>
#include <rviz_common/window_manager_interface.hpp>

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  QApplication app(argc, argv);

  QWidget window;
  window.setWindowTitle("Embedded RViz");
  QVBoxLayout *layout = new QVBoxLayout;
  window.setLayout(layout);

  auto render_panel = new rviz_common::RenderPanel();
  layout->addWidget(render_panel);

  // 建立 RosNodeAbstraction（Humble: 傳 string）
  auto node_abstraction = std::make_shared<rviz_common::ros_integration::RosNodeAbstraction>("embedded_rviz");

  auto manager = std::make_shared<rviz_common::VisualizationManager>(
    render_panel,
    node_abstraction,
    nullptr,
    rclcpp::Clock::make_shared()
  );

  render_panel->initialize(manager.get());
  manager->initialize();         // 初始化 Display 等元件
  manager->startUpdate();
  manager->setFixedFrame("map");

  // 建議先不加任何 display，確認能正常顯示後再加入
  // manager->createDisplay("rviz_default_plugins/Grid", "grid", true);

  window.resize(800, 600);
  window.show();

  int result = app.exec();

  rclcpp::shutdown();
  return result;
}
