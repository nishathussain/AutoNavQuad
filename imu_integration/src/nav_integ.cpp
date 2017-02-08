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

geometry_msgs::Vector3 pos, pos0;
tf::Quaternion orientation0, orientation1;
tfScalar y, p, r,ya,pi,ro;
double axE,ayE,azE,azEn,axB,ayB,azB;
double vxE,vyE,vzE;
double norm=0.0,normB=0.0;
//std_msgs::Header tm0, tm1;
double dt;
double tm1=0,tm0=0;
double matrix[3][3], matrixT[3][3];
double PI=3.14159265;
nav_msgs::Odometry msg;
void callbk(const sensor_msgs::Imu::ConstPtr& imu)
{

  tf::quaternionMsgToTF(imu->orientation, orientation1);
  tf::Matrix3x3(orientation1).getEulerYPR(ya, pi, ro,1);//(psi,theta,phi) YXZ::yaw Z axis, pitch Y axis, roll X axis
//Z Y X yaw, pitch, and roll about Z, Y, X axes respectively 

y= ro ; //ro
p= pi ; //pi
r= ya ; //ya
//tf::Matrix3x3(orientation1).getRPY(r, p, y);
msg.pose.pose.orientation.x=imu->orientation.x;
msg.pose.pose.orientation.y=imu->orientation.y;
msg.pose.pose.orientation.z=imu->orientation.z;
msg.pose.pose.orientation.w=imu->orientation.w;
//ROS_INFO("%f",imu->orientation.x);
//ROS_INFO("%d"imu->header.stamp);
if(tm0==0){
  tm0=imu->header.stamp.sec*pow(10,9)+imu->header.stamp.nsec;
  return;
  }else{
  tm1=imu->header.stamp.sec*pow(10,9)+imu->header.stamp.nsec;
  dt=((tm1-tm0)/pow(10,9)>0)?(tm1-tm0)/pow(10,9):dt;
//  std::cout<<dt<<" : "<<tm0<<" : "<<tm1<<" : "<<std::endl;
  tm0=tm1;
  }
//ROS_INFO(" %d,%d,%d", imu->header.seq,imu->header.stamp.sec,imu->header.stamp.nsec);
//tm1=imu->header.stamp.sec;
//ROS_INFO("msg %f, %f,%f", ya*180/PI,pi*180/PI,ro*180/PI);
//ROS_INFO("msg %f, %f,%f", ya*180/PI,pi*180/PI,ro*180/PI);
//ROS_INFO("msg %f, %f,%f", yaw*180/PI,pitch*180/PI,roll*180/PI);
//ROS_INFO("msg %f, %f,%f,%f", orientation1[0],orientation1[1],orientation1[2],orientation1[3]);
//ROS_INFO("msg %f", cos(0));
matrix[0][0]=cos(p)*cos(r);                        matrix[0][1]=cos(p)*sin(r);                       matrix[0][2]=-sin(p);
matrix[1][0]=-cos(y)*sin(r)+sin(y)*sin(p)*cos(r);  matrix[1][1]=cos(y)*cos(r)+sin(y)*sin(p)*sin(r);  matrix[1][2]=sin(y)*cos(p);
matrix[2][0]=sin(y)*sin(r)+cos(y)*sin(p)*cos(r);   matrix[2][1]=-sin(y)*cos(r)+cos(y)*sin(p)*sin(r); matrix[2][2]=cos(y)*cos(p);

matrixT[0][0]=matrix[0][0] ; matrixT[0][1]=matrix[1][0] ; matrixT[0][2]=matrix[2][0];
matrixT[1][0]=matrix[0][1] ; matrixT[1][1]=matrix[1][1] ; matrixT[1][2]=matrix[2][1];
matrixT[2][0]=matrix[0][2] ; matrixT[2][1]=matrix[1][2] ; matrixT[2][2]=matrix[2][2];

axB=imu->linear_acceleration.x;
ayB=imu->linear_acceleration.y;
azB=imu->linear_acceleration.z;
//normB=std::sqrt(axB*axB+ayB*ayB+azB*azB);
//axE=axE/norm;
//ayE=ayE/norm;
//azE=(azE/norm);

axE=matrixT[0][0]*axB + matrixT[0][1]*ayB + matrixT[0][2]*azB ;
ayE=matrixT[1][0]*axB + matrixT[1][1]*ayB + matrixT[1][2]*azB ;
azE=matrixT[2][0]*axB + matrixT[2][1]*ayB + matrixT[2][2]*azB ;


//axE=matrix[0][0]*imu->linear_acceleration.x + matrix[0][1]*imu->linear_acceleration.y + matrix[0][2]*imu->linear_acceleration.z ;
//ayE=matrix[1][0]*imu->linear_acceleration.x + matrix[1][1]*imu->linear_acceleration.y + matrix[1][2]*imu->linear_acceleration.z ;
//azE=matrix[2][0]*imu->linear_acceleration.x + matrix[2][1]*imu->linear_acceleration.y + matrix[2][2]*imu->linear_acceleration.z ;
//norm=std::sqrt(axE*axE+ayE*ayE+azE*azE);
//axE=axE/norm;
//ayE=ayE/norm;
//azE=(azE/norm);
azEn=(azE - 9.8);
//vxE+=axE*dt;vyE+=ayE*dt;vzE+=azE*dt;
pos.x+=vxE*dt+0.5*axE*dt*dt;
pos.y+=vyE*dt+0.5*ayE*dt*dt;
pos.z+=vzE*dt+0.5*azEn*dt*dt;

vxE+=axE*dt;vyE+=ayE*dt;vzE+=azEn*dt;

//ROS_INFO("Earth: %f,%f,%f",vxE,vyE,vzE);
//ROS_INFO("Body: %f,%f,%f",imu->linear_acceleration.x,imu->linear_acceleration.y,imu->linear_acceleration.z);
//ROS_INFO("---,%f",norm);
//tm0=tm1;
//ROS_INFO("Published msg %f",);
}



int main(int argc, char **argv){
  ros::init(argc, argv, "imu_integration");
  ros::NodeHandle n; 
  ros::Publisher chatter_pub = n.advertise<nav_msgs::Odometry>("/quad/ground_truth/odometry", 1);
  ros::Rate loop_rate(100);
//  nav_msgs::Odometry msg;
  msg.pose.pose.position.x=0.0;msg.pose.pose.position.y=0.0;msg.pose.pose.position.z=0.0;
  pos.x=0.0;pos.y=0.0;pos.z=0.0;
  pos0.x=0.0;pos0.y=0.0;pos0.z=0.0;
  axE=0.0;ayE=0.0;azE=0.0;azEn=0.0;
  vxE=0.0;vyE=0.0;vzE=0.0;
  dt=0.1;

  ros::Subscriber sub = n.subscribe("mavros/imu/data", 1,callbk); 
  while (ros::ok())
  {
    
    msg.header.frame_id="world";
    msg.child_frame_id="base_link";
//    msg.child_frame_id="map";
    msg.header.stamp=ros::Time::now();
//    msg.header.stamp.nsecs=ros::Time::now();
    msg.pose.pose.position.x = pos.x;
    msg.pose.pose.position.y = pos.y;
    msg.pose.pose.position.z = pos.z;
       // ROS_INFO("Published msg %f, %f,%f", msg.x,msg.y,msg.z);
    chatter_pub.publish(msg); //keep msg in buffer
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}


