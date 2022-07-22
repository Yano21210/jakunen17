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

int log_flag=0;//logger用

void callback_range_front(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_front\t:\t%f\n\n", msg.data);
    f_kyori =  msg.data;
}
void callback_range_front2(const std_msgs::Float32& msg) {
    //ROS_INFO("/range_front\t:\t%f\n\n", msg.data);
    work_kyori =  msg.data;
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
  ros::Subscriber sub_range_front2 = nh.subscribe("/range_front", 100, callback_range_front2);
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
      case 1: //後ろ
        log_msg.data = "Case1_usiro";
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
        if(b_kyori<0.55){
          vel.linear.x = -1*gensoku;
        }
        if(b_kyori<0.32){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break; 
      case 2: //右
        log_msg.data = "Case1_usiro";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = -1*gensoku;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0; 
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(b_kyori<0.26){
          vel.linear.x = gensoku;
        }
        if(l_kyori>0.90){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 3://前
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
        if(b_kyori>0.8){
          cou=0;
          log_flag=0;
          stage=18;//強制終了
        }
        break;
      // case 4://左あわせ
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 0.0;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(l_kyori>0.285){
      //     printf("aaaaa=\n");
      //     vel.linear.y = 0.2*gensoku;
      //   }
      //   if(l_kyori<0.28){
      //     printf("aaaaa=\n");
      //     vel.linear.y = -0.2*gensoku;
      //   }
      //   if(l_kyori>0.28&&l_kyori<0.285){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      //   break;
      // case 5://前
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = gensoku;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(work_kyori<0.20){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      //   break;
      // case 6://前
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = gensoku;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(cou>=15){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      //   break;
      // case 7://num_6送るよ
      //   num.data = 6;
      //   pub_hand_num.publish(num);
      //   log_msg.data = "Case7_numokuru";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 0.0;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   if(cou>=5){
      //     cou=0;
      //     stage++;
      //     log_flag=0;//logger用
      //   }
      //   break;
      // case 8://tast_1送るよ持ち上げ
      //   fin.data = 1;
      //   pub_hand_task.publish(fin);
      //   log_msg.data = "Case8_tastokuru";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   if(cou>=230){
      //     cou=0;
      //     stage++;
      //     log_flag=0;
      //   }
      //   break;
      // case 9://tast_0送るよ
      //   fin.data=0;
      //   pub_hand_task.publish(fin);
      //   log_msg.data = "Case10_num0";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 0.0;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   if(cou>=5){
      //     cou=0;
      //     stage++;
      //     log_flag=0;//logger用
      //   }
      //   break;
      // case 10://前
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = gensoku;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(work_kyori<0.21){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      //   break;
      // case 11://前
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 1.2*gensoku;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(cou>=30){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      // case 12://num_5送るよ
      //   num.data = 5;
      //   pub_hand_num.publish(num);
      //   log_msg.data = "Case7_numokuru";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 0.0;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   if(cou>=5){
      //     cou=0;
      //     stage++;
      //     log_flag=0;//logger用
      //   }
      //   break;
      // case 13://tast_1送るよ持ち上げ
      //   fin.data = 1;
      //   pub_hand_task.publish(fin);
      //   log_msg.data = "Case8_tastokuru";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   if(cou>=230){
      //     cou=0;
      //     stage++;
      //     log_flag=0;
      //   }
      //   break;
      // case 14://tast_0送るよ
      //   fin.data=0;
      //   pub_hand_task.publish(fin);
      //   log_msg.data = "Case10_num0";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 0.0;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   if(cou>=5){
      //     cou=0;
      //     stage++;
      //     log_flag=0;//logger用
      //   }
      //   break;
      // case 15://左あわせ
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = 0.0;
      //   vel.linear.y = -1*gensoku;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(cou>=10){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      //   break;
      // case 16://後ろ
      //   log_msg.data = "Case2_higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = -1*gensoku;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0; higasimuki";
      //   if(log_flag<4){
      //     pub_logger.publish(log_msg);
      //     log_flag++;
      //   }
      //   cou++;
      //   vel.linear.x = -0.5*gensoku;
      //   vel.linear.y = 0.0;
      //   vel.linear.z = 0.0;
      //   vel.angular.x = 0.0;
      //   vel.angular.y = 0.0;
      //   vel.angular.z = 0.0;
      //   if(b_kyori<0.95 && work_kyori>0.87){
      //     cou=0;
      //     log_flag=0;
      //     stage++;//強制終了
      //   }
      //   break;
      case 18://左
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
        if(l_kyori<0.685){
          vel.linear.y = -0.4*gensoku;
        }
        if(l_kyori>0.675){
          vel.linear.y = 0.4*gensoku;
        }
        if((l_kyori>0.670)&&(l_kyori<0.685)&&(work_kyori<0.60)){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 19://前
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
        if(work_kyori<0.19){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 20://num_4送るよ
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
      case 21://tast_1送るよ持ち上げ
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
      case 22://tast_0送るよ
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
      case 23://前
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
        pub_twist.publish(vel);
        if(work_kyori<0.19){
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
        if(f_kyori<0.32 && work_kyori<0.19){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
      case 25://num_3送るよ
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
      case 26://tast_1送るよ持ち上げ
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
        if(work_kyori>0.7){
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
        if(f_kyori>0.60){
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
        vel.linear.y = -0.5*gensoku;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(l_kyori>0.60){
          vel.linear.y = -0.5*gensoku;
        }
        if(work_kyori<0.5 || r_kyori<0.1){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
        break;
      case 31://前
        log_msg.data = "Case2_higasimuki";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
        cou++;
        vel.linear.x = 1.5*gensoku;
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
        break;
      case 32://前
        cou++;
        vel.linear.x = sokudo;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        if(f_kyori<0.38 && work_kyori<0.19){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
      case 33://num_2送るよ
        num.data =4;
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
      case 34://tast_1送るよ持ち上げ
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
        if(work_kyori<0.19){
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
        if(f_kyori<0.38){
          cou=0;
          log_flag=0;
          stage++;//強制終了
        }
      case 38://num_1送るよ
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
      case 39://tast_1送るよ持ち上げ
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
          stage++;
          log_flag=0;//logger用
        }
        break;
      case 41://前
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
         if(work_kyori<0.6){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
      case 42://南向く
        cou++;
        vel.linear.x = 0.0;
        vel.linear.y = 0.0;
        vel.linear.z = 0.0;
        vel.angular.x = 0.0;
        vel.angular.y = 0.0;
        vel.angular.z = 0.0;
        ang_dir.data=1;
        pub_tgt_ang.publish(ang_dir);
        if(cou>=30){
          cou=0;
          stage++;
        }
        break;
        case 43://右
          log_msg.data = "Case2_higasimuki";
          if(log_flag<4){
           pub_logger.publish(log_msg);
           log_flag++;
         }
         cou++;
         vel.linear.x = 0.0;
         vel.linear.y = -1*gensoku;
         vel.linear.z = 0.0;
         vel.angular.x = 0.0;
         vel.angular.y = 0.0;
         vel.angular.z = 0.0;
         if(b_kyori<0.26){
          vel.linear.x = 0.5*gensoku;
        }
         if(b_kyori>0.31){
          vel.linear.x = -0.3*gensoku;
        }
        if(r_kyori>0.28){
          vel.linear.y = -0.3*gensoku;
        }
         if((r_kyori>0.80)&&(work_kyori<0.65)&&(l_kyori>0.6)){
           cou=0;
           log_flag=0;
           stage++;//強制終了
         }
         break;
      case 44://tast_1送るよ持ち上げ
        fin.data = 4;
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
      case 45://tast_0送るよ
        fin.data=0;
        pub_hand_task.publish(fin);
        log_msg.data = "Case10_num0";
        if(log_flag<4){
          pub_logger.publish(log_msg);
          log_flag++;
        }
      case 46://end
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

    if(robot_mode==4){
      stage=99;
      log_flag=0;
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