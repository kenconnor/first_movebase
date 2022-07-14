#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Joy.h"

class Joy2Twist{
	private:
		ros::NodeHandle nh_;
		ros::Publisher pub_;
		ros::Subscriber sub_;
		geometry_msgs::Twist twist;
		float max_speed_x = 0.3;
		float max_speed_z = 2;
	public:
		Joy2Twist();
		void callback(const sensor_msgs::Joy& msg);
};

Joy2Twist::Joy2Twist()
{
	pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 1);
	sub_ = nh_.subscribe("joy", 1, &Joy2Twist::callback, this);
}

void Joy2Twist::callback(const sensor_msgs::Joy& msg) {
	double L_horizontal = msg.axes[0];
	double L_vertical = msg.axes[1];
	//ROS_INFO("horizontal: %lf", L_horizontal);
	//ROS_INFO("vertical: %lf", L_vertical);

	twist.linear.x = L_vertical * max_speed_x;
	twist.angular.z = L_horizontal * max_speed_z;
	pub_.publish(twist);
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "joy_to_twist");
	Joy2Twist j2t;
	ros::Rate loop_rate(10);

	while(ros::ok()) {
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
