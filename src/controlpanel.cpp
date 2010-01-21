
#include "controlpanel.h"

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

#include "satview-debug.h"

using namespace std;

namespace satview  {

ControlPanel::ControlPanel (QApplication *pA)
  :wantRerun(false),
   mMeth(DBConnection::Con_None)
{
    pApp = pA;

    setupUi(this);

    pDisplay = new ImageWin(this);

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

    /** @brief the runstate is context for the animation and winding features*/
    mRunState.timelimit = 0;
    mRunState.allway = false;
    mRunState.show   = true;
    mRunState.backwards = false;
    mRunState.stopped = true;
    mRunState.pBuf    = 0;
    mPicState.waiting = false;
    mPicState.failed  = false;
    mPicState.pBuf    = 0;
    mPicState.pImg    = 0;

    ChangeStatusLabel (mStateStoppedText);
    bool loadok;
    loadok = mStateStoppedPix.load(":/img/statusDone.png");
    loadok &= mStateRunningPix.load(":/img/statusRun.png");
    loadok &= mStateLoadingPix.load(":/img/statusLoad.png");
    if (!loadok) {
      exit(1);
    }
    
    connect (&showTimer, SIGNAL(timeout()), this, SLOT(DoShowMove()));

    /** @brief make sure we check for clicked buttons every now and then */
    connect (&updateTimer, SIGNAL(timeout()), this, SLOT(update()));

    /** and we have a whole bunch of buttons to connect */

    connect (versionButton, SIGNAL(clicked()), this, SLOT(ShowVersion()));
    connect (authorButton, SIGNAL(clicked()), this, SLOT(LinkToBernd()));

    connect (newNameButton, SIGNAL(clicked()), this, SLOT(DoSwitchPicname()));
    connect (quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect (restartButton, SIGNAL(clicked()), this, SLOT(DoAgain()));
    
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
    connect (changeInterfaceButton, SIGNAL(clicked()),
	     this, SLOT(ToggleConn()));

    connect (clearTrackButton, SIGNAL(clicked()), this, SLOT(ClearTrack()));
    connect (clearFrameButton, SIGNAL(clicked()), this, SLOT(ClearFrame()));
    connect (finishPolygonButton, SIGNAL(clicked()),
	     this, SLOT(FinishPolygon()));
    connect (saveFrameButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));

    StartTimers ();

}

ControlPanel::~ControlPanel()
{
  quit();
  if (pDisplay) {
    delete pDisplay;
    pDisplay = 0;
  }
}

void
ControlPanel::StartTimers ()
{
    showTimeDelay = 100;
    noshowTimeDelay = 1;
    currentDelay = noshowTimeDelay;
    updateDelay = 300;
    showTimer.start(currentDelay);
    updateTimer.start(updateDelay);

}

void
ControlPanel::Restart ()
{
   hide();
   showTimer.stop();
   updateTimer.stop();
   if (pDisplay) {
     delete pDisplay;
   }
   pDisplay = new ImageWin(this);
   StartTimers();
   pDisplay->show();
   show();
}

void
ControlPanel::Reset ()
{
   wantRerun = false;
}

void
ControlPanel::quit()
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
ControlPanel::DoAgain ()
{
  wantRerun = true;
  quit();
}

void
ControlPanel::update()
{
  if (DBWaiting()) {
  }
  this->QDialog::update();
}

void
ControlPanel::show()
{
  if (pDisplay) {
    pDisplay->show();
  }
  QDialog::show();
}

bool
ControlPanel::DBWaiting()
{
  return SatPicList::Instance()->DBConnect()->Waiting();
}

void
ControlPanel::LinkToBernd()
{
  QDesktopServices::openUrl
    (QUrl(QString("http://www.bernd-stramm.com/freestuff.php")));
}

void
ControlPanel::ShowVersion()
{
  ShowVersionWindow();
}

void
ControlPanel::ChangeStatusLabel (QString status)
{
  if (status == mStateRunningText) {
    runStatusLabel->setPixmap (mStateRunningPix);
  } else if (status == mStateLoadingText) {
    runStatusLabel->setPixmap (mStateLoadingPix);
  } else {
    runStatusLabel->setPixmap (mStateStoppedPix);
  }
 // runStatusLabel->setText (status);
}

void 
ControlPanel::ShowStatus ()
{
  string statusmsg = string("Status:\n") + mServer + "\r " + mConMeth;
  QMessageBox msg;
  QTimer::singleShot(15000,&msg, SLOT(accept()));
  msg.setText (statusmsg.c_str());
  msg.exec();
}


void
ControlPanel::PicArrive (QImage * pImg)
{
  mPicState.pImg = pImg;
  mPicState.waiting = false;
  ReallyShowPic();
}

void
ControlPanel::ShowIndexRec (SatPicBuf *pBuf)
{
     
  time_t t = pBuf->Ident();
  SetIdentTag((berndsutil::toString(t)
                     + string(" - ")
                     + berndsutil::toString(pBuf->Serial())).c_str() 
                    );
  struct tm theTime;
#ifdef _MSC_VER
  localtime_s(&theTime,&t);
#else
  localtime_r (&t, &theTime);
#endif
  const int datelen = 256;
  char plain[datelen+sizeof(void*)];
  int len = strftime (plain, datelen, "%Y-%m-%d %H:%M:%S", &theTime);
  plain[len] = 0;
  SetDate (plain);
  SetPicname (pBuf->PicName().c_str());
  SetRemark (pBuf->Remark().c_str());
}

void
ControlPanel::ReallyShowPic ()
{
  static SatPicBuf * pOldBuf(0);

  SatPicBuf *pBuf = mPicState.pBuf;
  QImage    *pI   = mPicState.pImg;
  if (pDisplay) {
    pDisplay->SetImage (pI,FrameTag(pBuf->Ident()));
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
ControlPanel::ShowImgAgain ()
{
  SatPicBuf * pBuf;
  pBuf = SatPicList::Instance()->Current(false);
  mPicState.SetNewBuf(pBuf);
  pBuf->Forget_Image();
  ShowPic(pBuf);
}

void
ControlPanel::ShowPic (SatPicBuf * pBuf)
{
  if (pBuf) {
    if (mPicState.waiting) {   // one at a time!
      return;
    }
    if (DBWaiting()) {
      return;
    }
    mPicState.SetNewBuf(pBuf);
    mPicState.waiting = true;
    QImage *pI = pBuf->Get_Image();
    if (pI == 0) {
      connect (pBuf, SIGNAL(ImageArrival(QImage *)),
               this, SLOT(PicArrive(QImage *)));
    }
    if (pI) {
      mPicState.waiting = false;
      mPicState.pImg = pI;
      ReallyShowPic();
    } else {
      ShowIndexRec(pBuf);
    }
  }

}

void
ControlPanel::ClearTrack ()
{
  if (pDisplay) {
    pDisplay->ClearTrack();
  }
}

void
ControlPanel::ClearFrame ()
{
  if (pDisplay) {
    pDisplay->ClearFrame();
  }
}

void
ControlPanel::FinishPolygon()
{
  if (pDisplay) {
    pDisplay->FinishPolygon();
  }
}

void
ControlPanel::NotImplemented ()
{
  QMessageBox msgBox;
  QTimer::singleShot(15000, &msgBox, SLOT(accept()));
  msgBox.setText("not implemented at this time");
  msgBox.exec();
}


unsigned long int
ControlPanel::EndTime(long int diff)
{
  SatPicBuf * curBuf = SatPicList::Instance()->Current();
  unsigned long end_t;
  if (curBuf) {
    end_t = curBuf->Ident() + diff;
  } else   if (diff < 0) {
    end_t = 0;
  } else {
    end_t = static_cast<unsigned long int>(~0);
  }
  return end_t;
}

void
ControlPanel::IndexWaitWakeup()
{
  mRunState.stopped = true;
  stopButton->setEnabled(false);
  SatPicList::Instance()->ToEnd();
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
    stopButton->setEnabled(DBWaiting());
  }
}

void
ControlPanel::DoStepFwd ()
{ 
  if (DBWaiting()) {
    return;
  }
  mRunState.stopped = true;
  stopButton->setEnabled(false);
  SatPicList::Instance()->PostIncr();
  SatPicBuf *pBuf = SatPicList::Instance()->Current(false);
  if (pBuf) {
    ShowPic(pBuf);
    stopButton->setEnabled(DBWaiting());
  }
}

void
ControlPanel::DoStepBack ()
{ 
  if (DBWaiting()) {
    return;
  }
  mRunState.stopped = true;
  stopButton->setEnabled(false);
  SatPicList::Instance()->PostDecr();
  SatPicBuf *pBuf = SatPicList::Instance()->Current(false);
  if (pBuf) {
    ShowPic(pBuf);
    stopButton->setEnabled(DBWaiting());
  }
}


void
ControlPanel::DoWindBack (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = false;
  mRunState.stopped = false;
  mRunState.backwards = true;
  mRunState.pBuf = SatPicList::Instance()->Current(false);
  if (allway) {
    mRunState.pBuf = SatPicList::Instance()->ToStart();
    mRunState.stopped = true;
    mRunState.show = true;
    mPicState.SetNewBuf(mRunState.pBuf);
    ShowPic (mRunState.pBuf);
    return;
  }
  if (!DBWaiting()) {
    SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
    mRunState.pBuf = pBuf;
  }
  stopButton->setEnabled(true);
  currentDelay = noshowTimeDelay;
  showTimer.setInterval(noshowTimeDelay);
}


void
ControlPanel::DoRunBack (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = true;
  mRunState.stopped = false;
  mRunState.backwards = true;
  mRunState.pBuf = SatPicList::Instance()->Current(false);
  if (!DBWaiting()) {
    SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
    if (pBuf) {
      ShowPic(pBuf);
    }
    mRunState.pBuf = pBuf;
  }
  stopButton->setEnabled(true);
  currentDelay = showTimeDelay;
  showTimer.setInterval(showTimeDelay);
}

void
ControlPanel::DoWindFwd (int secs, bool allway)
{
  unsigned long int to = EndTime (+ secs);
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = false;
  mRunState.stopped = false;
  mRunState.backwards = false;
  mRunState.pBuf = SatPicList::Instance()->Current();
  if (!DBWaiting()) {
    SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
    mRunState.pBuf = pBuf;
  }
  stopButton->setEnabled(true);
  currentDelay = noshowTimeDelay;
  showTimer.setInterval(noshowTimeDelay);
}


void
ControlPanel::DoRunFwd (int secs, bool allway)
{
  unsigned long int to = EndTime (+ secs);
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = true;
  mRunState.stopped = false;
  mRunState.backwards = false;
  mRunState.pBuf = SatPicList::Instance()->Current();
  if (!DBWaiting()) {
    SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
    if (pBuf) {
      ShowPic(pBuf);
    }
    mRunState.pBuf = pBuf;
  }
  stopButton->setEnabled(true);
  currentDelay = showTimeDelay;
  showTimer.setInterval(showTimeDelay);
}

void
ControlPanel::FwdSome ()
{
  if (DBWaiting()) {
    return;   // maybe later
  }
  unsigned long int to = mRunState.timelimit;
  bool allway = mRunState.allway;
  bool oldStopped = mRunState.stopped;
  SatPicBuf * pBuf = mRunState.pBuf;
  if  (pBuf && (allway || (pBuf->Ident() <= to))) {
    pBuf = SatPicList::Instance()->PostIncr();
    if (pBuf && mRunState.show) {
      ShowPic(pBuf);
    }
    mRunState.pBuf = pBuf;
    mRunState.stopped |= (pBuf == 0) || !(allway || (pBuf->Ident() <= to));
  } else { // fell off end
    mRunState.stopped = true;
    if (allway) {
      pBuf = SatPicList::Instance()->ToEnd();
    }
  }
  if (mRunState.stopped != oldStopped) { /* if we stopped just now */
    if (pBuf) {
      ShowPic(pBuf);
    }
    currentDelay = showTimeDelay;
    showTimer.setInterval(showTimeDelay);
  }
  stopButton->setEnabled (!mRunState.stopped || DBWaiting());
}

void
ControlPanel::BackSome ()
{
  if (DBWaiting()) {
    return;
  }
  unsigned long int to = mRunState.timelimit;
  bool allway = mRunState.allway;
  bool oldStopped = mRunState.stopped;
  SatPicBuf * pBuf = mRunState.pBuf;
  if  (pBuf && (allway || (pBuf->Ident() >= to))) {
    pBuf = SatPicList::Instance()->PostDecr();
    if (pBuf && mRunState.show) {
      ShowPic(pBuf);
    }
    mRunState.pBuf = pBuf;
    mRunState.stopped |= (pBuf == 0) || !(allway || (pBuf->Ident() >= to));
  } else { // fell off end
    mRunState.stopped = true;
    if (allway || pBuf == 0) {
      pBuf = SatPicList::Instance()->ToStart();
    }
  }
  if (mRunState.stopped != oldStopped) { /* if we stopped just now */
    if (pBuf) {
      ShowPic(pBuf);
    }
    showTimer.setInterval(showTimeDelay);
  }
  stopButton->setEnabled (!mRunState.stopped || DBWaiting());
}

void
ControlPanel::DoStopMoving ()
{
  mRunState.stopped = true;
  stopButton->setEnabled(true);
}

void
ControlPanel::DoShowMove ()
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
ControlPanel::ConnectDB ()
{
  NotImplemented();
}

string
ControlPanel::GetBoxString (QPlainTextEdit *pBox)
{
  QString newval = pBox->toPlainText();
  QByteArray bytes = newval.toLatin1();
  return string(bytes.data());
}

void
ControlPanel::ReloadDB ()
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
    DoWindFwd(0,true);
    DoStepFwd();
  }
  update();
}

