#include "rosnode.hpp"

void ROSNode::ros_init() {
    int argc = 0;
    msg_to_send.header.frame_id = "base_link";
    msg_to_send.angle_min = M_PI - 7*step_angle;
    msg_to_send.angle_max = M_PI + 7*step_angle;
    msg_to_send.angle_increment = step_angle;
    msg_to_send.scan_time = 0.6;
    msg_to_send.time_increment = 0.04;
    msg_to_send.range_min = 0.04;
    msg_to_send.range_max = 1.3;
    msg_to_send.intensities = {};

    ros::init(argc, NULL, "jisyupro");
    ros::NodeHandle n;
    pub = n.advertise<sensor_msgs::LaserScan>("scan", 1000);
    sub = n.subscribe<geometry_msgs::Twist>("cmd_vel", 1000, cmdvel_cb);
}

void ROSNode::publish(std::vector<float> ranges) {
    msg_to_send.header.stamp = first_point_stamp;

    msg_to_send.ranges = ranges;
    pub.publish(msg_to_send);
}

void cmdvel_cb(const geometry_msgs::Twist::ConstPtr& msg) {
    // (linear.x[m/s]±chasis_radius[m]*angular.z[rad/s])/wheel_radius[m]/omega_max
    device.sendvel(-(msg->linear.x+chasis_radius*msg->angular.z)/wheel_radius/max_omega,
                   (msg->linear.x-chasis_radius*msg->angular.z)/wheel_radius/max_omega);
    static tf::TransformBroadcaster br;
    t_now = ros::Time::now();
    ros::Duration dt = t_now - t_pre;
    pos_x += v_linear * cosf64(rot_z) * dt.toSec();
    pos_y += v_linear * sinf64(rot_z) * dt.toSec();
    rot_z += v_angular * dt.toSec();
    tf::Transform transform;
    transform.setOrigin(tf::Vector3(pos_x, pos_y, 0.0));
    tf::Quaternion q;
    q.setRPY(0, 0, rot_z);
    transform.setRotation(q);
    br.sendTransform(tf::StampedTransform(transform, t_now, "world", "base_footprint"));
    transform.setOrigin(tf::Vector3(0.08, 0.0, 0.083));
    transform.setRotation(tf::Quaternion(0.0, 0.0, 0.0, 1.0));
    br.sendTransform(tf::StampedTransform(transform, t_now, "base_footprint", "base_scan"));
    // Store previous values
    v_linear = msg->linear.x;
    v_angular = msg->angular.z;
    t_pre = t_now;
}
