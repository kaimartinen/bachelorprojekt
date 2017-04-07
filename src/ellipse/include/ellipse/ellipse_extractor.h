#ifndef ELLIPSE_EXTRACTOR
#define ELLIPSE_EXTRACTOR

#include "ellipse/ellipse.h"
#include "ellipse/EllipseSeg.h"
#include "ellipse/EllipseList.h"
#include "laser_line_extraction/line.h"
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

namespace ellipse_extraction
{

class Ellipse_extractor
{
private:
  ros::Publisher ellipse_publisher_;
  ros::Publisher ellipse_marker_;

  ros::NodeHandle nh_;
  ros::NodeHandle nh_local_;
  std::vector<ellipse_extraction::Ellipse> ellipses_;

  void populateMarkerMsg(visualization_msgs::MarkerArray& marker_msg);
  void populateEllipseListMsg(ellipse::EllipseList& ellipse_list);

public:
  Ellipse_extractor(ros::NodeHandle &nh, ros::NodeHandle &nh_local);
  ~Ellipse_extractor();
  void extract(std::vector<line_extraction::Line> &lines);
  void publish(bool visual);

  std::vector<ellipse_extraction::Ellipse> getEllipses();
  void extract_from_msg(ellipse::EllipseList& ellipse_list);
};

}

#endif
