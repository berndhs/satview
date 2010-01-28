
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
  mainWin = new QMainWindow;
  qDebug () << " main win " << mainWin;
  setupUi (mainWin);
  imagePix.load (":/img/noimage.png");
    
  pDisplay->setPixmap(imagePix);
  Additional ();
  ConnectThings ();
  settingsMenu->hide();
  mainWin->show();
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

void
OnePanelControl::ConnectThings ()
{
  connect (stopButton, SIGNAL(clicked()), this, SLOT(DoStopMoving())); 
  connect (runBackButton, SIGNAL(clicked()), this, SLOT(DoRunBack()));
  connect (runForwardButton, SIGNAL(clicked()), this, SLOT(DoRunFwd())); 
  connect (backStepButton, SIGNAL(clicked()), this, SLOT(DoStepBack()));
  connect (forwardStepButton, SIGNAL(clicked()), this, SLOT(DoStepFwd()));
  connect (settingsAction, SIGNAL (triggered()), this, SLOT (Settings()));
  connect (this, SIGNAL (ReallyShowPic (QImage*)), 
             this, SLOT (DisplayPic (QImage*)));
}

void
OnePanelControl::Additional ()
{
  settingsAction = new QAction (tr("Settings"),mainWin);
  menubar->addAction (settingsAction);
  settingsMenu = new SettingsMenu (mainWin);
}

void
OnePanelControl::Settings ()
{
  DoStopMoving ();
  SettingsAction  action;
  
  action = settingsMenu->Exec();
  qDebug () << "settings user wants " << action;
  
}

}
