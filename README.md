# AutoNavQuad
<br>Autonomous Navigation of Quadcopter, 3d path planning and execution on Gazebo simulation.<br />
<br>create a catkin workspace and clone below libraries in src directory<br />

<br>mkdir -p ~/quad_demo/src<br />
<br>$ cd ~/quad_ws/src<br />
<br>$ catkin_init_workspace  # initialize your catkin workspace<br />
<br>clone below libraries along with QutoNavQuad<br />
<br>$ git clone https://github.com/catkin/catkin_simple<br />
<br>$ git clone https://github.com/ethz-asl/glog_catkin<br />
<br>$ git clone https://github.com/PX4/mav_comm<br />
<br>$ git clone https://github.com/nishathussain/rotors_simulator<br />

<br>catkin_make<br />

<br>RUN COMMAND:<br />
<br>roslaunch quad_srdf quad_rviz.launch<br />

<br>Run Moveit Setup Assistant:<br />
<br>roslaunch moveit_setup_assistant moveit_setup_assistant.launch<br />
<br>will upload further details<br />

![alt tag](https://github.com/nishathussain/AutoNavQuad/blob/master/pres_data_full.png)
<br>Autonomous Navigation, Pathplanning in complex indoor environment, Mavros communication to UAV.<br />

<br>Currently working on stereo and path planning using deep neural net.<br />
<br>contact: hussain.nishat@gmail.com<br />
