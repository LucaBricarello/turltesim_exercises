#!/usr/bin/env python3


import rospy
from geometry_msgs.msg import Twist
from turtlesim.msg import Pose
from turtlesim.srv import Spawn
import math


def turtle_callback(msg):
    global prev_x, prev_y, prev_ang_z, prev_time, estimated_velx, estimated_vely, estimated_ang_velz

    rospy.loginfo("Turtle subscriber@[%f, %f, %f]", msg.x, msg.y, msg.theta)

    current_time = rospy.Time.now()
    
    # Calculate estimated velocities only if prev_time is set
    if prev_time.to_sec() > 0:
        delta_time = (current_time - prev_time).to_sec()
        estimated_velx = (msg.x - prev_x) / delta_time
        estimated_vely = (msg.y - prev_y) / delta_time
        
        estimated_velx = math.sqrt(estimated_velx * estimated_velx + estimated_vely * estimated_vely)
        
        estimated_ang_velz = (msg.theta - prev_ang_z) / delta_time
    else:
        estimated_velx = estimated_vely = 0

    # Update previous position and time
    prev_x, prev_y, prev_ang_z, prev_time = msg.x, msg.y, msg.theta, current_time


def main():
    global pub, prev_time
    
    # Initialize the node
    rospy.init_node('turtlebot_subscriber_ex3', anonymous=True)

    # Define the publisher and subscriber
    pub = rospy.Publisher('turtle2/cmd_vel', Twist, queue_size=1)
    rospy.Subscriber('turtle1/pose', Pose, turtle_callback)
    
    # Define the service client and call it to spawn turtle2
    client1 = rospy.ServiceProxy('/spawn', Spawn)
    client1(4.0, 4.0, 0.0, "turtle2")
    
    rate = rospy.Rate(100)  # 100 Hz
    my_vel = Twist()
    
    while not rospy.is_shutdown():
        # Set the estimated velocities
        my_vel.linear.x = estimated_velx
        my_vel.linear.y = 0;
        my_vel.angular.z = estimated_ang_velz  # Set to 0 if no angular velocity is calculated

        # Publish the velocity message
        pub.publish(my_vel)
        rate.sleep()


# Global variables
prev_x = 0
prev_y = 0
prev_ang_z = 0
prev_time = rospy.Time(0)
estimated_velx = 0
estimated_vely = 0
estimated_ang_velz = 0;

if __name__ == '__main__':
    main()

