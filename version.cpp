#include "version.h"
#include <string>

namespace satview {

  std::string SatviewVersion("0.3");
  
  std::string Version ()
  { 
    return std::string(SatviewVersion + " " + std::string(__DATE__) + " "
		       + std::string(__TIME__));
  }

}
