#ifndef ELLIPSE
#define ELLIPSE

#include "laser_line_extraction/line.h"

namespace ellipse
{

class Ellipse
{

private:
  line_extraction::Line _line;
  double _height;
  double _width;
  double _p1;
  double _p2;

  boost::array<double, 2>  _start;
  boost::array<double, 2>  _end;
  boost::array<double, 2>  _p_point;


public:
  Ellipse(const line_extraction::Line &line, const double height, const double width);
  ~Ellipse();
  std::vector<double[]>     calc_points();

  double  getAngle();
  double  getWidth();
  double  getHeight();

  boost::array<double, 2>&  getStart();
  boost::array<double, 2>&  getEnd();
  boost::array<double, 2>&  getPPoint();
  bool                      onLine(boost::array<double, 2>& point);

};

}
#endif
