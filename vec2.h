
/*
//
// miniscsim library, Copyright (C) 2004,2009 - Bernd H Stramm
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

#ifndef COORD_H
#define COORD_H


#include <iostream>
#include <string>



namespace berndsutil {

/** @ingroup BerndsUtils */

/** @brief Vec2 is a vector of 2 components.
 */

class Vec2 {

 public:

  Vec2();
  Vec2 (double X, double Y, double Z);
  Vec2 (double X, double Y);
  Vec2 (const Vec2& c);

  ~Vec2();

  static Vec2 Zero() { return Vec2(0.0,0.0);}


  double X() const { return myX; }
  double Y() const { return myY; }
  double X(double x) { return myX = x; }
  double Y(double y) { return myY = y; }
  double Abs();

  double Manhattan () { return myX+myY; }

  Vec2 & Rotate2 (double theta); 
  friend Vec2 Rotate2 (Vec2 c, double theta);

  Vec2 Unit ();
  friend Vec2 Unit (Vec2 c);
  Vec2 & Add (const Vec2 c) 
               { myX += c.myX; myY += c.myY; 
                 return *this;}
  Vec2 & Scale (const double d) 
               { myX *= d; myY *= d;
                 return *this;}
  Vec2 & MakeUnit ();

  std::string String();

  friend double Abs(Vec2 c1);

 
  friend double Dist (const Vec2 c1, const Vec2 c2);

  friend int operator== (const Vec2 c1, 
			 const Vec2 c2);

  friend Vec2 operator+ (const Vec2 c1,
                          const Vec2 c2);

  friend Vec2 operator- (const Vec2 c1,
                          const Vec2 c2);
 
  friend Vec2 operator* (const Vec2 c1,
                          const double  f);
friend Vec2 operator* (const double f, const Vec2 c2);

friend Vec2 operator* (const float f, const Vec2 c2);

friend Vec2 operator* (const Vec2 c1, const float f);

friend std::ostream& operator<<(std::ostream& out, const Vec2 c);
friend int operator!= (const Vec2 c1, const Vec2 c2);

 friend class LexicalLessVec2;

 private:

    double myX;
    double myY;


};

 class LexicalLessVec2 {
 public:
   bool operator() (Vec2 c1, Vec2 c2);
 };


} // namespace

#endif
