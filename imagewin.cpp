#include "imagewin.h"
#include "controlpanel.h"
#include <string>
#include <iostream>
#include <QMessageBox>
#include <QTimer>


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

using namespace std;

namespace satview {

  ImageWin::ImageWin (ControlPanel * pCon)
    :pControl(0)
  {
    pControl = pCon;
    setupUi(this);
  }

  ImageWin::~ImageWin ()
  {
    quit();
  }

  void
  ImageWin::quit()
  {
  }

  void
  ImageWin::NotImplemented ()
  {
    cout << "not implemented" << endl;
    QMessageBox msgBox;
    QTimer::singleShot(15000, &msgBox, SLOT(accept()));
    msgBox.setText("not implemented at this time");
    msgBox.exec();
  }

  void
  ImageWin::ShowImage (QImage * pImg)
  {
    CanvasArea->setPixmap(QPixmap::fromImage(*pImg));
    CanvasArea->update();
    CanvasArea->show();
  }

}
