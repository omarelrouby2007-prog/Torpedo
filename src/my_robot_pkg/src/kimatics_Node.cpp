#include <rclcpp/rclcpp.hpp>
#include<geometry_msgs/msg/twist.hpp>
#include<std_msgs/msg/float32_multi_array.hpp>
using namespace std;


class kimematic : public rclcpp::Node {

    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscription_;

    void Twist_callback(const geometry_msgs::msg::Twist::SharedPtr msg) const {
        
        float linear = msg->linear.x;
        float angular = msg->angular.z;
        
        
        float left_motor_speed = linear - angular;
        float right_motor_speed = linear + angular;

        
        auto speed_msg = std_msgs::msg::Float32MultiArray();
        
    
        speed_msg.data.push_back(left_motor_speed);
        speed_msg.data.push_back(right_motor_speed);

        
        publisher_->publish(speed_msg);
    }

public:
    kimematic() : Node("kinematics_node2") {
        publisher_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("motor_speeds", 10);
        
        subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "cmd_vel", 10, std::bind(&kimematic::Twist_callback, this, std::placeholders::_1));
    }
};






int main(int argc,char**argv){

        rclcpp::init(argc,argv);
        rclcpp::spin(make_shared<kimematic>());
        rclcpp::shutdown();






    return 0;
}
