#ifndef SATVIEWSPLASH_H
#define SATVIEWSPLASH_H

#include <QSplashScreen>
#include <QPixmap>

namespace satview {

class Splash : public QSplashScreen {
Q_OBJECT

public:

  Splash (const QPixmap & pix);
  Splash ();

public slots:

  void Finish ();
  
private:
  void ToTop ();

  QPixmap pm;

};

}


#endif
