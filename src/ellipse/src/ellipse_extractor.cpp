#include <ellipse/ellipse_extractor.h>
#include <ros/ros.h>
#include <ellipse/ellipse.h>
#include <laser_line_extraction/line.h>
#include <visualization_msgs/Marker.h>
#include <boost/array.hpp>

namespace ellipse_extraction {

  Ellipse_extractor::Ellipse_extractor(ros::NodeHandle& nh, ros::NodeHandle& nh_local):
  nh_(nh),
  nh_local_(nh_local)
  {
    ellipse_marker_ = nh.advertise<visualization_msgs::MarkerArray>("ellipse_marker", 1);
    ellipse_publisher_ = nh.advertise<ellipse::EllipseList>("ellipses", 1);;
  }

  Ellipse_extractor::~Ellipse_extractor(){

  }

  void Ellipse_extractor::extract(std::vector<line_extraction::Line> &lines) {
    ellipses_.clear();
    double width = 0.4;
    for (std::vector<line_extraction::Line>::const_iterator cit = lines.begin();
         cit != lines.end(); ++cit) {
      ellipses_.push_back(ellipse_extraction::Ellipse(*cit, (cit->length() / 2), width));
    }
  }

  void Ellipse_extractor::populateMarkerMsg(visualization_msgs::MarkerArray& marker_array)
  {
    visualization_msgs::Marker marker_msg;
    marker_msg.ns = "ellipse_extraction";

    marker_msg.type = visualization_msgs::Marker::CYLINDER;
    marker_msg.color.r = 0.0;
    marker_msg.color.g = 0.0;
    marker_msg.color.b = 1.0;
    marker_msg.color.a = 0.2;
    marker_msg.header.frame_id = "base_laser_link";
    marker_msg.header.stamp = ros::Time::now();
    for (int i = 0; i < ellipses_.size(); i++)
    {
      marker_msg.id = i;
      marker_msg.pose.position.x = ellipses_[i].getPPoint()[0];
      marker_msg.pose.position.y = ellipses_[i].getPPoint()[1];
      marker_msg.pose.orientation.x = 0;
      marker_msg.pose.orientation.y = 0;
      marker_msg.pose.orientation.z = -sin(ellipses_[i].getAlpha()/2);
      marker_msg.pose.orientation.w = cos(ellipses_[i].getAlpha()/2);
      marker_msg.scale.y = ellipses_[i].getWidth()*2;
      marker_msg.scale.x = ellipses_[i].getHeight()*2;
      marker_msg.scale.z = 1.0;
      marker_array.markers.push_back(marker_msg);
    }

  }

  void Ellipse_extractor::populateEllipseListMsg(ellipse::EllipseList& ellipse_list) {
    for (int i = 0; i < ellipses_.size(); i++)
    {
      ellipse::EllipseSeg ellipse;
      laser_line_extraction::LineSegment line;
      line.radius = ellipses_[i].getLine().getRadius();
      line.angle =  ellipses_[i].getLine().getAngle();
      line.covariance =  ellipses_[i].getLine().getCovariance();
      line.start =  ellipses_[i].getLine().getStart();
      line.end =  ellipses_[i].getLine().getEnd();
      ellipse.line = line;
      ellipse.width = ellipses_[i].getWidth();
      ellipse.height = ellipses_[i].getHeight();
      ellipse.p1 = ellipses_[i].getP1();
      ellipse.p2 = ellipses_[i].getP2();
      ellipse_list.ellipses.push_back(ellipse);
    }
    ellipse_list.size = ellipses_.size();
    ellipse_list.header.frame_id = "base_link";
    ellipse_list.header.stamp = ros::Time::now();
  }

  void Ellipse_extractor::publish(bool visual){
    ellipse::EllipseList ellipses;
    populateEllipseListMsg(ellipses);
    ellipse_publisher_.publish(ellipses);

    if (visual) {
      visualization_msgs::MarkerArray marker_msg;
      populateMarkerMsg(marker_msg);
      ellipse_marker_.publish(marker_msg);
    }
  }

  void Ellipse_extractor::extract_from_msg(ellipse::EllipseList& ellipse_list) {
    ellipses_.clear();
    for(size_t i = 0; i < ellipse_list.size; i++) {
      laser_line_extraction::LineSegment line_seg = ellipse_list.ellipses[i].line;
      boost::array<double, 4> cov = {line_seg.covariance[0], line_seg.covariance[1],
                                     line_seg.covariance[2], line_seg.covariance[3]};
      boost::array<double, 2> start ={line_seg.start[0], line_seg.start[1]};
      boost::array<double, 2> end ={line_seg.end[0], line_seg.end[1]};
      //problem with indices calc
      std::vector<unsigned int> indices = {};
      line_extraction::Line line(line_seg.angle, line_seg.radius,
                                 cov, start, end, indices);
      ellipses_.push_back(ellipse_extraction::Ellipse(line, ellipse_list.ellipses[i].height, ellipse_list.ellipses[i].width));
    }
  }

  std::vector<ellipse_extraction::Ellipse> Ellipse_extractor::getEllipses() {
    return ellipses_;
  }

}
