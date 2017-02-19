#ifndef SHAPEFRAME_H
#define SHAPEFRAME_H

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

#include <map>
#include "frametypes.h"

namespace satview {

  /** @brief A ShapeFrame is the set of shapes (sets of points) that
   * are present (visible) at the same time.
   */


  class ShapeFrame {

  public:

    ShapeFrame(){}

    ShapeFrame(FrameTag t):mytag(t){}

    void SetTag (FrameTag t) { mytag = t; }

    void AddShape (Polygon & sh);
    int  NumShapes ();

    FrameTag Tag() { return mytag;}

    void Draw (QWidget *pW, QPainter *pP);

    void Write (ostream & out);


  private:

    typedef map<ShapeTag, Polygon> ShapeMap;

    ShapeMap shapes;

    FrameTag mytag;
    
    
  };

} // namespace

#endif
