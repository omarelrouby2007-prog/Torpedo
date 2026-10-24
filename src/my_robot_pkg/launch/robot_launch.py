from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
    
        Node(
            package='my_robot_pkg',
            executable='input_node',
            name='input_node',
            output='screen'
        ),

        Node(
            package='my_robot_pkg',
            executable='kinematics_node',
            name='kinematics_node',
            output='screen'
        ),
        
        Node(
            package='my_robot_pkg',
            executable='wifi_tx_node',
            name='wifi_tx_node',
            output='screen'
        ),
        
        Node(
            package='my_robot_pkg',
            executable='wifi_rx_node', 
            name='wifi_rx_node',
            output='screen'
        )
    ])