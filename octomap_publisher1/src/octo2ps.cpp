#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

#include <moveit/planning_scene/planning_scene.h>
#include <moveit/kinematic_constraints/utils.h>
#include <eigen_conversions/eigen_msg.h>

#include <octomap/octomap.h>
#include <octomap_msgs/conversions.h>

#include <octomap/OcTree.h>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "octomap_publisher");

  ros::NodeHandle n;

  //publisher for the planning scene
  ros::Publisher octomap_pub = n.advertise<moveit_msgs::PlanningScene>("planning_scene", 1);
  ros::Rate loop_rate(1);

  int count = 0;
  while (ros::ok())
  {
        static octomap_msgs::Octomap octomap;
        static bool msgGenerated = false;

        //Turn the octomap .bt file into an octree format, which is needed by BinaryMapToMsg

        if ( msgGenerated == false)
        {
              octomap::OcTree myOctomap("/home/nishat/nishat/quad_demo/test_map_octo1.bt");
              octomap_msgs::binaryMapToMsg(myOctomap, octomap);
              msgGenerated = true;
        }

      moveit_msgs::PlanningScene planning_scene;
      planning_scene.name = "(noname)+";
      planning_scene.is_diff = true;


//      planning_scene.world.octomap.header.frame_id = "world";
      planning_scene.world.octomap.octomap.header = octomap.header;
      planning_scene.world.octomap.header.frame_id = "/map";//''octomap.header.frame_id'';
      std::cout<<octomap.header.frame_id<<std::endl;
      planning_scene.world.octomap.octomap.binary = true;
      planning_scene.world.octomap.octomap.id = octomap.id;
      planning_scene.world.octomap.octomap.data = octomap.data;
      planning_scene.world.octomap.octomap.resolution = octomap.resolution;

      ROS_INFO("Adding the octomap into the world.");
      octomap_pub.publish(planning_scene);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  } 
  return 0;
}
