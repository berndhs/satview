
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

//


// Portions Copyright 1998-2005 by Bill Spitzak and others.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// 
//

#ifndef BLOB_IMAGE_H
#define BLOB_IMAGE_H

#include "satview-config.h"
#include "sjdatasrc.h"

#include <QImage>

namespace satview {

class Blob_Image : public QImage {

  public:

  Blob_Image(sjdata & data_source);

  private:

  uchar * array;
  uint    wide, hi, deep;
  int     alloc_array;
  bool    imageok;
};

} // namespace

#endif
