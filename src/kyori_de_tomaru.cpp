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
float sokudo = 0.15;
float gensoku= 0.05;

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
  ros::Publisher pub_hand_num = nh.advertise<std_msgs::Int8>("/hand_num", 10);
  ros::Publisher pub_logger = nh.advertise<std_msgs::String>("/logger", 10);//logger用
  std_msgs::String log_msg;//logger用
  ros::Rate rate(10);
  geometry_msgs::Twist vel;
  std_msgs::Int8 fin;
  std_msgs::Int8 num;
  ros::Subscriber sub_robot_state = nh.subscribe("/robot_state", 100, callback_robot_state);
  ros::Subscriber sub_range_front = nh.subscribe("/range_ahead", 100, callback_range_front);
  ros::Subscriber sub_range_back = nh.subscribe("/range_back", 100, callback_range_back);
  ros::Subscriber sub_range_right = nh.subscribe("/range_right", 100, callback_range_right);
  ros::Subscriber sub_range_left = nh.subscribe("/range_left", 100, callback_range_left);
  while (ros::ok()) {
    switch(stage){
      case 0:
        log_msg.data = "kadai1 node start";
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
        fin.data=0;
        num.data = 0;
        pub_hand_num.publish(num);
        pub_hand_task.publish(fin);
        if(robot_mode==1){
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 1: //まっすぐ
        log_msg.data = "Case1_maenisusumuyo";
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
        if(f_kyori<0.50){
          vel.linear.x = gensoku;
        }
        if(f_kyori<0.40){
          cou=0;
          log_flag++;
          stage++;
        }
        break;
      case 2://左
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = -1*sokudo;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
       if(r_kyori<0.68){
          printf("aaaaa=\n");
          vel.linear.y = -1*gensoku;
        }
       if(r_kyori<0.46){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 3: //まっすぐ
        log_msg.data = "Case3_maenisusumuyo";
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
        if(f_kyori<0.65){
          printf("aaaaa=\n");
          vel.linear.x = gensoku;
        }
        if(f_kyori<0.65){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 4://西向き
        log_msg.data = "Case4_mawaremigi";
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
        ang_dir.data=2;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=40){
          cou=0;
          log_flag=0;
          stage++;
        }
        break;
    case 5: //西向き右合わせ
        log_msg.data = "Case5_migiitiawase";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = -1.2*gensoku;
        if(r_kyori<0.43){
          printf("aaaaa=\n");
          vel.linear.y = -1*gensoku;
        }
        if(r_kyori<0.37){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 6://西向きまっすぐ
        log_msg.data = "Case6_massugu";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.5*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
       if(r_kyori<0.29){
          vel.linear.y = 0.5*gensoku;
        }
        if(f_kyori<0.63){
          vel.linear.x = gensoku;
        }
       if(f_kyori<0.52 && l_kyori<0.48){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 7://num_3送るよ
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
    case 8://tast_1送るよ持ち上げ
        fin.data = 1;
        pub_hand_task.publish(fin);
        log_msg.data = "Case8_tastokuru";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=150){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    // case 9://1.5秒待つ
    //     log_msg.data = "Case9_matu";
    //     if(log_flag<4){
    //       pub_logger.publish(log_msg);
    //       log_flag++;
    //     }
    //     cou++;
    //     if(cou>=15){
    //       cou=0;
    //       stage++;
    //       log_flag=0;
    //     }
    //     break;
    case 9://tast_0送るよ
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
    case 10: //前
        log_msg.data = "Case11_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.4*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(cou>10){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 11://num_2送るよ
        num.data = 2;
        pub_hand_num.publish(num);
        log_msg.data = "Case12_num2";
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
    case 12://tast_1送るよ持ち上げ
        fin.data = 1;
        pub_hand_task.publish(fin);
        log_msg.data = "Case13_tast1";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=150){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    // case 14://1.5秒待つ
    //     log_msg.data = "Case14_3matu";
    //     if(log_flag<4){
    //       pub_logger.publish(log_msg);
    //       log_flag++;
    //     }
    //     cou++;
    //     if(cou>=15){
    //       cou=0;
    //       stage++;
    //       log_flag=0;
    //     }
    //     break;
    case 13://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case15_num0";
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
    case 14: //まっすぐ
        log_msg.data = "Case16_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.2*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.34){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 15://num_1送るよ
        num.data = 1;
        pub_hand_num.publish(num);
        log_msg.data = "Case17_num3";
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
    case 16://tast_1持ち上げ
        fin.data = 1;
        pub_hand_task.publish(fin);
        log_msg.data = "Case18_tast1";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        if(cou>=150){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    //  case 19://1.5秒待つ
    //     log_msg.data = "Case19_3matu";
    //     if(log_flag<4){
    //       pub_logger.publish(log_msg);
    //       log_flag++;
    //     }
    //     cou++;
    //     if(cou>=15){
    //       cou=0;
    //       stage++;
    //       log_flag=0;
    //     }
    //     break;
    case 17://tast_0送るよ
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
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
    case 18://左向き、後ろ1
        log_msg.data = "Case21_usiro1";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -1*sokudo;
        vel.angular.y = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.6){
          printf("aaaaa=\n");
          vel.linear.x = -1.6*gensoku;
        }
       if(r_kyori<0.30){
          printf("aaaaa=\n");
          vel.linear.y = 0.5*gensoku;
        }
      if(r_kyori>0.45){
          printf("aaaaa=\n");
          vel.linear.y = -0.5*gensoku;
        }
       if(l_kyori>0.5&&f_kyori>0.6){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
     case 19://左向き、後ろ2
        log_msg.data = "Case22_usiro2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -0.7*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(b_kyori<0.52){
          printf("aaaaa=\n");
          vel.linear.x = -0.5*gensoku;
        }
       if(r_kyori<0.29){
          printf("aaaaa=\n");
          vel.linear.y = 0.5*gensoku;
        }
       if(r_kyori>0.45){
          printf("aaaaa=\n");
          vel.linear.y = -0.5*gensoku;
        }
       if(b_kyori<0.47){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 20://正面向き
        log_msg.data = "Case23_maemuku";
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
    case 21: //正面向き、バック
        log_msg.data = "Case24_back";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -1*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(b_kyori<0.6){
          printf("aaaaa=\n");
          vel.linear.x = -1*gensoku;
        }
        if(l_kyori<0.30){
          printf("aaaaa=\n");
          vel.linear.y = -1*gensoku;
        }
        if(r_kyori<0.30){
          printf("aaaaa=\n");
          vel.linear.y = gensoku;
        }
        if(b_kyori<0.38){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 22://東向く
        log_msg.data = "Case23_higasimuku";
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
   case 23: //前
        log_msg.data = "Case24_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.3*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(r_kyori>0.42){
          vel.linear.y = -1*gensoku;
        }
        if(r_kyori<0.32){
          vel.linear.y = gensoku;
        }
        if(l_kyori<0.80){
          cou=0;
          stage++;
          log_flag=0;//logger用
        }
        break;
    case 24://1.5秒待つ
       log_msg.data = "Case24_mae";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.25*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        cou++;
        if(cou>=10){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 25://tast_0送るよ
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
    case 26://num_1送るよ
        num.data = 1;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
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
    case 27://tast_2送るよ下ろす
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case26_tast2";
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
    // case 30://1.5秒待つ
    //     log_msg.data = "Case27_3matu";
    //     if(log_flag<4){
    //       pub_logger.publish(log_msg);
    //       log_flag++;
    //     }
    //     cou++;
    //     if(cou>=15){
    //       cou=0;
    //       stage++;
    //       log_flag=0;
    //     }
    //     break;
    case 28://num_2送るよ
        num.data = 2;
        pub_hand_num.publish(num);
        log_msg.data = "Case29_num2";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag=0;//logger用
        }
        cou++;
        if(cou>=5){
          cou=0;
          stage++;
          log_flag=0;
        }
        break;
    case 29://tast_0送るよ
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
    case 30://tast_2送るよ
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
    case 31://num_3送るよ
        num.data = 3;
        pub_hand_num.publish(num);
        log_msg.data = "Case33_num3";
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
    case 32://tast_0送るよ
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
    case 33://tast_2送るよ下ろす
        fin.data = 2;
        pub_hand_task.publish(fin);
        log_msg.data = "Case34_tast2";
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
    //  case 33://1.5秒待つ
    //     log_msg.data = "Case35_3matu";
    //     if(log_flag<4){
    //       pub_logger.publish(log_msg);
    //       log_flag++;
    //     }
    //     cou++;
    //     if(cou>=15){
    //       cou=0;
    //       stage++;
    //       log_flag=0;
    //     }
    //     break;
    case 34://北向き、後ろ
        log_msg.data = "Case37_usiro";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = -1*sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(r_kyori<0.30){
          printf("aaaaa=\n");
          vel.linear.y = 0.5*gensoku;
         }
         if(r_kyori>0.33){
          printf("aaaaa=\n");
          vel.linear.y = -0.5*gensoku;
         }
        if(b_kyori<0.45){
          vel.linear.x = -1*gensoku;
        }
       if(b_kyori<0.38){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 35://北向き
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
    case 36: //北向き合わせ
        log_msg.data = "Case5_migiitiawase";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        if(l_kyori>0.31){
          printf("aaaaa=\n");
          vel.linear.y = gensoku;
        }
        if(b_kyori>0.33){
          printf("aaaaa=\n");
          vel.linear.x = -1*gensoku;
        }
        if(l_kyori<=0.4 && b_kyori<=0.4){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
    case 37://End
        log_msg.data = "Case39_end";
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
        fin.data = 7;
        pub_hand_task.publish(fin);
        if(robot_mode==0){
          log_flag=0;
          stage=0;
        }
        break;
    case 99://エラー
        log_msg.data = "Case99_Error";
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
        fin.data = 7;
        pub_hand_task.publish(fin);
        pub_twist.publish(vel);
        break;
    }

    pub_twist.publish(vel);
    // if(robot_mode==0){
    //   log_flag=0;
    //   stage=0;
    // }
    if(robot_mode==4){
      stage=99;
      log_flag=0;
    }
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