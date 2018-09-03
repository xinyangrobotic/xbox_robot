#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include<iostream>

using namespace std;

double vlinear, vangular;
int axis_ang,axis_lin,ton1, ton2;


void callback(const sensor_msgs::Joy::ConstPtr& Joy, ros::Publisher& pub)
{
    geometry_msgs::Twist v;
    if(Joy->buttons[ton1] && Joy->buttons[ton2] )
    {
        float score = 0.15;
        float l_x = Joy->axes[axis_lin]+Joy->axes[1];
        float a_z = Joy->axes[axis_ang]+Joy->axes[0];
        if((Joy->axes[axis_lin] < score)
        && (Joy->axes[axis_ang] < score)
        && (Joy->axes[1] < score)
        && (Joy->axes[0] < score)){
            v.linear.x = 0;
            v.angular.z = 0;
            ROS_INFO("linear:%.3lf   angular:%.3lf", v.linear.x, v.angular.z);
            pub.publish(v);
        } else{
            v.linear.x = l_x * vlinear;
            v.angular.z = a_z * vangular;
            ROS_INFO("linear:%.3lf   angular:%.3lf", v.linear.x, v.angular.z);
            pub.publish(v);
        }
    } else{
        v.linear.x = 0;
        v.angular.z = 0;

    }

}


int main(int argc,char** argv) {
    ros::init(argc, argv, "log");
    ros::NodeHandle n;
    ros::Subscriber sub;
    ros::Publisher pub;
    n.param<int>("axis_linear",axis_lin,4);
    n.param<int>("axis_angular",axis_ang,3);
    n.param<double>("vel_linear",vlinear,1);
    n.param<double>("vel_angular",vangular,1);
    n.param<int>("button1",ton1,4);
    n.param<int>("button2",ton2,5);
    geometry_msgs::Twist v;
    pub = n.advertise<geometry_msgs::Twist>("cmd_vel",1);
    sub = n.subscribe<sensor_msgs::Joy>("joy",10, boost::bind(&callback, _1, pub));
    ros::spin();

    return 0;
}