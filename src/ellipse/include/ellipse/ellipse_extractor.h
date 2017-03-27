#ifndef ELLIPSE_EXTRACTOR
#define ELLIPSE_EXTRACTOR

#include "ellipse.h"
#include "laser_line_extraction/line.h"
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

namespace ellipse
{

class Ellipse_extractor
{
private:
  ros::Publisher ellipse_publisher_;
  ros::Publisher ellipse_marker_;

  ros::NodeHandle nh_;
  ros::NodeHandle nh_local_;
  std::vector<ellipse::Ellipse> ellipses_;

  void populateMarkerMsg(visualization_msgs::Marker &marker_msg);

public:
  Ellipse_extractor(ros::NodeHandle&, ros::NodeHandle&);
  ~Ellipse_extractor();
  void extract(std::vector<line_extraction::Line> &lines);
  void run(bool visual);
};

}

#endif
