#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
using namespace std;
class wifiTxNode:public rclcpp::Node{

    private:

    int sock_;
    struct sockaddr_in esp_adder_;
     string esp_ip="192.168.4.1";
     int esp_port_=8080;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray()>::SharedPtr Subscription;

    void motor_speed_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) const{


        if(msg->data.size()>=2){

          float left_motor_speed=msg->data[0];
          float right_motor_speed=msg->data[1];
          string payload="L"+std::to_string(left_motor_speed)+"R"+std::to_string(right_motor_speed)+"#n";
          sendto(sock_,payload.c_str(),payload.length(),0,(struct sockaddr*)&esp_adder_,sizeof(esp_adder_));
          RCLCPP_INFO(this->get_logger(),"sent wifi:%s",payload.c_str());


        }


      




    }
    public:
    wifiTxNode():Node("wifiTxNode"){

         sock_=socket(AF_INET,SOCK_DGRAM,0);
         esp_adder_.sin_family=AF_INET;
         esp_adder_.sin_port=htons(esp_port_);
         inet_pton(AF_INET,esp_ip.c_str(),&esp_adder_.sin_addr);
         Subscription=this->create_subscription<std_msgs::msg::Float32MultiArray()>(
            "motor_speeds", 10,bind(&wifiTxNode::motor_speed_callback, this, placeholders::_1));

            RCLCPP_INFO(this->get_logger(),"ip=%s",esp_ip.c_str());



    }
    ~wifiTxNode(){

        close(sock_);
    }
  

};

int main(int argc,char**argv){


         rclcpp::init(argc,argv);
         rclcpp::spin(make_shared<wifiTxNode>());
         rclcpp::shutdown();







    return 0;
}