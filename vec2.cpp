
/*
//
// miniscsim library, Copyright (C) 200,20094 - Bernd H Stramm
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it may be useful,
// but WITHOUT ANY WARRANTY; WITHOUT EVEN THE IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
*/

#include "vec2.h"
#include <math.h>
#include <string>
#include <sstream>

namespace berndsutil {

  using namespace std;

Vec2::Vec2()
  :myX(0.0),myY(0.0)
{}

Vec2::Vec2(double X, double Y)
  :myX(X),myY(Y)
{}

Vec2::Vec2 (double X, double Y, double Z)
  :myX(X),myY(Y)
{}

Vec2::Vec2(const Vec2& c)
  :myX(c.myX),myY(c.myY)
{}

Vec2::~Vec2()
{}

double
Vec2::Abs()
{
  return sqrt((myX*myX)+(myY*myY));
}

double
Abs ( Vec2 c )
{
  return c.Abs();
}


Vec2 & 
Vec2::Rotate2 (double theta)
{
  double sint = sin(theta), cost = cos(theta);
  double newX = cost * myX - sint * myY;
  double newY = sint * myX + cost * myY;
  myX = newX; myY = newY;
  return *this;
}

Vec2
Rotate2 (Vec2 c, double theta)
{
  double sint = sin(theta), cost = cos(theta);
  double newX = cost * c.myX - sint * c.myY;
  double newY = sint * c.myX + cost * c.myY;
  return Vec2 (newX,newY);
}

Vec2
Vec2::Unit()
{  
  double x = myX, y = myY;
  double l = sqrt(x*x+ y*y);
  x = x/l;
  y = y/l;

  return Vec2 (x,y);
}

Vec2 &
Vec2::MakeUnit()
{
  double x = myX, y = myY ;
  double l = sqrt(x*x+ y*y );
  myX = x/l;
  myY = y/l;
  return *this;
}

Vec2
Unit (Vec2 c)
{
  double x = c.X(), y = c.Y();
  double l = sqrt(x*x+ y*y);
  if (l > 0.0) {
    x = x/l;
    y = y/l;
  } else {
    x = y = 0.0;
  }

  return Vec2 (x,y);
}

double
Dist (const Vec2 c1, const Vec2 c2)
{
  double dX = c1.myX - c2.myX;
  double dY = c1.myY - c2.myY;
  return sqrt((dX*dX)+(dY*dY));
}

std::string
Vec2::String()
{
  ostringstream sX, sY;
  sX << myX; sY << myY; ;
  return string("(") + sX.str() 
      + string(",") + sY.str() 
      + string(")");
}

int
operator== (const Vec2 c1, const Vec2 c2)
{
  return c1.myX == c2.myX 
         && c1.myY == c2.myY;
}

int
operator!= (const Vec2 c1, const Vec2 c2)
{ return !(c1==c2) ; }

Vec2 
operator+ (const Vec2 c1, const Vec2 c2)
{
  return Vec2(c1.myX+c2.myX,c1.myY+c2.myY);
}


Vec2
operator- (const Vec2 c1, const Vec2 c2)
{
  return Vec2 (c1.myX - c2.myX, c1.myY - c2.myY);
}

Vec2
operator* (const Vec2 c1, const double f)
{
  return Vec2 (c1.myX*f, c1.myY*f);
}




Vec2 
operator* (const double f, const Vec2 c2)
{ return c2*f ; }


Vec2 
operator* (const float f, const Vec2 c2)
{ return c2*double(f); }

Vec2 
operator* (const Vec2 c1, const float f)
{ return c1*double(f); }

std::ostream& 
operator<< (std::ostream& out, const Vec2 c)
{ 
  out << "( " << c.X() << " , " << c.Y() <<  " )";
  return out;
}

bool
LexicalLessVec2::operator() (Vec2 c1, Vec2 c2) 
{
  if (c1.myX > c2.myX) return false;
  if (c1.myY > c2.myY) return false;
  // at this point, c1 < c2 or c1 == c2
  return c1 != c2;
}

} // namespace
