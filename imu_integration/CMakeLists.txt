cmake_minimum_required(VERSION 2.8.3)
project(imu_integration)
find_package(catkin REQUIRED)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs geometry_msgs genmsg message_generation)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

catkin_package()

###########
## Build ##
###########
include_directories(include ${catkin_INCLUDE_DIRS} /usr/include/gazebo-6.6/gazebo/math/)

## Declare a C++ library
# add_library(wind
#   src/${PROJECT_NAME}/wind.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(wind ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
add_executable(integ src/integ.cpp)
target_link_libraries(integ ${catkin_LIBRARIES})

add_executable(nav_integ src/nav_integ.cpp)
target_link_libraries(nav_integ ${catkin_LIBRARIES})

add_executable(FileReader src/FileReader.cpp)
target_link_libraries(FileReader ${catkin_LIBRARIES})

add_executable(IMU_data src/IMU_data.cpp)
target_link_libraries(IMU_data ${catkin_LIBRARIES})

add_executable(odom_remap src/odom_remap.cpp)
target_link_libraries(odom_remap ${catkin_LIBRARIES})

add_executable(odom_remap_anil src/odom_remap_anil.cpp)
target_link_libraries(odom_remap_anil ${catkin_LIBRARIES})


#add_dependencies(wind ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_executable(windPub src/windPub.cpp)
#target_link_libraries(windPub ${catkin_LIBRARIES})

#add_dependencies(windPub ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(wind_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS wind wind_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )


