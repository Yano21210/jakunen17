#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
using namespace std;
#include <stdio.h>

int robot_mode=0;
int stage = 0;
int cou=0;
std_msgs::Int8 ang_dir;

int f_kyori = 0;
int b_kyori = 0;
int r_kyori = 0;
int l_kyori = 0;

int log_flag=0;

void callback_range_front(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_front\t:\t%f\n\n", msg.data);
    f_kyori =  msg.data;
}

void callback_robot_state(const std_msgs::Int8& msg) {
    if(msg.data<1)robot_mode=0;
    else if(msg.data<4)robot_mode=1;
    else robot_mode=2;
}

int main(int argc, char **argv){
  ros::init(argc, argv, "jakunen17_master");
  ros::NodeHandle nh;
  ros::Publisher pub_twist= nh.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
  ros::Publisher pub_hand_task = nh.advertise<std_msgs::Int8>("/hand_tast", 10);
  ros::Publisher pub_hand_num = nh.advertise<std_msgs::Int8>("/hand_num", 10);
  ros::Publisher pub_tgt_ang = nh.advertise<std_msgs::Int8>("/tgt_ang", 10);
  ros::Publisher pub_logger = nh.advertise<std_msgs::String>("/logger", 10);
  std_msgs::String log_msg;
  ros::Rate rate(10);
  geometry_msgs::Twist vel;
  std_msgs::Int8 fin;
  std_msgs::Int8 num;
  ros::Subscriber sub_robot_state = nh.subscribe("/robot_state", 100, callback_robot_state);
  ros::Subscriber sub_range_front = nh.subscribe("/range_ahead", 100, callback_range_front);
  
  log_msg.data = "Sent a stop command to the robot";
  while (ros::ok()) {
    switch(stage){
      case 0:
        log_msg.data = "suu test hazimaruyo";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        fin.data = 0;
        num.data = 0;
        pub_hand_num.publish(num);
        pub_hand_task.publish(fin);
        if(robot_mode==1){
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 1:
        log_msg.data = "num okuruyo";
        if(log_flag<1){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        num.data = 1;
        pub_hand_num.publish(num);
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 2:
        cou++;
        fin.data = 1;
        pub_hand_task.publish(fin);
        if(cou>=250){
          cou=0;
          stage++;
        }
        break;
      case 3:
        cou++;
        if(cou>=30){
          cou=0;
          stage++;
        }
        break;
      case 4:
        cou++;
        fin.data = 2;
        pub_hand_task.publish(fin);
        if(cou>=300){
          cou=0;
          stage++;
        }
        break;
      case 5:
        cou++;
        fin.data = 7;
        pub_hand_task.publish(fin);
        break;
      case 99:
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        break;
    }
    printf("state=%d\n",stage);
    if(robot_mode==0)stage=0;
    if(robot_mode==2)stage=99;
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}