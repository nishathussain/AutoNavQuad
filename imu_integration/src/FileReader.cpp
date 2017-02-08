#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <stdio.h>
#include <string.h>
//#include </usr/include/gazebo-6.6/gazebo/math/gzmath.hh>
//#include </usr/include/gazebo-6.6/gazebo/math/Angle.hh>
//using namespace std;
sensor_msgs::Imu Imu;
double roll,pitch,yaw,accX,accY,accZ;
double t0,t1,t2,t3,t4,t5;
double q[4];
std::string STRING;
float Tokens[7];
char * pch;
double PI=3.14159265;

std::ifstream infile;


int main(int argc, char **argv)
{
  ros::init(argc, argv, "reader");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<sensor_msgs::Imu>("IMUdatafile", 1);
  ros::Rate loop_rate(10);
  roll=0.0,pitch=0.0,yaw=0.0,accX=0.0,accY=0.0,accZ=0.0;

  infile.open ("/home/nishat/Desktop/data_IMU_64.csv");
  while (ros::ok() & std::getline(infile,STRING)>0)
  {

//     while(getline(infile,STRING)>0) // Saves the line in STRING.
//	{


            pch = strtok (&STRING[0],",");
            int i=0;
  	while ((pch != NULL)&&(i<7))
 	 { 
                Tokens[i]=std::atof(pch);
                i++;
    		pch = strtok (NULL, ",");
         }   
roll=Tokens[4];        
pitch=Tokens[5];
yaw=Tokens[6];

accX=Tokens[1];
accY=Tokens[2];
accZ=Tokens[3];
//        for(int j=0;j<7;j++)
//          std::cout<<Tokens[j]<<", ";
//          std::cout<<std::endl;  

//        }


//q=Quaternion(roll,pitch,yaw);

t0 = cos(yaw * 0.5f* PI / 180.0);
t1 = sin(yaw * 0.5f* PI / 180.0);
t2 = cos(roll * 0.5f* PI / 180.0);
t3 = sin(roll * 0.5f* PI / 180.0);
t4 = cos(pitch * 0.5f* PI / 180.0);
t5 = sin(pitch * 0.5f* PI / 180.0);


    Imu.header.seq=0;
    Imu.header.stamp.nsec=Tokens[0];
//    Imu.header.frame_id;
    Imu.orientation.w=t0 * t2 * t4 + t1 * t3 * t5;
    Imu.orientation.x=t0 * t3 * t4 - t1 * t2 * t5;
    Imu.orientation.y=t0 * t2 * t5 + t1 * t3 * t4;
    Imu.orientation.z=t1 * t2 * t4 - t0 * t3 * t5;
    Imu.linear_acceleration.x=accX;
    Imu.linear_acceleration.y=accY;
    Imu.linear_acceleration.z=accZ;
    Imu.angular_velocity.x=roll;
    Imu.angular_velocity.y=pitch;
    Imu.angular_velocity.z=yaw;
    chatter_pub.publish(Imu);
    ros::spinOnce();
    loop_rate.sleep();
  }

  infile.close();
  return 0;
}
