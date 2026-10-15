#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32_multi_array.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
using namespace std;
class wifiTxNode:public rclcpp::Node{

    private:

    int sock;
    struct sockaddr_in esp_adder;
     string esp_ip="192.168.4.1";
     int esp_port=8080;
    rclcpp::Subscription<std_msgs::msg::Float32MultiArray()>::SharedPtr Subscription;

    void motor_speed_callback(const std_msgs::msg::Float32MultiArray::SharedPtr msg) const{


        if(msg->data.size()>=2){

          float left_motor_speed=msg->data[0];
          float right_motor_speed=msg->data[1];
          string payload="L"+std::to_string(left_motor_speed)+"R"+std::to_string(right_motor_speed)+"#n";
          sendto(sock,payload.c_str(),payload.length(),0,(struct sockaddr*)&esp_adder,sizeof(esp_adder));
          RCLCPP_INFO(this->get_logger(),"sent wifi:%s",payload.c_str());


        }


      




    }
    public:
    wifiTxNode():Node("wifiTxNode"){

         sock=socket(AF_INET,SOCK_DGRAM,0);
         esp_adder.sin_family=AF_INET;
         esp_adder.sin_port=htons(esp_port);
         inet_pton(AF_INET,esp_ip.c_str(),&esp_adder.sin_addr);
         Subscription=this->create_subscription<std_msgs::msg::Float32MultiArray()>(
            "motor_speeds", 10,bind(&wifiTxNode::motor_speed_callback, this, placeholders::_1));

            RCLCPP_INFO(this->get_logger(),"ip=%s",esp_ip.c_str());



    }
    ~wifiTxNode(){

        close(sock);
    }
  

};

int main(int argc,char**argv){


         rclcpp::init(argc,argv);
         rclcpp::spin(make_shared<wifiTxNode>());
         rclcpp::shutdown();







    return 0;
}