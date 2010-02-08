#include "satviewsplash.h"

namespace satview {

Splash::Splash (const QPixmap & pix)
{
  setPixmap (pix);
  ToTop ();
}

Splash::Splash ()
{
  pm.load (":/img/splash.png");
  setPixmap (pm);
  ToTop ();
}

void
Splash::Finish ()
{
  close();
}

void
Splash::ToTop ()
{
  Qt::WindowFlags flags = this->windowFlags ();
  flags |=  Qt::WindowStaysOnTopHint;
  this->setWindowFlags (flags);
}

}
