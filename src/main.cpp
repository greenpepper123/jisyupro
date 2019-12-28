#include "device.hpp"

#ifdef ENABLE_VIZ
#include "visualizer.hpp"
#include <pthread.h>
#endif

#ifdef ENABLE_ROS
#include "rosnode.hpp"
#endif

#include <iostream>
#include <cstdlib>

std::vector<std::array<float, 2>> points = {};
Device device;
double v_linear = 0.0, v_angular = 0.0, pos_x = 0.0, pos_y = 0.0, rot_z = 0.0;
ros::Time t_pre, t_now;
tf::Transform transform;

int main() {
    // initialize
    int len,i,distance,pos=0,pos_new;
    std::vector<float> ranges_to_send, distances;
    bool send_ready = false, counting_up = false, valid = true;
    device.init();

#ifdef ENABLE_VIZ
    pthread_t thread;
    pthread_create(&thread, NULL, gl_main, NULL);
    pthread_detach(thread);
#endif

#ifdef ENABLE_ROS
    ROSNode node(device);
    node.ros_init();
    tf::Transform tf_scan, tf_id;
    tf::Quaternion qq;
    tf_id.setOrigin(tf::Vector3(0, 0, 0));
    qq.setRPY(0, 0, 0);
    tf_id.setRotation(qq);
    transform.setOrigin(tf::Vector3(0, 0, 0));
    transform.setRotation(qq);
    tf_scan.setOrigin(tf::Vector3(0.08, 0.0, 0.083));
    qq.setRPY(0, 0, M_PI);
    tf_scan.setRotation(qq);
    while (ros::ok()) {
        if (send_ready) {
            if (counting_up) {
                for (int i = 0; i < 15; i++) {
                    ranges_to_send.push_back(distances[i] / 1000.0);
                }
            } else {
                for (int i = 14; i >= 0; i--) {
                    ranges_to_send.push_back(distances[i] / 1000.0);
                }
            }
            node.publish(ranges_to_send);
            points.clear();
            distances.clear();
            ranges_to_send.clear();
            send_ready = false;
            node.first_point_stamp = ros::Time::now();
        }
        ros::spinOnce();
#else
    while (true) {
#endif
        len = device.read();
        if(len < 3) {
            continue;
        }
        distance = 0;
        for(i = len; i > 3; i--) {
            distance+=((device.buf[len-i]-48)*std::pow(10.0, i-4));
        }
        pos_new = device.buf[len-3];
        if (distance < 5) {
            valid = false;
        }
        if ((not valid) and (pos_new == pos)) {
            points.clear();
            distances.clear();
            valid = true;
        }
        if (valid) {
            if (pos_new == 8 && pos == 7) {
                counting_up = true;
            } else if (pos_new == 7 && pos == 8) {
                counting_up = false;
            }
            distances.push_back(distance);
            points.push_back({distance/300.0f*sinf32(2.0*pos_new*M_PI/45.0), distance/300.0f*cosf32(2.0*pos_new*M_PI/45.0)});
            if (points.size() > 14) {
                if ((pos_new == 14 and pos == 13) or (pos_new == 0 and pos == 1)) {
                    send_ready = true;
                } else {
                    valid = false;
                }
            }
        }
        pos = pos_new;
        std::cout<<points.size()<<" "<<pos<<" "<<distance<<std::endl;

	// Publish TF
    	static tf::TransformBroadcaster br;
    	br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "world", "odom"));
    	br.sendTransform(tf::StampedTransform(tf_id, ros::Time::now(), "odom", "base_footprint"));
    	br.sendTransform(tf::StampedTransform(tf_scan, ros::Time::now(), "base_footprint", "base_scan"));
    }

    return 0;
}
