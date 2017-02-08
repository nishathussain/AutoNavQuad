#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Quaternion.h"
//#include <pthread.h>
#include <sstream>
#include <tf/transform_broadcaster.h>
//#include <Matrix3x3.h>
//#include <math.h>

double PI=3.14159265;
nav_msgs::Odometry msg;
void callbk(const nav_msgs::Odometry::ConstPtr& odom)
{
  msg.header=odom->header;
  msg.pose=odom->pose;
  msg.twist=odom->twist;
}

int main(int argc, char **argv){
  ros::init(argc, argv, "odom_republish");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<nav_msgs::Odometry>("/quad/ground_truth/odometry", 1);
  ros::Rate loop_rate(50);

  msg.pose.pose.orientation.w=1.0;
  ros::Subscriber sub = n.subscribe("/stereo/odom", 1,callbk); 
  while (ros::ok())
  {
    msg.header.frame_id="odom";
    msg.child_frame_id="base_link";
//    msg.child_frame_id="map";
    msg.header.stamp=ros::Time::now();
//    msg.header.stamp.nsecs=ros::Time::now();
       // ROS_INFO("Published msg %f, %f,%f", msg.x,msg.y,msg.z);
    chatter_pub.publish(msg); //keep msg in buffer
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}