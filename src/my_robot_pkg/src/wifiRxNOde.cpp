#include <rclcpp/rclcpp.hpp> 
#include <std_msgs/msg/string.hpp>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h> 
#include <unistd.h>
using namespace std;
class wifiRxNOde :public rclcpp::Node{
  
     private:
     int sock_;
     struct sockaddr_in rx_addr_;
     int rx_port_=8081;

     rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher;
     rclcpp::TimerBase::SharedPtr timer_;
     void receive_callback(){
           char buffer[1024];
           struct sockaddr_in sender_addr;
           socklen_t sender_len=sizeof(sender_addr);
          int bytes_received = recvfrom(sock_, buffer, sizeof(buffer) - 1, 0, 
                                      (struct sockaddr *)&sender_addr, &sender_len);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            
            auto msg = std_msgs::msg::String(); 
            msg.data = std::string(buffer); 
            
            publisher->publish(msg); 
            RCLCPP_INFO(this->get_logger(), "Received from ESP32: %s", msg.data.c_str());
        }



     }
     public:
     wifiRxNOde():Node("wifiRxNOde"){

      publisher=this->create_publisher<std_msgs::msg::String>("sensor_data",10);
      sock_ = socket(AF_INET, SOCK_DGRAM, 0);
        rx_addr_.sin_family = AF_INET;
        rx_addr_.sin_addr.s_addr = INADDR_ANY;
        rx_addr_.sin_port = htons(rx_port_);
        bind(sock_, (const struct sockaddr *)&rx_addr_, sizeof(rx_addr_));
        int flags = fcntl(sock_, F_GETFL, 0);
        fcntl(sock_, F_SETFL, flags | O_NONBLOCK);
        timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&wifiRxNOde::receive_callback, this));
        RCLCPP_INFO(this->get_logger(), "WiFi RX Node Started. Listening on port: %d", rx_port_);



     }
      ~wifiRxNOde(){
        close(sock_);
      }




};
int main(int argc,char**argv){

  rclcpp::init(argc,argv);
  rclcpp::spin(make_shared<wifiRxNOde>());
  rclcpp::shutdown();





    return 0;
}