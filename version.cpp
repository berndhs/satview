#include "version.h"
#include <string>

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

namespace satview {

  std::string SatviewVersion("0.3");
  
  std::string Version ()
  { 
    return std::string(SatviewVersion + " " + std::string(__DATE__) + " "
		       + std::string(__TIME__));
  }

}
