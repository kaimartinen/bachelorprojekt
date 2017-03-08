#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/PointCloud.h"
#include "laser_geometry/laser_geometry.h"
#include "laser_line_extraction/line_extraction_ros.h"
#include "laser_line_extraction/line.h"
#include "ellipse/ellipse.h"
#include <visualization_msgs/Marker.h>


void extract_ellipses(std::vector<line_extraction::Line> &lines, std::vector<ellipse::Ellipse> &ellipses) {
  ellipses.clear();
  double width = 2.0;
  for (std::vector<line_extraction::Line>::const_iterator cit = lines.begin();
       cit != lines.end(); ++cit) {
    ellipses.push_back(ellipse::Ellipse(*cit, (cit->length() / 2), width));
  }
}

void populateMarkerMsg(std::vector<ellipse::Ellipse> &ellipses, visualization_msgs::Marker &marker_msg)
{
  marker_msg.ns = "ellipse_extraction";
  marker_msg.id = 0;
  marker_msg.type = visualization_msgs::Marker::CYLINDER;
  marker_msg.color.r = 0.0;
  marker_msg.color.g = 0.0;
  marker_msg.color.b = 1.0;
  marker_msg.color.a = 1.0;
  for (int i = 0; i < ellipses.size(); i++)
  {
    geometry_msgs::Point p;
    p.x = ellipses[i].getPPoint()[0];
    p.y = ellipses[i].getPPoint()[1];
    p.z = 0;
    marker_msg.points.push_back(p);
    marker_msg.scale.x = ellipses[i].getWidth();
    marker_msg.scale.y = ellipses[i].getHeight();
  }
  marker_msg.header.frame_id = "base_link";
  marker_msg.header.stamp = ros::Time::now();
}


int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_scan_listener");

  ros::NodeHandle nh;
  ros::NodeHandle nh_local("~");
  line_extraction::LineExtractionROS line_extractor(nh, nh_local);

  double frequency;
  nh_local.param<double>("frequency", frequency, 25);
  ros::Rate rate(frequency);
  ros::Publisher ellipse_marker = nh.advertise<visualization_msgs::Marker>("ellipse_marker", 1);

  std::vector<line_extraction::Line> lines;
  std::vector<ellipse::Ellipse> ellipses;
  while (ros::ok())
  {
    line_extractor.run();
    line_extractor.extract(lines);
    ROS_INFO("Amount of lines: %d", ellipses.size());
    extract_ellipses(lines, ellipses);
    if (lines.size() >= 1 && ellipses.size() >= 1) {
      //ROS_INFO("Amount of start:%lf, %lf", lines[0].getStart()[0], lines[0].getStart()[1]);
      //ROS_INFO("Amount of start:%lf, %lf", ellipses[0].getStart()[0], ellipses[0].getStart()[1]);
      /*if (ellipses[0].onLine(ellipses[0].getStart())) {
        ROS_INFO("Start: true");
      } else {
        ROS_INFO("Start: false");
      }
      if (ellipses[0].onLine(ellipses[0].getEnd())) {
        ROS_INFO("Start: true");
      } else {
        ROS_INFO("Start: false");
      }*/
      ROS_INFO("Middle point: (%lf, %lf)", ellipses[0].getPPoint()[0], ellipses[0].getPPoint()[1]);
      //ROS_INFO("Truth Start: " + ellipses[0].onLine(ellipses[0].getStart()));
      //ROS_INFO("Truth End: " + ellipses[0].onLine(ellipses[0].getEnd()));
    }
    visualization_msgs::Marker marker_msg;
    populateMarkerMsg(ellipses, marker_msg);
    ellipse_marker.publish(marker_msg);

    ros::spinOnce();
    rate.sleep();

  }

  return 0;
}