void
ControlPanel::DoSwitchPicname ()
{
  mPicname = "?";
  QString newval = picnameBox->toPlainText();
  QByteArray bytes = newval.toLatin1();
  mPicname = string(bytes.data());
  SatPicList::Instance()->SetFilename(mPicname);
  SatPicList::Instance()->Ditch();
  SatPicList::Instance()->LoadFromDB();
  DoWindFwd(0,true);
  DoStepFwd();
  update();
}

string
ControlPanel::Server ()
{
  return mServer;
}

string
ControlPanel::SetServer (string sv)
{
  QString newname(sv.c_str());
  serverBox->clear();
  serverBox->setPlainText(newname);
  mServer = sv;
  return mServer;
}

string
ControlPanel::Picname ()
{
  return mPicname;
}

string
ControlPanel::SetPicname (string pn)
{
  QString newname(pn.c_str());
  picnameBox->clear();
  picnameBox->setPlainText(newname);
  mPicname = pn;
  return mPicname;
}

string
ControlPanel::Remark ()
{
  return mRemark;
}

string
ControlPanel::SetRemark (string rm)
{
  QString newremark(rm.c_str());
  commentBox->clear();
  commentBox->setPlainText(newremark);
  mRemark = rm;
  return mRemark;
}

string
ControlPanel::IdentTag ()
{
  return mIdFancy;
}

