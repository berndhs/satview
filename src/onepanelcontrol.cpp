
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


OnePanelControl::OnePanelControl (QGuiApplication *pA)
{
  mainWin = new QMainWindow;
  pApp = pA;
  setupUi (mainWin);
  imagePix.load (":/img/noimage.png");
    
  picnameBox = new TextBox (mainWin);
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
  connect (picnameAction, SIGNAL (triggered()), this, SLOT (NewPicname()));
  connect (reloadAction, SIGNAL (triggered()), this, SLOT (Reload()));
  connect (this, SIGNAL (ReallyShowPic (QImage*)), 
             this, SLOT (DisplayPic (QImage*)));
}

void
OnePanelControl::Additional ()
{
  picnameBox->setWindowTitle (tr("Image Name"));
  picnameBox->SetLabel ("");
  picnameBox->SetDefault ("");
  picnameAction = new QAction (tr("Image Name"), mainWin);
  menubar->addAction (picnameAction);
  settingsAction = new QAction (tr("Settings"),mainWin);
  menubar->addAction (settingsAction);
  settingsMenu = new SettingsMenu (mainWin);
  reloadAction = new QAction (tr("Reload"), mainWin);
  menubar->addAction (reloadAction);
}

void
OnePanelControl::NewPicname ()
{
  connect (picnameBox->okButton, SIGNAL (clicked()),
           this, SLOT (DoCallSwitch ()));
  connect (picnameBox, SIGNAL (accepted()),
           this, SLOT (DoCallSwitch ()));
  connect (picnameBox, SIGNAL (rejected()),
           this, SLOT (CloseNewPicname()));
  picnameBox->SetDefault (mPicname.c_str());
  picnameBox->show ();
}

void
OnePanelControl::CloseNewPicname ()
{
  disconnect (picnameBox->okButton, SIGNAL (clicked()),
           this, SLOT (DoCallSwitch ()));
  disconnect (picnameBox, SIGNAL (accepted()),
           this, SLOT (DoCallSwitch ()));  
  disconnect (picnameBox, SIGNAL (rejected()),
           this, SLOT (CloseNewPicname()));
  picnameBox->hide ();
}

void
OnePanelControl::DoCallSwitch ()
{
  QString newname = picnameBox->GetText ();
  newNameBytes = newname.toLatin1();
  CloseNewPicname ();
  AbstractControl::DoSwitchPicname (newNameBytes);
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
}

}
