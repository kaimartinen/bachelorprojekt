#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "laser_geometry/laser_geometry.h"
#include "laser_line_extraction/line_extraction_ros.h"
#include "laser_line_extraction/line.h"
#include "ellipse/ellipse.h"


/*laser_geometry::LaserProjection projector;
ros::Publisher _publisher;

void LaserCallback(const sensor_msgs::LaserScan::ConstPtr& scan_in) {

  sensor_msgs::PointCloud cloud;
  //ROS_INFO("Angle increment of laser: %lf\n", scan_in->angle_increment);
  projector.projectLaser(*scan_in, cloud);
  _publisher.publish(cloud);

}*/

void extract_ellipses(std::vector<line_extraction::Line> &lines, std::vector<ellipse::Ellipse> &ellipses) {
  ellipses.clear();
  double width = 2.0;
  double height_offset = 0.2;
  for (std::vector<line_extraction::Line>::const_iterator cit = lines.begin();
       cit != lines.end(); ++cit) {
    ellipses.push_back(ellipse::Ellipse(*cit, height_offset + (cit->length() / 2), width));
  }
}


int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_scan_listener");

  /*ros::NodeHandle n;
  ros::NodeHandle m;
  _publisher = m.advertise<sensor_msgs::PointCloud>("Laser_Point_Cloud", 1000);
  ros::Subscriber sub = n.subscribe("base_scan", 1000, LaserCallback);*/

  //ros::spin();

  ros::NodeHandle nh;
  ros::NodeHandle nh_local("~");
  line_extraction::LineExtractionROS line_extractor(nh, nh_local);

  double frequency;
  nh_local.param<double>("frequency", frequency, 25);
  ros::Rate rate(frequency);

  std::vector<line_extraction::Line> lines;
  std::vector<ellipse::Ellipse> ellipses;
  while (ros::ok())
  {
    line_extractor.run();
    line_extractor.extract(lines);
    ROS_INFO("Amount of lines: %d", lines.size());
    extract_ellipses(lines, ellipses);

    ros::spinOnce();
    rate.sleep();

  }

  return 0;
}
