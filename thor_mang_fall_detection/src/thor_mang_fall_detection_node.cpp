
#include "ros/ros.h"
#include <actionlib/client/simple_action_client.h>
#include <vigir_humanoid_control_msgs/ChangeControlModeAction.h>
#include "sensor_msgs/Imu.h"
#include "std_msgs/Bool.h"
#include "tf/tf.h"
#include <actionlib/server/simple_action_server.h>
#include "thor_mang_fall_detection/FallDetectionControlAction.h"

typedef actionlib::SimpleActionClient<vigir_humanoid_control_msgs::ChangeControlModeAction> ChangeControlModeActionClient;
typedef actionlib::SimpleActionServer<thor_mang_fall_detection::FallDetectionControlAction> FallDetectionControlActionServer;

double rollThresholdPositive = 0.245;
double rollThresholdNegative = -0.325;
double pitchThresholdPositive = 0.3;
double pitchThresholdNegative = -0.15;

ChangeControlModeActionClient *action_client;
bool falling = false;

bool running = true;

ros::Publisher status_pub;

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
    if(!running || falling){
        return;
    }

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

void start(){
    running = true;
    falling = false;
}

void stop(){
    running = false;
    falling = false;
}

void executeFallDetectionControlAction(const thor_mang_fall_detection::FallDetectionControlGoalConstPtr& goal, FallDetectionControlActionServer* as)
{
    if(goal->run && ! running){
        start();
    }else if(!goal->run && running){
        stop();
    }else{
        //ignore
    }

    as->setSucceeded();
}

void timerCallback(const ros::TimerEvent&){

    std_msgs::Bool msg;

    msg.data = falling;

    status_pub.publish(msg);

}

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{

    ros::init(argc, argv, "thor_mang_fall_detection");

    ros::NodeHandle nh("~");

    nh.param("rollThresholdPositive", rollThresholdPositive, 0.245);
    nh.param("rollThresholdNegative", rollThresholdNegative, -0.325);
    nh.param("pitchThresholdPositive", pitchThresholdPositive, 0.3);
    nh.param("pitchThresholdNegative", pitchThresholdNegative, -0.15);

    FallDetectionControlActionServer server(nh, "/thor_mang_fall_detection/command", boost::bind(&executeFallDetectionControlAction, _1, &server), false);
    server.start();

    std::string control_mode_topic = "/mode_controllers/control_mode_controller/change_control_mode";

    action_client = new ChangeControlModeActionClient(control_mode_topic, true);

    ROS_INFO("Waiting for Action Server to come up");
    if (!action_client->waitForServer(ros::Duration(5.0))){
        ROS_ERROR("Action Client timed out while waiting for server!");
        return -1;
    }
    ROS_INFO("Connection to Action Server completed");

    ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/thor_mang/pelvis_imu", 1, imuCallback);

    status_pub = nh.advertise<std_msgs::Bool>("falling", 1000);

    ros::Timer timer = nh.createTimer(ros::Duration(0.1), timerCallback);

    ros::spin();

    ROS_INFO("Node killed");

    delete action_client;

    return 0;
}
