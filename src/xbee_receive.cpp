#include "ros/ros.h"
#include "std_msgs/String.h"
 #include <geometry_msgs/PoseStamped.h> 
#include <sstream>
#include "sys/time.h"
#include <math.h>
#include <stdio.h>
#include "ftditools.h"


double getTime()
{
    struct timeval t;
    gettimeofday(&t,NULL);
    double rt=t.tv_usec;
    rt=rt/1000000+t.tv_sec;
    return rt;
}


int main(int argc, char **argv) {
	ros::init(argc, argv, "xbee receiver");
	ros::NodeHandle n;
	ros::Publisher pub = n.advertise<geometry_msgs::PoseStamped>("/mocap/pose", 10);
	ros::Rate loop_rate(50);
	short data_out[11];
	int secs[2];
	int data_new_flag=0;
	geometry_msgs::PoseStamped msg;
	double ltime,ntime;
	ltime=getTime();
	ntime=ltime;
	/*
	msg.layout.dim[0].size=2;
	msg.layout.dim[0].stride=2;
	msg.layout.dim[0].label="Time";
	msg.layout.dim[1].size=1;
	msg.layout.dim[2].size=1;
	*/
	openFtdi = open_ftdi(57600, "Aero0", 5, 15);
	ROS_INFO("Open Ftdi: [%d]", openFtdi);
	while (ros::ok()) {
		ntime=getTime();
		ltime=ntime;
		//msg.data[1]=getTime();
		//ROS_INFO("Show Data: [%s]", msg->data.c_str());
/*
		
		for(int i=0;i<8;i++)
		{
			msg.data[i]=data_out[i]/512.0f;
		}*/
		if(openFtdi<1)
		{
			openFtdi = open_ftdi(57600, "Quad_USB1", 5, 15);
		}
		data_new_flag = read_ftdi (data_out);
		memcpy(secs,&data_out[7],sizeof(secs));
		msg.pose.position.x=data_out[0]/1000.0f;
		msg.pose.position.y=data_out[1]/1000.0f;
		msg.pose.position.z=data_out[2]/1000.0f;
		msg.pose.orientation.x=data_out[3]/1000.0f;
		msg.pose.orientation.y=data_out[4]/1000.0f;
		msg.pose.orientation.z=data_out[5]/1000.0f;
		msg.pose.orientation.w=data_out[6]/1000.0f;
		msg.header.stamp.sec=secs[0];
		msg.header.stamp.nsec=secs[1];
		pub.publish(msg);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
