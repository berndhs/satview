#ifndef IMAGEWIN_H
#define IMAGEWIN_H
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

#include "ui_imagewin.h"
#include "satpicbuf.h"
#include <QImage>


namespace satview {

  class ControlPanel;

class ImageWin : public QWidget, public Ui_ImageBox {

Q_OBJECT

  public:

    ImageWin (ControlPanel * pControl);
    ~ImageWin();

    void ShowImage (QImage *pImg);

  public slots:

    void quit ();

    void NotImplemented ();

  private:

    ControlPanel  *pControl;


};

}
#endif
