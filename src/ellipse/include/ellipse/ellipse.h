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

public:
  Ellipse(const line_extraction::Line &line, const double height, const double width);
  ~Ellipse();
  std::vector<double[]> calc_points();
};

}
#endif
