#ifndef SJDATASRC_H
#define SJDATASRC_H

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

//
#include <string.h>
#include <string>
#include <sstream>
#include "berndsutil.h"

namespace satview {

class sjdata {
public:
  char    * indata;
  size_t  inlen;
  size_t  cur_pos;
  std::string String () { 
    std::string s;    
    s = berndsutil::toString(indata)
      + "  " + berndsutil::toString(inlen)
      + "  " + berndsutil::toString(cur_pos) ;
    return s;
  }
  
  sjdata() : indata(0),
             inlen(0),
             cur_pos(0)
             {}
};

}

#endif
