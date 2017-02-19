#include "frametypes.h"

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

namespace satview {


  Polygon & Polygon::operator= (const Polygon &p)
  {
    if (this != &p) {
      numCorners = p.numCorners;
      corners.clear();
      corners = p.corners;
    }
    return *this;  
  }

  void
  Polygon::Draw (QWidget *pW, QPainter *painter)
  {
    if ((!pW) || (!painter) || corners.size() < 1) {
      return;
    }
    painter->save();
    painter->setPen (QColor (0,100,255,200));
    painter->setBrush (Qt::NoBrush);
    PointList::iterator corner1 = corners.begin();
    if (corners.size() == 1) {
      // draw a point
    } else {
      QLine line;
      PointList::iterator corner2 = corner1++;
      while (corner2 != corners.end()) {
        line.setP1(*corner1);
        line.setP2(*corner2);
        painter->drawLine(line);
        corner1 = corner2;
        corner2++;
      }
      line.setP1(corners.front());
      line.setP2(corners.back());
      painter->drawLine(line);
    }
    painter->restore();
  }


} // namespace
