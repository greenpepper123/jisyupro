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
#include <cmath>

std::vector<std::array<double, 2>> points = {};
Device device;

int main() {
    // initialize
    int len,i,distance,pos;
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
        node.publish();
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
        pos = device.buf[len-3];
        points.push_back({distance/300.0*sin(2.0*pos*M_PI/45.0), distance/300.0*cos(2.0*pos*M_PI/45.0)});
        std::cout<<points.size()<<" "<<pos<<" "<<distance<<std::endl;
        device.sendvel(0.5, -0.5);
    }

    return 0;
}
