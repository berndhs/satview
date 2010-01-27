
#include "onepanelcontrol.h"

//
//  Copyright (C) 2010 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

namespace satview {


OnePanelControl::OnePanelControl (QApplication *pA)
{
  setupUi (this);
  imagePix.load (":/img/noimage.png");
    
  pDisplay->setPixmap(imagePix);

  connect (stopButton, SIGNAL(clicked()), this, SLOT(DoStopMoving())); 
  connect (runBackButton, SIGNAL(clicked()), this, SLOT(DoRunBack()));
  connect (runForwardButton, SIGNAL(clicked()), this, SLOT(DoRunFwd())); 
  connect (backStepButton, SIGNAL(clicked()), this, SLOT(DoStepBack()));
  connect (forwardStepButton, SIGNAL(clicked()), this, SLOT(DoStepFwd()));
    
  connect (this, SIGNAL (ReallyShowPic (QImage*)), 
             this, SLOT (DisplayPic (QImage*)));
}

void
OnePanelControl::DisplayPic (QImage *pImg)
{
  SatPicBuf *pBuf = mPicState.pBuf;
  if (pDisplay) {
    pDisplay->SetImage (pImg);
    time_t t = pBuf->Ident();
    string t_str = TimeAndDate (t);
    pDisplay->SetText (QString(t_str.c_str()));
    pDisplay->update();
  }
  update();
}

}
