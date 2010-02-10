#include "satview-config.h"

namespace satview {

bool
SatviewMaemo ()
{
  #if defined (Q_WS_MAEMO_5)
  return true;
  #else
  return false;
  #endif
}

}
