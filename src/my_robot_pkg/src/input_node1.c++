#include <rclcpp/rclcpp.hpp>
#include<geometry_msgs/msg/twist.hpp>
using namespace std;
class inputNode:public rclcpp::Node{
           private:
           rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher ;
  
           public:
           inputNode():Node("input_node1"){

               
            publisher=this->create_publisher<geometry_msgs::msg::Twist>("cmd_vol",10);
            RCLCPP_INFO(this->get_logger(),"input_Node_start");
     


           }




};
int main(int argc,char **argv){
  
          rclcpp::init(argc,argv);
         rclcpp::spin(make_shared<inputNode>());
         rclcpp::shutdown();
         return 0;

}
