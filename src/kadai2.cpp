#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/Int8.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>//logger用
using namespace std;
#include <stdio.h>

int robot_mode=0;
int stage = 0;
int cou=0;
std_msgs::Int8 ang_dir;

float f_kyori = -1;
float b_kyori = -1;
float r_kyori = -1;
float l_kyori = -1;
float sokudo = 0.1;
float gensoku= 0.07;

int log_flag=0;//logger用

void callback_range_front(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_front\t:\t%f\n\n", msg.data);
    f_kyori =  msg.data;
}
void callback_range_back(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_back\t:\t%f\n\n", msg.data);
     b_kyori =  msg.data;
}
void callback_range_right(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_right\t:\t%f\n\n", msg.data);
     r_kyori =  msg.data;
}
void callback_range_left(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_left\t:\t%f\n\n", msg.data);
    l_kyori =  msg.data;
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
  ros::Publisher pub_tgt_ang = nh.advertise<std_msgs::Int8>("/tgt_ang", 10);
  ros::Publisher pub_logger = nh.advertise<std_msgs::String>("/logger", 10);//logger用
  std_msgs::String log_msg;//logger用
  ros::Rate rate(10);
  geometry_msgs::Twist vel;
  std_msgs::Int8 fin;
  ros::Subscriber sub_robot_state = nh.subscribe("/robot_state", 100, callback_robot_state);
  ros::Subscriber sub_range_front = nh.subscribe("/range_ahead", 100, callback_range_front);
  ros::Subscriber sub_range_back = nh.subscribe("/range_back", 100, callback_range_back);
  ros::Subscriber sub_range_right = nh.subscribe("/range_right", 100, callback_range_right);
  ros::Subscriber sub_range_left = nh.subscribe("/range_left", 100, callback_range_left);
  while (ros::ok()) {
    switch(stage){
      case 0:
         //logger用
        log_msg.data = "kadai2 node start";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        fin.data=0;
        pub_hand_task.publish(fin);
        if(robot_mode==1 || 1){
          stage++;
          log_flag=0;//logger用
        }
      case 1: //後ろ
        log_msg.data = "Case1_usiro";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0; 
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.55){
          printf("aaaaa=\n");
          vel.linear.x = gensoku;
        }
        if(f_kyori<0.42 && r_kyori>0.5){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break; 
      case 2://東向き
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=3;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=40){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 3: //東向き、前
        log_msg.data = "Case3_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(b_kyori<0.50){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 4://3秒待ち
        log_msg.data = "Case4_mati";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(cou>=30){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 5: //東向き、前
        log_msg.data = "Case5_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        //pub_twist.publish(vel);
        // if(cou>=40){
        if(b_kyori<1.0){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 6://3秒待ち
        log_msg.data = "Case6_mati";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(cou>=30){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 7://北向き
        log_msg.data = "Case7_kitamuku";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=0;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=40){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 8: //北向き、前
        log_msg.data = "Case8_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.6){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 9://3秒待ち
        log_msg.data = "Case9_mati";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(cou>=30){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 10: //北向き、前
        log_msg.data = "Case10_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.5){
          cou=0;
          stage++;//強制終了
        }
        break;
      case 11://3秒待ち
        log_msg.data = "Case11_mati";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(cou>=30){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 12://西向き
        log_msg.data = "Case12_nisimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=1;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=40){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 13: //西向き、前
        log_msg.data = "Case13_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.8){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 14://3秒待ち
        log_msg.data = "Case14_mati";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(cou>=30){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 15://南向き
        log_msg.data = "Case15_minamimuku";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=1;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=40){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 16: //西向き、前
        log_msg.data = "Case16_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.8){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 17://3秒待ち
        log_msg.data = "Case17_matu";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(cou>=30){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
      case 18://end
        log_msg.data = "Case18_end";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(robot_mode==0){
          log_flag=0;
          stage=0;
        }
        break;
      case 99://非常停止
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        pub_twist.publish(vel);
        if(robot_mode==0)stage=0;
        break;
    }
    pub_twist.publish(vel);
    if(robot_mode==0)stage=0;
    if(robot_mode==4)stage=99;
    printf("state=%d\n",stage);
    printf("F_kyori:%f\n",f_kyori);
    printf("B_kyori:%f\n",b_kyori);
    printf("R_kyori:%f\n",r_kyori);
    printf("L_kyori:%f\n\n",l_kyori);
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}