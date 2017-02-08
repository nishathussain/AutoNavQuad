#include "ros/ros.h"
#include "geometry_msgs/Vector3.h"
#include "sensor_msgs/Imu.h"
#include "std_msgs/Header.h"
#include <sstream>
#include "gazebo_msgs/ModelState.h"
#include <tf/transform_broadcaster.h>

geometry_msgs::Vector3 pos, pos0;
gazebo_msgs::ModelState msg;
double axE,ayE,azE,azEn,axB,ayB,azB;
double vxE,vyE,vzE,r,p,y,ro,pi,ya;
double norm=0.0,normB=0.0;
double dt,q0,q1,q2,q3;
double tm1=0,tm0=0;
double matrix[3][3], matrixT[3][3];
double PI=3.14159265;
void callbk(const sensor_msgs::Imu::ConstPtr& imu)
{
q0=imu->orientation.x;
q1=imu->orientation.y;
q2=imu->orientation.z;
q3=imu->orientation.w;
//if(tm0==0){
//  tm0=imu->header.stamp.sec*pow(10,9)+imu->header.stamp.nsec;
//  return;
//  }else{
//  tm1=imu->header.stamp.sec*pow(10,9)+imu->header.stamp.nsec;
//  dt=((tm1-tm0)/pow(10,9)>0)?(tm1-tm0)/pow(10,9):dt;
//  std::cout<<dt<<" : "<<tm0<<" : "<<tm1<<" : "<<std::endl;
//  tm0=tm1;
//  }
dt=0.1;
ro=imu->angular_velocity.x;
pi=imu->angular_velocity.y;
ya=imu->angular_velocity.z;
ro=ro * PI / 180.0;
pi=pi * PI / 180.0;
ya=ya * PI / 180.0;

y= ro ; //ro
p= pi ; //pi
r= ya ; //ya

matrix[0][0]=cos(p)*cos(r);                        matrix[0][1]=cos(p)*sin(r);                       matrix[0][2]=-sin(p);
matrix[1][0]=-cos(y)*sin(r)+sin(y)*sin(p)*cos(r);  matrix[1][1]=cos(y)*cos(r)+sin(y)*sin(p)*sin(r);  matrix[1][2]=sin(y)*cos(p);
matrix[2][0]=sin(y)*sin(r)+cos(y)*sin(p)*cos(r);   matrix[2][1]=-sin(y)*cos(r)+cos(y)*sin(p)*sin(r); matrix[2][2]=cos(y)*cos(p);

matrixT[0][0]=matrix[0][0] ; matrixT[0][1]=matrix[1][0] ; matrixT[0][2]=matrix[2][0];
matrixT[1][0]=matrix[0][1] ; matrixT[1][1]=matrix[1][1] ; matrixT[1][2]=matrix[2][1];
matrixT[2][0]=matrix[0][2] ; matrixT[2][1]=matrix[1][2] ; matrixT[2][2]=matrix[2][2];

axB=imu->linear_acceleration.x;
ayB=imu->linear_acceleration.y;
azB=imu->linear_acceleration.z;

axE=matrixT[0][0]*axB + matrixT[0][1]*ayB + matrixT[0][2]*azB ;
ayE=matrixT[1][0]*axB + matrixT[1][1]*ayB + matrixT[1][2]*azB ;
azE=matrixT[2][0]*axB + matrixT[2][1]*ayB + matrixT[2][2]*azB ;
azEn=(azE + 9.81);
//vxE+=axE*dt;vyE+=ayE*dt;vzE+=azE*dt;
pos.x+=vxE*dt+0.5*axE*dt*dt;
pos.y+=vyE*dt+0.5*ayE*dt*dt;
pos.z+=vzE*dt+0.5*azEn*dt*dt;

vxE+=axE*dt;vyE+=ayE*dt;vzE+=azEn*dt;
}



int main(int argc, char **argv){
  ros::init(argc, argv, "eve_position");
  ros::NodeHandle n; 
  ros::Publisher chatter_pub = n.advertise<gazebo_msgs::ModelState>("/gazebo/set_model_state", 1);
  ros::Rate loop_rate(10);

  msg.model_name="quadrotor";
  msg.reference_frame="world";
  msg.pose.orientation.x=0.0,msg.pose.orientation.y=0.0,msg.pose.orientation.z=0.0,msg.pose.orientation.w=1.0;
  msg.pose.position.x=1.0,msg.pose.position.y=1.0,msg.pose.position.z=1.0;
  pos.x=0.0;pos.y=0.0;pos.z=0.1;
  pos0.x=0.0;pos0.y=0.0;pos0.z=0.0;
  q0=0.0,q1=0.0,q2=0.0,q3=1.0;
  axE=0.0;ayE=0.0;azE=0.0;azEn=0.0;
  vxE=0.0;vyE=0.0;vzE=0.0;
  dt=0.1;

  ros::Subscriber sub = n.subscribe("IMUdatafile", 1,callbk); 
  while (ros::ok())
  {
    msg.pose.position.x = pos.x;
    msg.pose.position.y = pos.y;
    msg.pose.position.z = pos.z;

    msg.pose.orientation.x=q0;
    msg.pose.orientation.y=q1;
    msg.pose.orientation.z=q2;
    msg.pose.orientation.w=q3;
       // ROS_INFO("Published msg %f, %f,%f", msg.x,msg.y,msg.z);
    chatter_pub.publish(msg); //keep msg in buffer
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
