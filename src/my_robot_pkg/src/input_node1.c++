#include <rclcpp/rclcpp.hpp>
#include<geometry_msgs/msg/twist.hpp>
using namespace std;
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
class inputNode:public rclcpp::Node{
           private:
           rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher ;
           rclcpp::TimerBase::SharedPtr timer;
           struct termios oldt,newt;
           void Timer_callback(){

                 char c;
                 if(read(STDIN_FILENO,&c,1)>0){

                auto msg = geometry_msgs::msg::Twist();

            if (c == 'w') { msg.linear.x = 1.0; }      
            else if (c == 's') { msg.linear.x = -1.0; } 
            else if (c == 'a') { msg.angular.z = 1.0;}
            else if (c == 'd') { msg.angular.z = -1.0; }
            else if (c == ' ') {                        
                msg.linear.x = 0.0; 
                msg.angular.z = 0.0; 
            }
            
            publisher->publish(msg);
        }
    }

                 



           
  
           public:
           inputNode():Node("input_node1"){

               
            publisher=this->create_publisher<geometry_msgs::msg::Twist>("cmd_vol",10);
            timer = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&inputNode::Timer_callback, this));

            tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
     


           }
           ~ inputNode() {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    }




};
int main(int argc,char **argv){
  
          rclcpp::init(argc,argv);
         rclcpp::spin(make_shared<inputNode>());
         rclcpp::shutdown();
         return 0;

}
