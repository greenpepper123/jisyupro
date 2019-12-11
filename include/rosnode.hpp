#pragma once
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "device.hpp"

extern Device device;

#define DEG2RAD(x) ((x)*M_PI/180.)

class ROSNode {
public:
    Device device;
    ros::Publisher pub;
    sensor_msgs::LaserScan msg_to_send;

    ROSNode(Device dev) {
        device = dev;
    }

    void ros_init();
    void publish();
};

void cmdvel_cb(const geometry_msgs::Twist::ConstPtr& msg);
