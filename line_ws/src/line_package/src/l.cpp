#include<ros/ros.h>
#include<visualization_msgs/Marker.h>
#include<cmath>

int main(int argc,char **argv)
{
	ros::init(argc,argv,"point_line");
	ros::NodeHandle n;
	ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker",10);
	ros::Rate r(30);
	float f = 0.0;
	
	while(ros::ok())
	{
		visualization_msgs::Marker line_list;
		line_list.header.frame_id = "/my_frame";
		line_list.header.stamp = ros::Time::now();
		line_list.action = visualization_msgs::Marker::ADD;
		line_list.pose.orientation.w = 1.0;
		line_list.ns = "points_and_lines";
			
		line_list.id = 1;
		line_list.type = visualization_msgs::Marker::LINE_LIST;
		line_list.scale.x = 0.1;

		line_list.color.g = 1.0;
		line_list.color.a = 1.0;

		for(uint32_t i=0; i<100 ; i++)
		{
		
			
			geometry_msgs::Point p;
			p.x = (int32_t)i - 50;;
			p.y = 0.0;
			p.z = 0.0;
			line_list.points.push_back(p);
		}

		marker_pub.publish(line_list);
		r.sleep();
		
		


	}

}
