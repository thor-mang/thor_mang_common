cmake_minimum_required(VERSION 2.8.3)
project(thor_mang_utils)

find_package(catkin REQUIRED COMPONENTS
  moveit_msgs
  rospy
  trajectory_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES thor_mang_utils
#  CATKIN_DEPENDS moveit_msgs rospy trajectory_msgs
#  DEPENDS system_lib
)

#include_directories(
#  ${catkin_INCLUDE_DIRS}
#)

catkin_install_python(PROGRAMS src/goto_ready_stance.py
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})