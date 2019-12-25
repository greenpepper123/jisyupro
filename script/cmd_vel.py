#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist

class track_box_to_cmd_vel:
    pub = None
    def __init__(self):
        rospy.init_node('client')
        self.pub = rospy.Publisher('/cmd_vel', Twist)
    def loop(self):
        rate = rospy.Rate(10)
        while not rospy.is_shutdown():
            cmd_vel = Twist()
            cmd_vel.linear.x = 0.1
            self.pub.publish(cmd_vel)
            rate.sleep()        

obj = track_box_to_cmd_vel()
obj.loop()
