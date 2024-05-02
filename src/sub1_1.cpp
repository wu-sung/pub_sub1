//헤더파일 포함
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <memory>
#include <functional>
using std::placeholders::_1;
//콜백함수 정의
void mysub_callback(rclcpp::Node::SharedPtr node, const 
//스마트 포인터를 사용하여 string 메세지형태의 mgs사용
std_msgs::msg::String::SharedPtr msg)
{
RCLCPP_INFO(node->get_logger(), "Received message: %s", msg->data.c_str());
}

int main(int argc, char* argv[])
{
rclcpp::init(argc, argv);
//mynode라는 ROS2노드를 만듬
auto node = std::make_shared<rclcpp::Node>("mynode");
// qos 설정
auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
//std::function 객체인 fn을 선언
std::function<void(const std_msgs::msg::String::SharedPtr msg)> fn;

// mysub_callback: 호출할 함수 또는 함수 객체, node: mysub_callback 함수의 첫 번째 매개변수로 전달될 인수,
//  _1: std::bind 함수에서 두 번째 매개변수부터 사용되는 placeholder
fn = std::bind(mysub_callback, node, _1);

// mytopic토픽을 만듬
auto mysub = node->create_subscription<std_msgs::msg::String>("mytopic",qos_profile,fn);
// ROS 2 노드가 종료될 때까지 프로그램을 실행
rclcpp::spin(node);
rclcpp::shutdown();
return 0;
}
