#pragma once
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "tf/transform_broadcaster.h"
#include "device.hpp"

extern Device device;
extern double v_linear, v_angular, pos_x, pos_y, rot_z;
extern ros::Time t_pre, t_now;
extern tf::Transform transform;

class ROSNode {
public:
    Device device;
    ros::Publisher pub;
    ros::Subscriber sub;
    sensor_msgs::LaserScan msg_to_send;
    ros::Time first_point_stamp;
    bool send_ready = false;
    float step_angle = M_PI/3*0.04/0.25;

    ROSNode(Device dev) {
        device = dev;
    }

    void ros_init();
    void publish(std::vector<float> ranges);
};

void cmdvel_cb(const geometry_msgs::Twist::ConstPtr& msg);
