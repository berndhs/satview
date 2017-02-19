#include "shapeframe.h"
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

  void
  ShapeFrame::AddShape (Polygon &sh)
  {
    ShapeTag t (sh.TheTag());
    shapes[t] = sh;
  }

  int
  ShapeFrame::NumShapes ()
  {
    return shapes.size();
  }

  void
  ShapeFrame::Write (ostream &out)
  {
    out << "Shape Frame ";
    mytag.Write(out);
    ShapeMap::iterator it = shapes.begin();
    while (it != shapes.end()) {
      it->first.Write(out);
      it++;
    }
  }

  void
  ShapeFrame::Draw (QWidget *pW, QPainter *pP)
  {
    ShapeMap::iterator it = shapes.begin();
    while (it != shapes.end()) {
      it->second.Draw(pW,pP);
      it++;
    }
  }

} // namespace
