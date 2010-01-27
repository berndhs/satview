
#include "twopanelcontrol.h"

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
#include <iostream>
#include <QMessageBox>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include <QDesktopServices>
#include <QUrl>
#include "satpiclist.h"
#include "satpicbuf.h"
#include "version.h"
#include <time.h>

#include "delib-debug.h"

using namespace std;

namespace satview  {

TwoPanelControl::TwoPanelControl (QApplication *pA)
{
    pApp = pA;

    setupUi(this);

    idtagBox->setReadOnly(true);
    dateBox->setReadOnly(true);
    mMethWebLabel = QString("Web");
    mMethDirLabel = QString("Direct");
    mStateStoppedText = "done";
    mStateRunningText = "running";
    mStateLoadingText = "loading";
    SetIFLabel();
    staticDraw->setChecked(true);
    staticDraw->setEnabled(false);
    
    pDisplay = new ImageWin(this,true);
    
    ChangeStatusLabel (mStateStoppedText);
    bool loadok;
    loadok = mStateStoppedPix.load(":/img/statusDone.png");
    loadok &= mStateRunningPix.load(":/img/statusRun.png");
    loadok &= mStateLoadingPix.load(":/img/statusLoad.png");
    loadok &= imagePix.load (":/img/noimage.png");
    
    pDisplay->setPixmap(imagePix);
    pDisplay->show();
    
    if (!loadok) {
      exit(1);
    }
    
    connect (&showTimer, SIGNAL(timeout()), this, SLOT(DoShowMove()));

    /** @brief make sure we check for clicked buttons every now and then */
    connect (&updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    /** and we have a whole bunch of buttons to connect */
    ConnectButtons ();
    
    connect (this, SIGNAL (ReallyShowPic (QImage*)), 
             this, SLOT (DisplayPic (QImage*)));
    connect (this, SIGNAL (ShowMove()), 
             this, SLOT (DoShowMove()));
    
    TwoStartTimers ();

}

void
TwoPanelControl::ConnectButtons ()
{
    connect (versionButton, SIGNAL(clicked()), this, SLOT(ShowVersion()));
    connect (authorButton, SIGNAL(clicked()), this, SLOT(LinkToBernd()));

    connect (newNameButton, SIGNAL(clicked()), this, SLOT(DoSwitchPicname()));
    connect (quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    
    connect (reloadImgButton, SIGNAL(clicked()),
                this, SLOT(ShowImgAgain()));
    connect (stopButton, SIGNAL(clicked()), this, SLOT(DoStopMoving()));
    stopButton->setEnabled(false);

    connect (runBackButton, SIGNAL(clicked()), this, SLOT(DoRunBack()));
    connect (backWeekButton, SIGNAL(clicked()), this, SLOT(DoRunBackWeek()));
    connect (backDayButton, SIGNAL(clicked()), this, SLOT(DoRunBackDay()));
    connect (backStepButton, SIGNAL(clicked()), this, SLOT(DoStepBack()));
    connect (runForwardButton, SIGNAL(clicked()), this, SLOT(DoRunFwd()));
    connect (forwardWeekButton, SIGNAL(clicked()), this, SLOT(DoRunFwdWeek()));
    connect (forwardDayButton, SIGNAL(clicked()), this, SLOT(DoRunFwdDay()));
    connect (forwardStepButton, SIGNAL(clicked()), this, SLOT(DoStepFwd()));
    connect (windBackWeekButton, SIGNAL(clicked()), this, 
                SLOT(DoWindBackWeek()));
    connect (windBackDayButton, SIGNAL(clicked()), this, 
                SLOT(DoWindBackDay()));
    connect (windBackAllButton, SIGNAL(clicked()), this, SLOT(DoWindBack()));
    connect (windFwdWeekButton, SIGNAL(clicked()), this, SLOT(DoWindFwdWeek()));
    connect (windFwdDayButton, SIGNAL(clicked()), this, SLOT(DoWindFwdDay()));
    connect (windFwdAllButton, SIGNAL(clicked()), this, SLOT(DoWindFwd()));
    connect (connectServerButton, SIGNAL(clicked()), this, SLOT(ReloadDB()));
    connect (reloadServerButton, SIGNAL(clicked()), this, SLOT(ReloadDB()));

    connect (clearTrackButton, SIGNAL(clicked()), this, SLOT(ClearTrack()));
    connect (clearFrameButton, SIGNAL(clicked()), this, SLOT(ClearFrame()));
    connect (finishPolygonButton, SIGNAL(clicked()),
	     this, SLOT(FinishPolygon()));
    connect (saveFrameButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
}

TwoPanelControl::~TwoPanelControl()
{
  quit();
  if (pDisplay) {
    delete pDisplay;
    pDisplay = 0;
  }
}

void
TwoPanelControl::TwoStartTimers ()
{
    updateDelay = 300;
    
    updateTimer.start(updateDelay);

}

void
TwoPanelControl::ChangeSettings ()
{
  qDebug () << " settings called ";
}

void
TwoPanelControl::quit()
{
  showTimer.stop();
  updateTimer.stop();
  if (pDisplay) {
    pDisplay->quit();
  }
  SatPicList::Instance()->Ditch();
  if (pApp) {
    pApp->quit();
  }
}

void
TwoPanelControl::update()
{
  if (DBWaiting()) {
    qDebug () << " something missing in update?" ;
  }
  this->AbstractControl::update();
}

void
TwoPanelControl::show()
{
  if (pDisplay) {
    pDisplay->show();
  }
  AbstractControl::show();
}


void
TwoPanelControl::LinkToBernd()
{
  QDesktopServices::openUrl
    (QUrl(QString("http://www.bernd-stramm.com/freestuff.php")));
}

void
TwoPanelControl::ShowVersion()
{
  ShowVersionWindow();
}

void
TwoPanelControl::ChangeStatusLabel (QString status)
{
  if (status == mStateRunningText) {
    runStatusLabel->setPixmap (mStateRunningPix);
  } else if (status == mStateLoadingText) {
    runStatusLabel->setPixmap (mStateLoadingPix);
  } else {
    runStatusLabel->setPixmap (mStateStoppedPix);
  }
  runStatusLabel->setText (status);
}

void 
TwoPanelControl::ShowStatus ()
{
  string statusmsg = string("Status:\n") + mServer + "\r " + mConMeth;
  QMessageBox msg;
  QTimer::singleShot(15000,&msg, SLOT(accept()));
  msg.setText (statusmsg.c_str());
  msg.exec();
}



void
TwoPanelControl::ShowIndexRec (SatPicBuf *pBuf)
{
     
  time_t t = pBuf->Ident();
  SetIdentTag((berndsutil::toString(t)
                     + string(" - ")
                     + berndsutil::toString(pBuf->Serial())).c_str() 
                    );
  string plain = TimeAndDate (t);
  SetDate (plain.c_str());
  SetPicname (pBuf->PicName().c_str());
  SetRemark (pBuf->Remark().c_str());
}

void
TwoPanelControl::DisplayPic (QImage *pI)
{
  static SatPicBuf * pOldBuf(0);

  SatPicBuf *pBuf = mPicState.pBuf;
  if (pDisplay) {
    pDisplay->SetImage (pI,FrameTag(pBuf->Ident()));
    time_t t = pBuf->Ident();
    string t_str = TimeAndDate (t);
    pDisplay->SetText (QString(t_str.c_str()));
    pDisplay->update();
  }
  ShowIndexRec (pBuf);
  if (pApp) {
    pApp->processEvents();
  }
  if (pOldBuf) {
    if (pOldBuf != pBuf) {
      pOldBuf->Forget_Image();
      pOldBuf = pBuf;

    }
  }  
  mPicState.waiting = false;  
}

void
TwoPanelControl::ShowImgAgain ()
{
  SatPicBuf * pBuf;
  pBuf = SatPicList::Instance()->Current(false);
  mPicState.SetNewBuf(pBuf);
  pBuf->Forget_Image();
  ShowPic(pBuf);
}



void
TwoPanelControl::ClearTrack ()
{
  if (pDisplay) {
    pDisplay->ClearTrack();
  }
}

void
TwoPanelControl::ClearFrame ()
{
  if (pDisplay) {
    pDisplay->ClearFrame();
  }
}

void
TwoPanelControl::FinishPolygon()
{
  if (pDisplay) {
    pDisplay->FinishPolygon();
  }
}

void
TwoPanelControl::NotImplemented ()
{
  QMessageBox msgBox;
  QTimer::singleShot(15000, &msgBox, SLOT(accept()));
  msgBox.setText("not implemented at this time");
  msgBox.exec();
}

void
TwoPanelControl::DoShowMove ()
{
  if (DBWaiting()) {
    ChangeStatusLabel(mStateLoadingText);
  } else {
    ChangeStatusLabel(mRunState.stopped ?
                             mStateStoppedText
                  			   : mStateRunningText);
  }
  if (!mRunState.stopped) {
    showTimer.stop();     /** don't want to get called twice */
    if (mRunState.backwards) {
      BackSome();
    } else {
      FwdSome();
    }
    showTimer.start(currentDelay);
  }
}

void
TwoPanelControl::ConnectDB ()
{
  NotImplemented();
}

string
TwoPanelControl::GetBoxString (QPlainTextEdit *pBox)
{
  QString newval = pBox->toPlainText();
  QByteArray bytes = newval.toLatin1();
  return string(bytes.data());
}

void
TwoPanelControl::ReloadDB ()
{
  mServer = GetBoxString(serverBox);
  mPicname = GetBoxString(picnameBox);
  SatPicList::Instance()->SetServer(mServer);
  SatPicList::Instance()->SetFilename(mPicname);
  SatPicList::Instance()->SetMethod(mMeth);
  SatPicList::Instance()->Ditch();
  bool loadedok = SatPicList::Instance()->LoadFromDB();
  if (!loadedok && !DBWaiting()) {
     QMessageBox msgBox;
     QTimer::singleShot(15000, &msgBox, SLOT(accept()));
     string badmsg = "Cannot Load from Server '" + mServer
       + "' using Method '" + mConMeth + "'";
     msgBox.setText(badmsg.c_str());
     msgBox.exec();
  }
  if (!DBWaiting()) {
    AbstractControl::DoWindFwd(0,true);
    AbstractControl::DoStepFwd();
  }
  update();
}

void
TwoPanelControl::DoSwitchPicname ()
{
  QString newval = picnameBox->toPlainText();
  QByteArray bytes = newval.toLatin1();
  mPicname = string(bytes.data());
  AbstractControl::DoSwitchPicname (bytes);
  update();
}

string
TwoPanelControl::SetConMeth (string cm)
{
  bool is_dir = (cm == "dir");
  mConMeth = cm;
  mMeth = DBConnection::Con_Web;    // safer
  if (is_dir) {
    mMeth = DBConnection::Con_MySql;
  }
  SetIFLabel();
  return mConMeth;
}

string
TwoPanelControl::ConMeth ()
{
  return mConMeth;
}

void
TwoPanelControl::SetIFLabel ()
{
  QString newlabel;
  switch (mMeth) {
  case DBConnection::Con_Web:
    newlabel = mMethWebLabel;
    break;
  case DBConnection::Con_MySql:
    newlabel = mMethDirLabel;
    break;
  default:
    newlabel = QString("None");
  }
  interfaceLabel->setText(newlabel);
}



}
