#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
//Add the required header -----------------
#include "turtlesim/Kill.h"

ros::Publisher pub;


void turtleCallback(const turtlesim::Pose::ConstPtr& msg)
{
	ROS_INFO("Turtle subscriber@[%f, %f, %f]",  
	msg->x, msg->y, msg->theta);
	geometry_msgs::Twist my_vel;
	
	//modify the code for implementing the required behaviour ----------------------
	ros::Rate loop_rate(100);
	
		while (ros::ok())
		{
			if (msg->x < 9.0 && msg->x >= 2.0)
			{
				my_vel.linear.x = 1.0;
			}
			else
			{
				
				if (msg->x >= 9.0)
				{
					my_vel.linear.x = 1.0;
					my_vel.angular.z = 1.0;
				}
				else
				{
					my_vel.linear.x = 1.0;
					my_vel.angular.z = -1.0;
				}
				
				
				
			}
	
	
			//my_vel.angular.z = 1.0;
			pub.publish(my_vel);
			ros::spinOnce();
			loop_rate.sleep();

		}
	
	

}



int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system  
	ros::init(argc, argv, "turtlebot_subscriber_ex1");  
	ros::NodeHandle nh;
	// Define the subscriber to turtle's position  
	pub = nh.advertise<geometry_msgs::Twist>("rpr_turtle/cmd_vel",1);
	ros::Subscriber sub = nh.subscribe("rpr_turtle/pose", 1,turtleCallback);
	ros::ServiceClient client1 =  nh.serviceClient<turtlesim::Spawn>("/spawn");
	//add the required client
	ros::ServiceClient killClient = nh.serviceClient<turtlesim::Kill>("/kill");
	
	//call the service for "killing" turtle1
	turtlesim::Kill killService;
	killService.request.name = "turtle1";
	
	if (killClient.call(killService)) {
        	ROS_INFO("Turtle eliminata con successo.");
    	} else {
        	ROS_ERROR("Impossibile eliminare la turtle.");
    	}
	
	//modify the starting position of "my_turtle"
	turtlesim::Spawn srv1;
	srv1.request.x = 2.0;  
	srv1.request.y = 1.0;
	srv1.request.theta = 0.0;
	srv1.request.name = "rpr_turtle";
	client1.call(srv1);
	
 
	ros::spin();
	return 0;
}

