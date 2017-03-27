#include <ellipse/ellipse_extractor.h>
#include <ros/ros.h>
#include <ellipse/ellipse.h>
#include <laser_line_extraction/line.h>
#include <visualization_msgs/Marker.h>

namespace ellipse {

  Ellipse_extractor::Ellipse_extractor(ros::NodeHandle& nh, ros::NodeHandle& nh_local):
  nh_(nh),
  nh_local_(nh_local)
  {
    ellipse_marker_ = nh.advertise<visualization_msgs::Marker>("ellipse_marker", 1);
    ellipse_publisher_ = nh.advertise<visualization_msgs::Marker>("ellipses", 1);;
  }

  Ellipse_extractor::~Ellipse_extractor(){

  }

  void Ellipse_extractor::extract(std::vector<line_extraction::Line> &lines) {
    ellipses_.clear();
    double width = 2.0;
    for (std::vector<line_extraction::Line>::const_iterator cit = lines.begin();
         cit != lines.end(); ++cit) {
      ellipses_.push_back(ellipse::Ellipse(*cit, (cit->length() / 2), width));
    }
  }

  void Ellipse_extractor::populateMarkerMsg(visualization_msgs::Marker &marker_msg)
  {
    marker_msg.ns = "ellipse_extraction";
    marker_msg.id = 0;
    marker_msg.type = visualization_msgs::Marker::CYLINDER;
    marker_msg.color.r = 0.0;
    marker_msg.color.g = 0.0;
    marker_msg.color.b = 1.0;
    marker_msg.color.a = 1.0;
    for (int i = 0; i < ellipses_.size(); i++)
    {
      geometry_msgs::Point p;
      p.x = ellipses_[i].getPPoint()[0];
      p.y = ellipses_[i].getPPoint()[1];
      p.z = 0;
      marker_msg.points.push_back(p);
      marker_msg.scale.x = ellipses_[i].getWidth();
      marker_msg.scale.y = ellipses_[i].getHeight();
      marker_msg.scale.z = 1.0;
    }
    marker_msg.header.frame_id = "base_link";
    marker_msg.header.stamp = ros::Time::now();
  }

  void Ellipse_extractor::run(bool visual){

    if (visual) {
      visualization_msgs::Marker marker_msg;
      populateMarkerMsg(marker_msg);
      ellipse_marker_.publish(marker_msg);
    }
  }

}
