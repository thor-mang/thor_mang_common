cmake_minimum_required(VERSION 2.8.3)
project(thor_mang_description)

find_package(catkin REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES thor_mang_description
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

foreach(dir launch  meshes robots rviz urdf)
   install(DIRECTORY ${dir}/ 
      DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/${dir})
endforeach(dir)

