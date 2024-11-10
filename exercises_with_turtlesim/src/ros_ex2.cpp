#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"

//Add the required header -----------------
#include<unistd.h>


ros::Publisher pub;
int flag = 0;

int cnt = 0;


void turtleCallback(const turtlesim::Pose::ConstPtr& msg)
	{
	ROS_INFO("%f Turtle subscriber@[%f, %f, %f]",  
	abs(msg->x - 5.5), msg->x, msg->y, msg->theta);
	geometry_msgs::Twist my_vel;
	
	//modify the code for implementing the required behaviour ----------------------
	ros::Rate loop_rate(100);
	
	while (ros::ok())
	{
		
			
		if (msg->x < 10 && msg->y < 10 && msg->x > 1 && msg->y > 1)
		{
			if (flag == 0)
			{
				my_vel.linear.x = 1.0;
				my_vel.angular.z = 0;
			}
			else if (flag == 1)
			{
				my_vel.linear.x = -1.0;
				my_vel.angular.z = 0;
			}
			if (abs(msg->x - 5.5) < 0.1 && abs(msg->y - 5.5) < 0.1 && cnt > 400 && flag == 1)
			{
				my_vel.linear.x = 0.0;
				my_vel.angular.z = 1.57;
				flag = 2;
			}
		}
					
		else
		{
			flag = 1;
			my_vel.linear.x = -1.0;		
			my_vel.angular.z = 0;
		}
				
		cnt++;

		pub.publish(my_vel);
		if (flag == 2)
		{
			sleep(1);
			flag = 0;
		}
		ros::spinOnce();
		loop_rate.sleep();

	}
	
	

}



int main (int argc, char **argv)
{
	// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system  
	ros::init(argc, argv, "turtlebot_subscriber_ex2");  
	ros::NodeHandle nh;
	// Define the subscriber to turtle's position  
	pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1);
	ros::Subscriber sub = nh.subscribe("turtle1/pose", 1,turtleCallback);
 
	ros::spin();
	return 0;
}

