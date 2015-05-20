
#include "ros/ros.h"
#include <actionlib/client/simple_action_client.h>
#include <vigir_humanoid_control_msgs/ChangeControlModeAction.h>
#include "sensor_msgs/Imu.h"
#include "tf/tf.h"

typedef actionlib::SimpleActionClient<vigir_humanoid_control_msgs::ChangeControlModeAction> ChangeControlModeActionClient;

double rollThresholdPositive = 0.245;
double rollThresholdNegative = -0.325;
double pitchThresholdPositive = 0.3;
double pitchThresholdNegative = -0.15;

ChangeControlModeActionClient *action_client;
bool falling = false;

void modeSwitchDoneCallback(const actionlib::SimpleClientGoalState& state,  const vigir_humanoid_control_msgs::ChangeControlModeResultConstPtr& result){
    if(state == actionlib::SimpleClientGoalState::SUCCEEDED){
        falling = true;
    }else{
        ROS_ERROR("Mode switch failed!");
    }
}

void modeSwitchActiveCallback(){

}

void modeSwitchFeedbackCallback(const vigir_humanoid_control_msgs::ChangeControlModeFeedbackConstPtr &feedback){

}

void onFall(){
    if(!falling){
        ROS_INFO("going into fall");
        if (action_client->isServerConnected()){
            vigir_humanoid_control_msgs::ChangeControlModeGoal goal;
            goal.mode_request="falling";

            //Do something with the goal
            action_client->sendGoal(goal, modeSwitchDoneCallback, modeSwitchActiveCallback, modeSwitchFeedbackCallback);
            action_client->waitForResult(ros::Duration(5));
        }else{
            ROS_ERROR("Want to switch to falling but server is not connected: We got a big problem!");
        }
    }
}

void imuCallback(const sensor_msgs::Imu::ConstPtr msg)
{
    double roll, pitch, yaw;

    tf::Quaternion orientation;
    tf::quaternionMsgToTF(msg->orientation, orientation);

    tf::Matrix3x3(orientation).getRPY(roll, pitch, yaw);
    ROS_INFO("Roll: %f Pitch: %f ", roll, pitch);

    if (roll > rollThresholdPositive || roll < rollThresholdNegative
            || pitch < pitchThresholdNegative || pitch > pitchThresholdPositive){
        onFall();
    }

}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

    ros::init(argc, argv, "thor_mang_fall_detection");

    ros::NodeHandle pn("~");

    pn.param("rollThresholdPositive", rollThresholdPositive, 0.245);
    pn.param("rollThresholdNegative", rollThresholdNegative, -0.325);
    pn.param("pitchThresholdPositive", pitchThresholdPositive, 0.3);
    pn.param("pitchThresholdNegative", pitchThresholdNegative, -0.15);

    std::string control_mode_topic = "/mode_controllers/control_mode_controller/change_control_mode";

    action_client = new ChangeControlModeActionClient(control_mode_topic, true);

    ROS_INFO("Waiting for Action Server to come up");
    if (!action_client->waitForServer(ros::Duration(5.0))){
        ROS_ERROR("Action Client timed out while waiting for server!");
        return -1;
    }
    ROS_INFO("Connection to Action Server completed");

    ros::Subscriber imu_sub = pn.subscribe<sensor_msgs::Imu>("/thor_mang/pelvis_imu", 1, imuCallback);

    ros::spin();

    ROS_INFO("Node killed");

    delete action_client;

    return 0;
}
