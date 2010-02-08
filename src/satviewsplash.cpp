#include "satviewsplash.h"

namespace satview {

Splash::Splash (const QPixmap & pix)
{
  setPixmap (pix);
}

Splash::Splash ()
{
  pm.load (":/img/splash.png");
  setPixmap (pm);
}

void
Splash::Finish ()
{
  close();
}


}
