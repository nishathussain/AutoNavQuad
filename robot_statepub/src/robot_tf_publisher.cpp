#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Vector3Stamped.h>
#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/Transform.h>

geometry_msgs::TransformStamped odom_trans;

  void callbk(const nav_msgs::Odometry::ConstPtr& odom){
     odom_trans.header.stamp = odom->header.stamp;
     odom_trans.header.frame_id = "odom";
     odom_trans.child_frame_id = "base_link";
     odom_trans.transform.translation.x = odom->pose.pose.position.x;
     odom_trans.transform.translation.y = odom->pose.pose.position.y;
     odom_trans.transform.translation.z = odom->pose.pose.position.z;
     odom_trans.transform.rotation = odom->pose.pose.orientation; 
  }
int main(int argc, char** argv){
   ros::init(argc, argv, "robot_tf_publisher");
   ros::NodeHandle n;
   tf::TransformBroadcaster odom_broadcaster;
   ros::Rate r(50);
   ros::Subscriber sub = n.subscribe("quad/ground_truth/odometry", 1,callbk); 
     odom_trans.header.stamp = ros::Time::now();
     odom_trans.header.frame_id = "odom";
     odom_trans.child_frame_id = "base_link";
     odom_trans.transform.translation.x = 0;odom_trans.transform.translation.y = 0;odom_trans.transform.translation.z = 0;
     odom_trans.transform.rotation.x=0;odom_trans.transform.rotation.y=0;odom_trans.transform.rotation.z=0;odom_trans.transform.rotation.w=1;
   while(n.ok()){
   odom_trans.header.stamp = ros::Time::now();
   odom_broadcaster.sendTransform(odom_trans);
   ros::spinOnce();
     r.sleep();
   }
}
