#include "ellipse/ellipse.h"
#include "laser_line_extraction/line.h"

namespace ellipse
{

Ellipse::Ellipse(const line_extraction::Line &line, const double height, const double width):
  _line(line),
  _height(height),
  _width(width),
  _p1(1.5),
  _p2(2.0)
{

}

Ellipse::~Ellipse()
{

}

std::vector<double[]> Ellipse::calc_points()
{
  
}

}
