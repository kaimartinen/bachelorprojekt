#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "laser_geometry/laser_geometry.h"
#include "point_cloud_publisher/point_cloud_publisher.h"


laser_geometry::LaserProjection projector;
ros::NodeHandle m;
Point_Cloud_Publisher* publisher = new Point_Cloud_Publisher(&m, "Laser_Point_Cloud");

void LaserCallback(const sensor_msgs::LaserScan::ConstPtr& scan_in) {

  sensor_msgs::PointCloud cloud;
  projector.projectLaser(*scan_in, cloud);
  publisher->publish(cloud);
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_scan_listener");

  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("base_laser_link", 1000, LaserCallback);

  ros::spin();

  return 0;
}
