
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Imu.h"
#include "tf/tf.h"

#include <sstream>

//typedef actionlib::SimpleActionClient<control_msgs::vigir_humanoid_control_msgs::ChangeControlModeGoalConstPtr> ChangeControlModeActionClient;

double rollThreshold = 0.8;
double pitchThreshold = 0.7;

void imuCallback(const sensor_msgs::Imu::ConstPtr msg)
{
    double roll, pitch, yaw;

    tf::Quaternion orientation;
    tf::quaternionMsgToTF(msg->orientation, orientation);

    tf::Matrix3x3(orientation).getRPY(roll, pitch, yaw);
    ROS_INFO("Roll: %f Pitch: %f ", roll, pitch);

    if (fabs(roll) > rollThreshold || fabs(pitch) > pitchThreshold){
         ROS_WARN("FALLING!!!!");
    }

}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

  ros::init(argc, argv, "thor_mang_fall_detection");

  ros::NodeHandle pn("~");

  pn.getParam("rollThreshold", rollThreshold);
  pn.getParam("pitchThreshold", pitchThreshold);

  std::string control_mode_topic = "/mode_controllers/control_mode_controller/change_control_mode";

  ros::Subscriber imu_sub = pn.subscribe<sensor_msgs::Imu>("/thor_mang/pelvis_imu", 1, imuCallback);

  ros::spin();

  ROS_INFO("Node killed");

  return 0;
}
