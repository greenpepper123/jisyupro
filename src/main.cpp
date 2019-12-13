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

int main() {
    // initialize
    int len,i,distance,pos=0,pos_new;
    std::vector<float> ranges_to_send;
    bool send_ready = false, counting_up = false;
    device.init();

#ifdef ENABLE_VIZ
    pthread_t thread;
    pthread_create(&thread, NULL, gl_main, NULL);
    pthread_detach(thread);
#endif

#ifdef ENABLE_ROS
    ROSNode node(device);
    node.ros_init();
    while (ros::ok()) {
        if (send_ready) {
            if (counting_up) {
                for (int i = 0; i < 15; i++) {
                    ranges_to_send.push_back(points[i][0] / 1000.0);
                }
            } else {
                for (int i = 14; i >= 0; i--) {
                    ranges_to_send.push_back(points[i][0] / 1000.0);
                }
            }
            node.publish(ranges_to_send);
            points.clear();
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
        if (pos_new == 8 && pos == 7) {
            counting_up = true;
        } else if (pos_new == 7 && pos == 8) {
            counting_up = false;
        }
        pos = pos_new;
        points.push_back({distance/300.0f*sinf32(2.0*pos*M_PI/45.0), distance/300.0f*cosf32(2.0*pos*M_PI/45.0)});
        if (points.size() > 14) { send_ready = true; }
        //std::cout<<points.size()<<" "<<pos<<" "<<distance<<std::endl;
    }

    return 0;
}
