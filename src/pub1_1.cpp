// 헤더파일 참조
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "rclcpp/time_source.hpp"
#include <memory>
#include <chrono>
#include <functional>
using namespace std::chrono_literals;
// 콜백함수 (노드와 퍼블리셔를 인자로 받음)
void callback(rclcpp::Node::SharedPtr node, rclcpp::Publisher<std_msgs::msg::String>::SharedPtr pub)
{
// 카운트
static int count;
// auto 자료형을 써서 message의 자료형을 자동적으로 찾고 String을 사용
auto message = std_msgs::msg::String();
// message의 data값 초기화 -> hello world의 문자열과 정수형 변수 count를 string으로 바꿈
message.data = "Hello world! " + std::to_string(count++);

//ROS2의 로깅 매크로, node->get_logger()는 현재 노드의 로거를 가져오고 노드에서 로그를 기록하는데에 사용
// message.data값을 string으로하여 출력
RCLCPP_INFO(node->get_logger(), "Publish: %s", message.data.c_str());
//pub 객체를 사용하여 message에 저장된 메시지를 해당 토픽으로 발행
pub->publish(message);
}

int main(int argc, char * argv[])
{
// ROS 2 노드를 초기화
rclcpp::init(argc, argv);
// mynode라는 이름으로 새로운 ROS 2 노드를 만듬
auto node = std::make_shared<rclcpp::Node>("mynode");
// qos설정
auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
//mytopic이라는 이름의 토픽에 메시지를 발행하는 발행자를 생성. 메세지 유형은 String
auto pub = node->create_publisher<std_msgs::msg::String>("mytopic", qos_profile
);
//발행자 콜백 함수를 설정
std::function<void()> fn = std::bind(callback, node, pub);
//100밀리초마다 fn을 호출하는 타이머를 생성
auto timer = node->create_wall_timer(100ms, fn);
// ROS 2 노드가 종료될 때까지 프로그램을 실행
rclcpp::spin(node);
// ROS 2 노드와 관련된 리소스를 정리하고 종료
rclcpp::shutdown();
//0값을 반환
return 0;
}
