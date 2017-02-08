#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "nav_msgs/Odometry.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Header.h"
#include "mavros_msgs/Waypoint.h"
#include "geometry_msgs/Quaternion.h"
#include "mav_msgs/CommandTrajectory.h"
#include <sstream>
#include <tf/transform_broadcaster.h>

double PI=3.14159265;
mavros_msgs::Waypoint way;
void callbk(const mav_msgs::CommandTrajectory::ConstPtr& cmdTr)
{
way.frame=0;//cmdTr->header.frame_id;
way.command=0.0;
way.is_current=true;
way.autocontinue=true;
way.param1=cmdTr->yaw;
way.param2=cmdTr->yaw_rate;
way.param3=0.0;
way.param4=0.0;
way.x_lat =cmdTr->position.x;
way.y_long=cmdTr->position.y;
way.z_alt =cmdTr-> position.z;
//cmdTr->header //cmdTr->position //cmdTr->velocity //cmdTr->acceleration
}

int main(int argc, char **argv){
  ros::init(argc, argv, "waypoint_remap");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<mavros_msgs::Waypoint>("/mavros/mission/waypoints", 1);
  ros::Rate loop_rate(5);

  ros::Subscriber sub = n.subscribe("/cmd_3dnav", 100,callbk); 
  while (ros::ok())
  {


    chatter_pub.publish(way); //keep msg in buffer
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
