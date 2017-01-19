#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "laser_geometry/laser_geometry.h"


laser_geometry::LaserProjection projector;
ros::Publisher _publisher;

void LaserCallback(const sensor_msgs::LaserScan::ConstPtr& scan_in) {

  sensor_msgs::PointCloud cloud;
  //ROS_INFO("Angle increment of laser: %lf\n", scan_in->angle_increment);
  projector.projectLaser(*scan_in, cloud);
  _publisher.publish(cloud);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_scan_listener");

  ros::NodeHandle n;
  ros::NodeHandle m;
  _publisher = n.advertise<sensor_msgs::PointCloud>("Laser_Point_Cloud", 1000);
  ros::Subscriber sub = n.subscribe("base_scan", 1000, LaserCallback);

  ros::spin();

  return 0;
}
