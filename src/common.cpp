#include "satview-config.h"

namespace satview {

bool
SatviewMaemo ()
{
  #if defined (Q_MS_MAEMO_5)
  return true;
  #else
  return false;
  #endif
}

}
