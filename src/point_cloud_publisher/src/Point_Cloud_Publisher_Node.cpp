#include <string>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "Point_Cloud_Publisher_Node.h"

Point_Cloud_Publisher::Point_Cloud_Publisher(ros::NodeHandle node, std::string node_topic) {
  _n = node;
  _topic = node_topic;
  _publisher = _n.advertise<sensor_msgs::PointCloud>("Laser_Point_Cloud", 1000);
}

Point_Cloud_Publisher::~Point_Cloud_Publisher() {

}

void Point_Cloud_Publisher::publish(sensor_msgs::PointCloud cloud) {
  _publisher.publish(cloud);
  ros::spinOnce();
}

int main(int argc, char **argv  ) {
  /* code */
  return 0;
}
