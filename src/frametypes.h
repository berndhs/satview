#ifndef FRAMETYPES_H
#define FRAMETYPES_H

//
//  Copyright (C) 2017 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

#include <iostream>
#include <QPainter>
#include <QWidget>
#include <deque>
#include <sstream>
#include <string>

using namespace std;

namespace satview {



  typedef deque<QPoint> PointList;
  typedef deque<QLine>  LineList;

  class Tag {

  public:

    Tag():tagval(0){}
    Tag(unsigned long int t):tagval(t) {}
    Tag(const Tag& other) { tagval = other.tagval; }
    ~Tag(){}

    unsigned long int Val() { return tagval; }
    unsigned long int Set (unsigned long int t) { tagval = t; return tagval;}

    bool operator < (const Tag &other) const { return tagval < other.tagval; }
    bool operator == (const Tag &other) const { return tagval == other.tagval;}
    Tag & operator = (const Tag &other) 
      {
        tagval = other.tagval;
        return *this;
      }
    string String () 
                { 
                  std::ostringstream stream;
                  stream << tagval;
                  return stream.str();
                }

    void Write (ostream & out) const
      { out << "<tag> " << tagval << " </tag>"; }
    
  private:
    unsigned long int  tagval;
  }; // class Tag

  typedef Tag FrameTag;
  typedef Tag ShapeTag;

  class Shape {

  public:

    Shape():nameSet(false) {}
    ~Shape() {}
   
    ShapeTag  TheTag() { return tag; }
    string    Name () 
    { if (nameSet) {
        return name;
      } else {
        return tag.String();
      }
    }

    void  SetTag (ShapeTag t) { tag = t; }
    void  SetName (string n) { name = n; nameSet = true; }

    virtual void Draw (QWidget *pW, QPainter *pP) = 0;

  private:

    ShapeTag   tag;
    string     name;
    bool       nameSet;

  };  

class Polygon: public Shape {

  public:

    Polygon():numCorners(0){}
    Polygon(const Polygon &old) 
      {
        numCorners = old.numCorners;
        corners = old.corners;
      }

    void AddPoint (QPoint p) { corners.push_front(p); numCorners++; }
    int  NumCorners() { return numCorners; }

    void Draw (QWidget *pW, QPainter *pP);

    Polygon & operator = (const Polygon &p );

  private:


    PointList     corners;
    int            numCorners;

  };



  typedef deque<Polygon>  ShapeList;

} // namespace


#endif
