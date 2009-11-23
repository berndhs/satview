
//
//  Copyright (C) 2009 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//


#include "blob-image.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

namespace satview {

//
/// @brief Blob_Image() - Load a JPEG image from a memory buffer.
//

Blob_Image::Blob_Image (sjdata & data_source)  
  : QImage() {

  char * data = (data_source.indata);
  imageok = loadFromData ((uchar*)data,
                          data_source.inlen);
}

} // namespace
