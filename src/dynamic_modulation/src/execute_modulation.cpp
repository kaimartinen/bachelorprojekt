#include "ros/ros.h"
#include <ellipse/ellipse_extractor.h>

void callback_ellipse(){

}

int main(int argc, char **argv) {
  ros::init(argc, argv, "dynamic_modulation");

  ros::NodeHandle m;
  ros::NodeHandle mh_local("~");
  ros::Subscriber ellipse_sub = m.subscribe("ellipses", 1000, callback_ellipse);
  ellipse_extraction::Ellipse_extractor extractor(nh, nh_local);


  double frequency;
  mh_local.param<double>("frequency", frequency, 25);
  ros::Rate rate(frequency);

  while (ros::ok())
  {

    ros::spinOnce();
    rate.sleep();
  }

  return 0;
}
