#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Header.h"
#include "geometry_msgs/Quaternion.h"
#include <sstream>
#include <tf/transform_broadcaster.h>

double PI=3.14159265;
sensor_msgs::Imu msg;
void callbk(const sensor_msgs::Imu::ConstPtr& imu)
{
  msg.header=imu->header;
  msg.orientation=imu->orientation;
  msg.orientation_covariance=imu->orientation_covariance;
  msg.angular_velocity=imu->angular_velocity;
  msg.linear_acceleration_covariance=imu->linear_acceleration_covariance;
}

int main(int argc, char **argv){
  ros::init(argc, argv, "imu_remap");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<sensor_msgs::Imu>("/quad/imu", 1);
  ros::Rate loop_rate(100);

  msg.header.stamp=ros::Time::now();
  msg.orientation.w=1;
//  msg.orientation_covariance=;
//  msg.angular_velocity=;
//  msg.linear_acceleration_covariance=;

  ros::Subscriber sub = n.subscribe("/mavros/imu/data", 1,callbk); 
  while (ros::ok())
  {
    msg.header.frame_id="odom";
    msg.header.stamp=ros::Time::now();
//    msg.header.stamp.nsecs=ros::Time::now();
       // ROS_INFO("Published msg %f, %f,%f", msg.x,msg.y,msg.z);
    chatter_pub.publish(msg); //keep msg in buffer
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
