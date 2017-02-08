# AutoNavQuad
Autonomous Navigation of Quadcopter, 3d path planning and execution on Gazebo simulation.
create a catkin workspace and clone below libraries in src directory

mkdir -p ~/quad_demo/src
$ cd ~/quad_ws/src
$ catkin_init_workspace  # initialize your catkin workspace
clone below libraries along with QutoNavQuad
$ git clone https://github.com/catkin/catkin_simple
$ git clone https://github.com/ethz-asl/glog_catkin
$ git clone https://github.com/PX4/mav_comm
$ git clone https://github.com/nishathussain/rotors_simulator

catkin_make

RUN COMMAND:
roslaunch quad_srdf quad_rviz.launch

Run Moveit Setup Assistant:
roslaunch moveit_setup_assistant moveit_setup_assistant.launch
