#!/usr/bin/env python
import roslib; roslib.load_manifest('thor_mang_utils')
import math
import rospy
from trajectory_msgs.msg import *
from moveit_msgs.srv import *
from copy import deepcopy


if __name__ == "__main__":
  
	rospy.init_node("goto_ready_stance")	
	
	rospy.loginfo('goto_ready_pose waiting for trajectory execution service...')
	rospy.wait_for_service('/execute_kinematic_path')
	rospy.loginfo('goto_ready_pose finished waiting for trajectory execution service.')
        execute_service = rospy.ServiceProxy('/execute_kinematic_path', ExecuteKnownTrajectory)
	  
	names = [ 'l_ankle_pitch', 'l_ankle_roll', 'l_elbow', 'l_hip_pitch', 'l_hip_roll', 'l_hip_yaw', 'l_knee', 'l_shoulder_pitch', 'l_shoulder_roll', 'l_shoulder_yaw', 'l_wrist_roll', 'l_wrist_yaw1', 'l_wrist_yaw2', 'r_ankle_pitch', 'r_ankle_roll', 'r_elbow', 'r_hip_pitch', 'r_hip_roll', 'r_hip_yaw', 'r_knee', 'r_shoulder_pitch', 'r_shoulder_roll', 'r_shoulder_yaw', 'r_wrist_roll', 'r_wrist_yaw1', 'r_wrist_yaw2']
        positions = [ 0.5920838259292829, -0.06228113556334662, -1.5708088413053787, -0.736909995463745, -0.062043325761155385, 1.2516305378486057e-05, 1.1722896780543826, 0.7853856461946216, -0.2811537677169323, 0.0006007826581673308, 0.01563815008, -0.25205140042962965, 0.0, -0.5921088585400399, 0.06164280398904383, 1.5707838086946215, 0.7369350280745021, 0.06183054856972112, -1.2516305378486057e-05, -1.1723397432758964, -0.7853856461946216, 0.4944190950609562, -0.12139564586593626, -0.008977456527407408, 0.007922501958518518, 0.004736952868148149]
	
	req = ExecuteKnownTrajectoryRequest()
	req.trajectory.joint_trajectory.joint_names = names
	
	point = JointTrajectoryPoint()
	point.positions = positions
	point.velocities = [0] * len(positions)
	point.accelerations = [0] * len(positions)
	point.time_from_start = rospy.Duration(4.0)
	
	req.trajectory.joint_trajectory.points.append(point)
	req.trajectory.joint_trajectory.header.stamp = rospy.Time.now()
	
        resp = execute_service(req)
	
	
	#pan_control = JointTrajectoryPanControl()
	
	#pan_control.run()