string
ControlPanel::SetIdentTag (string id)
{
  /** should update the idtagBox field */
  idtagBox->clear();
  QString newtext (id.c_str());
  idtagBox->setPlainText(newtext);
  mIdFancy = id;
  return mIdFancy;
}

string
ControlPanel::Date ()
{
  return mDate;
}

string
ControlPanel::SetDate (string dt)
{
  QString newdate(dt.c_str());
  dateBox->clear();
  dateBox->setPlainText(newdate);
  mDate = dt;
  return mDate;
}

string
ControlPanel::SetMinAge (unsigned long int ma)
{
  return SetMinAge(berndsutil::toString(ma));
}

string
ControlPanel::SetMinAge (string ma)
{
  mMinHours = ma;
  QString age(ma.c_str());
  minHoursBox->clear();
  minHoursBox->setPlainText(age);
  return mMinHours;
}

string
ControlPanel::MinAge ()
{
  return mMinHours;
}

string
ControlPanel::SetMaxAge (unsigned long int ma)
{
  return SetMaxAge (berndsutil::toString(ma));
}

string 
ControlPanel::SetMaxAge (string ma)
{
  mMaxHours = ma;
  QString age(ma.c_str());
  maxHoursBox->clear();
  maxHoursBox->setPlainText(age);
  return mMaxHours;
}

string
ControlPanel::MaxAge ()
{
  return mMaxHours;
}

string
ControlPanel::SetConMeth (string cm)
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
ControlPanel::ConMeth ()
{
  return mConMeth;
}

void
ControlPanel::SetIFLabel ()
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

void
ControlPanel::ToggleConn ()
{
  switch (mMeth) {
  case DBConnection::Con_Web:
    mMeth = DBConnection::Con_MySql;
    break;
  case DBConnection::Con_MySql:
  default:
    mMeth = DBConnection::Con_Web;
    break;
  }
  SetIFLabel();
  QByteArray bytes = interfaceLabel->text().toLatin1();
  mConMeth = string(bytes.data());
  update();
}

}
