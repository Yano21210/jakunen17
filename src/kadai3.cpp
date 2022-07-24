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
int stage_time_flag = 0;
std_msgs::Int8 ang_dir;

float f_kyori = -1;
float work_kyori = -1;
float f_kyori2 = -1;
float b_kyori = -1;
float r_kyori = -1;
float l_kyori = -1;
float sokudo = 0.15;
float gensoku= 0.07;
int img_r=0,img_l=0;
int left_image_last=-1,right_image_last=-1;

int sub_flag=0;

int log_flag=0;//logger用

void callback_range_front(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_front\t:\t%f\n\n", msg.data);
    f_kyori =  msg.data;
    sub_flag=1;
}
void callback_range_front2(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_front\t:\t%f\n\n", msg.data);
    work_kyori =  msg.data;
    sub_flag=1;
}
void callback_range_back(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_back\t:\t%f\n\n", msg.data);
     b_kyori =  msg.data;
    sub_flag=1;
}
void callback_range_right(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_right\t:\t%f\n\n", msg.data);
     r_kyori =  msg.data;
    sub_flag=1;
}
void callback_range_left(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_left\t:\t%f\n\n", msg.data);
    l_kyori =  msg.data;
    sub_flag=1;
}

void callback_img_right(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_right\t:\t%f\n\n", msg.data);
     img_r =  msg.data;
}
void callback_img_left(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_left\t:\t%f\n\n", msg.data);
    img_l =  msg.data;
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
  ros::Publisher pub_hand_num = nh.advertise<std_msgs::Int8>("/hand_num", 10);
  ros::Publisher pub_logger = nh.advertise<std_msgs::String>("/logger", 10);//logger用
  std_msgs::String log_msg;//logger用
  ros::Rate rate(10);
  geometry_msgs::Twist vel;
  std_msgs::Int8 fin;
  std_msgs::Int8 num;
  ros::Subscriber sub_robot_state = nh.subscribe("/robot_state", 100, callback_robot_state);
  ros::Subscriber sub_range_front = nh.subscribe("/range_ahead", 100, callback_range_front);
  ros::Subscriber sub_range_front2 = nh.subscribe("/range_front", 100, callback_range_front2);
  ros::Subscriber sub_range_back = nh.subscribe("/range_back", 100, callback_range_back);
  ros::Subscriber sub_range_right = nh.subscribe("/range_right", 100, callback_range_right);
  ros::Subscriber sub_range_left = nh.subscribe("/range_left", 100, callback_range_left);
  ros::Subscriber sub_img_right = nh.subscribe("/img_r", 100, callback_img_right);
  ros::Subscriber sub_img_left = nh.subscribe("/img_l", 100, callback_img_left);
  while (ros::ok()) {
    switch(stage){
      case 0:
        log_msg.data = "kadai1 node start";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        //画像処理判別
        switch(img_r){
          case 0:
            log_msg.data = "right image is Cyan A";
            break;
          case 1:
            log_msg.data = "right image is Magenta A";
            break;
          case 2:
            log_msg.data = "right image is Yellow A";
            break;
          case 3:
            log_msg.data = "right image is Cyan A";
            break;
          case 4:
            log_msg.data = "right image is Magenta A";
            break;
          case 5:
            log_msg.data = "right image is Yellow A";
            break;
          case 6:
            log_msg.data = "right image is Cyan A";
            break;
          case 7:
            log_msg.data = "right image is Magenta A";
            break;
          case 8:
            log_msg.data = "right image is Yellow A";
            break;
        }
        pub_logger.publish(log_msg);

        //画像処理判別
        switch(img_l){
          case 0:
            log_msg.data = "left image is Cyan A";
            break;
          case 1:
            log_msg.data = "left image is Magenta A";
            break;
          case 2:
            log_msg.data = "left image is Yellow A";
            break;
          case 3:
            log_msg.data = "left image is Cyan A";
            break;
          case 4:
            log_msg.data = "left image is Magenta A";
            break;
          case 5:
            log_msg.data = "left image is Yellow A";
            break;
          case 6:
            log_msg.data = "left image is Cyan A";
            break;
          case 7:
            log_msg.data = "left image is Magenta A";
            break;
          case 8:
            log_msg.data = "left image is Yellow A";
            break;
        }
        pub_logger.publish(log_msg);

        //画像処理結果記録
        right_image_last=img_r;
        left_image_last=img_l;

        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        fin.data=0;
        num.data = 0;
        pub_hand_num.publish(num);
        pub_hand_task.publish(fin);
        ang_dir.data=0;
        pub_tgt_ang.publish(ang_dir);
        if(robot_mode==1){
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 1://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.5*gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori<0.54){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 2://南向く
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=1;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=50){
          cou=0;
          stage++;
        }
        break;
      case 3://後ろ
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
        if(b_kyori<0.28){
          vel.linear.x =0.2*gensoku;
        }
        if(b_kyori>0.30){
          vel.linear.x =-0.2*gensoku;
        }
        if((b_kyori<0.30) && (b_kyori>0.28)){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
       case 4://左
          log_msg.data = "Case2_higasimuki";
          if(log_flag<4){
           pub_logger.publish(log_msg);
           log_flag++;
         }
         cou++;
         vel.linear.x = 0.0;
         vel.linear.y = gensoku;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
         if(b_kyori<0.27){
          vel.linear.x = 0.15*gensoku;
        }
         if(b_kyori>0.30){
          vel.linear.x = -0.15*gensoku;
        }
        if(r_kyori<0.92){
          vel.linear.y = 0.2*gensoku;
        }
        if(r_kyori>0.96){
          vel.linear.y = -0.2*gensoku;
        }
         if((r_kyori>0.920)&&(work_kyori<0.65)&&(r_kyori<0.95)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
      case 5://num_1送るよ
        num.data = 6;
        pub_hand_num.publish(num);
        log_msg.data = "Case7_numokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 6://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 1.1*gensoku;
        vel.linear.y = 0.07*gensoku;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori<0.19 && b_kyori>0.45){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 7://tast_1送るよ持ち上げ
         vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
        fin.data = 8;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 8://tast_0送るよ
        cou++;
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 9://num_1送るよ
        num.data = 5;
        pub_hand_num.publish(num);
        log_msg.data = "Case7_numokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 10://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = gensoku;
        vel.linear.y = 0.002;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori<0.20 && f_kyori<0.42){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 11://tast_1送るよ持ち上げ
         vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
        fin.data = 8;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 12://tast_0送るよ
        cou++;
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 13://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori < 0.50){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 14://南向く
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=0;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=50){
          cou=0;
          stage++;
        }
        break;
      case 15://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(b_kyori>0.75){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 16://右
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = -0.8*gensoku;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(l_kyori<0.675){
          vel.linear.y = -0.2*gensoku;
        }
        if(l_kyori>0.6650){
          vel.linear.y = 0.2*gensoku;
        }
        if((l_kyori>0.665)&&(l_kyori<0.675)&&(work_kyori<0.7)){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 17://右
        log_msg.data = "tyoto tomaru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.002;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(cou>10){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 18://num_4送るよ
        num.data = 4;
        pub_hand_num.publish(num);
        log_msg.data = "Case7_numokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.15*gensoku;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 19://前青一
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = gensoku;
        vel.linear.y = 0.005;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori<0.19){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 20://tast_1送るよ持ち上げ
        fin.data = 8;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 21://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 22://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori<0.19 && f_kyori<0.55){
          cou=0;
          log_flag=0;
          stage++;//強制終了
          stage++;
        }
        break;
      case 23://左
        log_msg.data = "tyoto tomaru";
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
        if(cou>10){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 24://前
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(work_kyori<0.19){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
      case 25://num_3送るよ
        num.data = 3;
        pub_hand_num.publish(num);
        log_msg.data = "Case7_numokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 26://tast_1送るよ持ち上げ
        fin.data = 8;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 27://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
     case 28://後ろ
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -1*gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(work_kyori>0.85){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 29://後ろ
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -0.5*gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori>0.78){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
     case 30://左
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = -0.2*gensoku;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(r_kyori<0.63){
          vel.linear.y = -0.2*gensoku;
        }
        if(r_kyori>0.70){
          vel.linear.y = -0.2*gensoku;
        }
        if(work_kyori<0.6){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 31://num_2送るよ
        num.data =2;
        pub_hand_num.publish(num);
        log_msg.data = "Case7_numokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.15*gensoku;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 32://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 1.5*gensoku;
        vel.linear.y = 0.005;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(work_kyori<0.19){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 33://前
        cou++;
        vel.linear.x = 1.3*gensoku;
        vel.linear.y = 0.003;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.55 && work_kyori<0.21){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
      case 34://tast_1送るよ持ち上げ
        fin.data = 8;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 35://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 36://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = gensoku;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        if(work_kyori<=0.18){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 37://前
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.35){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
      case 38://num_1送るよ
        num.data = 1;
        pub_hand_num.publish(num);
        log_msg.data = "Case7_numokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 39://tast_1送るよ持ち上げ
        fin.data = 8;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 40://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=5){
          cou=0;
          stage=56;
          log_flag=0;//logger用
        }
        break;
      case 56://前
         cou++;
         vel.linear.x = gensoku;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
         if(work_kyori>0.59){
          vel.linear.x = 0.5*gensoku;
        }
         if(work_kyori<0.66){
          vel.linear.x = -0.5*gensoku;
        }
         if((work_kyori>0.59)&&(work_kyori<0.64)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
      case 57://南向く
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=3;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=50){
          cou=0;
          stage=324;
        }
        break;
      case 324://前
         cou++;
         vel.linear.x = gensoku;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
         if(work_kyori>0.55){
          vel.linear.x = 0.5*gensoku;
        }
         if(work_kyori<0.61){
          vel.linear.x = -0.5*gensoku;
        }
         if((work_kyori>0.59)&&(work_kyori<0.60)){
           cou=0;
           log_flag=0;
           stage=575;//強制終了
         }
         break;
       case 575://左
          log_msg.data = "Case2_higasimuki";
          if(log_flag<4){
           pub_logger.publish(log_msg);
           log_flag++;
         }
         cou++;
         vel.linear.x = -0.002;
         vel.linear.y = -gensoku;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
         if(l_kyori>0.295){
          vel.linear.y = 0.5*gensoku;
        }
         if(l_kyori<0.355){
          vel.linear.y = -0.5*gensoku;
        }
         if((l_kyori>0.315)&&(l_kyori<0.335)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
           stage=58;
         }
         break;
      case 58://num_2送るよ
        num.data = 6;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 59://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case20_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
    case 60://tast_2送るよ
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case30_tast2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 61://num_2送るよ
        num.data = 5;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 62://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case20_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
    case 63://tast_2送るよ
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case30_tast2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=170){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 64://横
         cou++;
         vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
        if(l_kyori<0.85){
          vel.linear.y = -0.4*gensoku;
        }
         if(l_kyori>0.92){
          vel.linear.y = 0.4*gensoku;
        }
         if((l_kyori>0.85)&&(l_kyori<0.92)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
      case 65://num_4送るよ
        num.data =4;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 66://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case20_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
       case 67://tast_2送るよ
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case30_tast2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=140){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 68://num_3送るよ
        num.data =3;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 69://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case20_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 70://tast_2送るよ
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case30_tast2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=140){
          cou=0;
          stage=71;
          log_flag=0;
        }
        break;
      case 71://横
         cou++;
         vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
        if(r_kyori>0.43){
          vel.linear.y = -0.4*gensoku;
        }
         if(r_kyori<0.32){
          vel.linear.y = 0.4*gensoku;
        }
         if((r_kyori>0.32)&&(r_kyori<0.43)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
      case 72://num_2送るよ
        num.data = 2;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 73://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case20_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
    case 74://tast_2送るよ
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case30_tast2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=140){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
      case 75://num_2送るよ
        num.data = 1;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        vel.linear.x = 0.0;
         vel.linear.y = 0.0;
         vel.linear.z = 0.0;
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 76://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case20_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
    case 77://tast_2送るよ
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case30_tast2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=140){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
     case 78://北向き、後ろ
        log_msg.data = "Case37_usiro";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -0.5*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(r_kyori<0.32){
          printf("aaaaa=\n");
          vel.linear.y = 0.3*gensoku;
         }
         if(r_kyori>0.34){
          printf("aaaaa=\n");
          vel.linear.y = -0.3*gensoku;
         }
        if(b_kyori<0.55){
          vel.linear.x = -0.3*gensoku;
        }
        if(sub_flag==0){
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        }
       if(b_kyori<0.35){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 79://右
         cou++;
         vel.linear.x =0.002;
         vel.linear.y = 0.5*gensoku;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
         if(r_kyori<0.90){
          vel.linear.y = 0.3*gensoku;
        }
         if(r_kyori>0.83){
          vel.linear.y = -0.3*gensoku;
        }
         if((r_kyori>0.83)&&(r_kyori<0.95)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
       case 80://北向き
        log_msg.data = "Case38_Kitamuki";
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
      case 81://end
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
         if(l_kyori>0.34){
          vel.linear.y = 0.3*gensoku;
        }
         if(l_kyori<0.30){
          vel.linear.y = -0.3*gensoku;
        }
        if(l_kyori<0.34&&l_kyori>0.30){
          fin.data=7;
          pub_hand_task.publish(fin);
        }
        break;
      case 99://非常停止
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(robot_mode==0)stage=0;
        break;
    }

    if(robot_mode==4){
      stage=99;
      log_flag=0;
    }
    pub_twist.publish(vel);
    if(robot_mode==0)stage=0;
    if(robot_mode==4)stage=99;
    printf("state=%d\n",stage);
    printf("F222_kyori:%f\n",work_kyori);
    printf("F_kyori:%f\n",f_kyori);
    printf("B_kyori:%f\n",b_kyori);
    printf("R_kyori:%f\n",r_kyori);
    printf("L_kyori:%f\n\n",l_kyori);
    sub_flag=0;
    ros::spinOnce();
    rate.sleep();
  }
  return 0;
}