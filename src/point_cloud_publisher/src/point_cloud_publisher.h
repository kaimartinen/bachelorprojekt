#ifndef POINT_CLOUD_PUBLISHER
#define POINT_CLOUD_PUBLISHER

#include "ros/ros.h"
#include "sensor_msgs/PointCloud.h"

class Point_Cloud_Publisher {
public:
  Point_Cloud_Publisher(ros::NodeHandle &node, std::string node_topic);
  ~Point_Cloud_Publisher();
  void publish(const sensor_msgs::PointCloud cloud);

private:
  ros::NodeHandle _n;
  std::string _topic;
  ros::Publisher _publisher;
};

#endif
