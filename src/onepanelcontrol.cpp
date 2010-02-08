
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
  pApp = pA;
  qDebug () << " main win " << mainWin;
  setupUi (mainWin);
  imagePix.load (":/img/noimage.png");
    
  pDisplay->setPixmap(imagePix);
  Additional ();
  ConnectThings ();
  settingsMenu->hide();
  setServer.hide();
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
  connect (reloadAction, SIGNAL (triggered()), this, SLOT (Reload()));
  connect (this, SIGNAL (ReallyShowPic (QImage*)), 
             this, SLOT (DisplayPic (QImage*)));
}

void
OnePanelControl::Additional ()
{
  settingsAction = new QAction (tr("Settings"),mainWin);
  menubar->addAction (settingsAction);
  settingsMenu = new SettingsMenu (mainWin);
  reloadAction = new QAction (tr("Reload"), mainWin);
  menubar->addAction (reloadAction);
}

void
OnePanelControl::Settings ()
{
  DoStopMoving ();
  SettingsAction  action;
  QString serv (mServer.c_str()); 
  QPoint here = QCursor::pos();
  do {
    action = settingsMenu->Exec(here);
    qDebug () << "settings user wants " << action;
    
    switch (action) {
    case ST_server:
      setServer.SetOldname (serv);
      setServer.show();
      setServer.exec();
      setServer.hide();
      serv = setServer.NewName();
      if (serv.length() > 0) {
        SetServer (serv.toStdString());
      }
      qDebug () << "user wanted " << serv;
      break;
    default:
      break;
    }
  } while (action != ST_none);
}

void
OnePanelControl::Reload ()
{
  ReloadDB ();
  qDebug () << " reload ";
}

}
