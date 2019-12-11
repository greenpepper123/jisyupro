#include "rosnode.hpp"

void ROSNode::ros_init() {
    int argc = 0;
    ros::init(argc, NULL, "jisyupro");
    ros::NodeHandle n;
    pub = n.advertise<sensor_msgs::LaserScan>("scan", 1000);
    ros::Subscriber sub = n.subscribe<geometry_msgs::Twist>("cmd_vel", 1000, cmdvel_cb);
}

void ROSNode::publish() {
    pub.publish(msg_to_send);
}

void cmdvel_cb(const geometry_msgs::Twist::ConstPtr& msg) {
    device.sendvel(msg->linear.x, msg->linear.y);
}
