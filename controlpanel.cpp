
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

using namespace std;

namespace satview  {

ControlPanel::ControlPanel (QApplication *pA)
  :mMeth(DBConnection::Con_None)
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
    SetIFLabel();
    staticDraw->setChecked(true);
    staticDraw->setEnabled(false);

    /** @brief the runstate is context for the animation and winding features*/
    mRunState.timelimit = 0;
    mRunState.allway = false;
    mRunState.show   = true;
    mRunState.backwards = false;
    mRunState.stopped = true;
    runStatusLabel->setText (mStateStoppedText);
    showTimeDelay = 100;
    noshowTimeDelay = 1;
    currentDelay = noshowTimeDelay;
    connect (&showTimer, SIGNAL(timeout()), this, SLOT(DoShowMove()));
    showTimer.start(currentDelay);

    /** @brief make sure we check for clicked buttons every now and then */
    updateDelay = 300;
    connect (&updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer.start(updateDelay);

    /** and we have a whole bunch of buttons to connect */

    connect (versionButton, SIGNAL(clicked()), this, SLOT(ShowVersion()));
    connect (authorButton, SIGNAL(clicked()), this, SLOT(LinkToBernd()));

    connect (newNameButton, SIGNAL(clicked()), this, SLOT(DoSwitchPicname()));
    connect (quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    
    connect (stopButton, SIGNAL(clicked()), this, SLOT(DoStopMoving()));
    stopButton->setEnabled(false);

    connect (runBackButton, SIGNAL(clicked()), this, SLOT(DoRunBack()));
    connect (backWeekButton, SIGNAL(clicked()), this, SLOT(DoRunBackWeek()));
    connect (backDayButton, SIGNAL(clicked()), this, SLOT(DoRunBackDay()));
    connect (backHoursButton, SIGNAL(clicked()), this, SLOT(DoRunBackHours()));
    connect (backStepButton, SIGNAL(clicked()), this, SLOT(DoStepBack()));
    connect (runForwardButton, SIGNAL(clicked()), this, SLOT(DoRunFwd()));
    connect (forwardWeekButton, SIGNAL(clicked()), this, SLOT(DoRunFwdWeek()));
    connect (forwardDayButton, SIGNAL(clicked()), this, SLOT(DoRunFwdDay()));
    connect (forwardHoursButton, SIGNAL(clicked()), this, SLOT(DoRunFwdHours()));
    connect (forwardStepButton, SIGNAL(clicked()), this, SLOT(DoStepFwd()));
    connect (windBackWeekButton, SIGNAL(clicked()), this, SLOT(DoWindBackWeek()));
    connect (windBackDayButton, SIGNAL(clicked()), this, SLOT(DoWindBackDay()));
    connect (windBackHoursButton, SIGNAL(clicked()), this, SLOT(DoWindBackHours()));
    connect (windFwdWeekButton, SIGNAL(clicked()), this, SLOT(DoWindFwdWeek()));
    connect (windFwdDayButton, SIGNAL(clicked()), this, SLOT(DoWindFwdDay()));
    connect (windFwdHoursButton, SIGNAL(clicked()), this, SLOT(DoWindFwdHours()));

    connect (connectServerButton, SIGNAL(clicked()), this, SLOT(ReloadDB()));
    connect (reloadServerButton, SIGNAL(clicked()), this, SLOT(ReloadDB()));
    connect (changeInterfaceButton, SIGNAL(clicked()),
	     this, SLOT(ToggleConn()));

    connect (clearTrackButton, SIGNAL(clicked()), this, SLOT(ClearTrack()));
    connect (clearFrameButton, SIGNAL(clicked()), this, SLOT(ClearFrame()));
    connect (finishPolygonButton, SIGNAL(clicked()),
	     this, SLOT(FinishPolygon()));
    connect (saveFrameButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));

}

ControlPanel::~ControlPanel()
{
  quit();
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
ControlPanel::update()
{
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
ControlPanel::ShowStatus ()
{
  string statusmsg = string("Status:\n") + mServer + "\r " + mConMeth;
  QMessageBox msg;
  QTimer::singleShot(15000,&msg, SLOT(accept()));
  msg.setText (statusmsg.c_str());
  msg.exec();
}

void
ControlPanel::ShowPic (SatPicBuf * pBuf)
{
  static SatPicBuf * pOldBuf(0);
  if (pBuf) {
    QImage *pI = pBuf->Get_Image();
    if (pI) {
      
      if (pDisplay) {
        pDisplay->SetImage (pI,FrameTag(pBuf->Ident()));
        pDisplay->update();
      }
     
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
      update();
      if (pApp) {
        pApp->processEvents();
      }
      if (pOldBuf) {
        if (pOldBuf != pBuf) {
          pOldBuf->Forget_Image();
          pOldBuf = pBuf;
	}
      }     
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

void
ControlPanel::StartImage ()
{
  pDisplay = new ImageWin(this);
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
ControlPanel::DoStepFwd ()
{ 
  mRunState.stopped = true;
  stopButton->setEnabled(false);
  SatPicList::Instance()->Skip(1);
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void
ControlPanel::DoStepBack ()
{ 
  mRunState.stopped = true;
  stopButton->setEnabled(false);
  SatPicList::Instance()->Skip(-1);
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
  }
}


void
ControlPanel::DoWindBack (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = false;
  mRunState.stopped = false;
  mRunState.backwards = true;
  mRunState.pBuf = pBuf;
  stopButton->setEnabled(true);
  currentDelay = noshowTimeDelay;
  showTimer.setInterval(noshowTimeDelay);
}


void
ControlPanel::DoRunBack (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
  if (pBuf) {
    ShowPic(pBuf);
  }
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = true;
  mRunState.stopped = false;
  mRunState.backwards = true;
  mRunState.pBuf = pBuf;
  stopButton->setEnabled(true);
  currentDelay = showTimeDelay;
  showTimer.setInterval(showTimeDelay);
}

void
ControlPanel::DoWindFwd (int secs, bool allway)
{
  unsigned long int to = EndTime (+ secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = false;
  mRunState.stopped = false;
  mRunState.backwards = false;
  mRunState.pBuf = pBuf;
  stopButton->setEnabled(true);
  currentDelay = noshowTimeDelay;
  showTimer.setInterval(noshowTimeDelay);
}


void
ControlPanel::DoRunFwd (int secs, bool allway)
{
  unsigned long int to = EndTime (+ secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  if (pBuf) {
    ShowPic(pBuf);
  }
  mRunState.timelimit = to;
  mRunState.allway = allway;
  mRunState.show = true;
  mRunState.stopped = false;
  mRunState.backwards = false;
  mRunState.pBuf = pBuf;
  stopButton->setEnabled(true);
  currentDelay = showTimeDelay;
  showTimer.setInterval(showTimeDelay);
}

void
ControlPanel::FwdSome ()
{
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
      SatPicList::Instance()->ToEnd();
    }
  }
  if (mRunState.stopped != oldStopped) { /* if we stopped just now */
    if (pBuf) {
      ShowPic(pBuf);
    }
    currentDelay = showTimeDelay;
    showTimer.setInterval(showTimeDelay);
  }
  stopButton->setEnabled(!mRunState.stopped);
}

void
ControlPanel::BackSome ()
{
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
      SatPicList::Instance()->Rewind();
    }
  }
  if (mRunState.stopped != oldStopped) { /* if we stopped just now */
    if (pBuf) {
      ShowPic(pBuf);
    }
    showTimer.setInterval(showTimeDelay);
  }
  stopButton->setEnabled(!mRunState.stopped);
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
  runStatusLabel->setText (mRunState.stopped ?
                             mStateStoppedText
			   : mStateRunningText);
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
  if (!loadedok) {
     QMessageBox msgBox;
     QTimer::singleShot(15000, &msgBox, SLOT(accept()));
     string badmsg = "Cannot Load from Server '" + mServer
       + "' using Method '" + mConMeth + "'";
     msgBox.setText(badmsg.c_str());
     msgBox.exec();
  }
  DoWindFwd(0,true);
  DoStepFwd();
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
ControlPanel::SetConMeth (string cm)
{
  bool is_dir = (cm == "dir");
  mConMeth = cm;
  mMeth = DBConnection::Con_WebSock;    // safer
  if (is_dir) {
    mMeth = DBConnection::Con_MySqlCPP;
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
  case DBConnection::Con_WebSock:
    newlabel = mMethWebLabel;
    break;
  case DBConnection::Con_MySqlCPP:
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
  case DBConnection::Con_WebSock:
    mMeth = DBConnection::Con_MySqlCPP;
    break;
  case DBConnection::Con_MySqlCPP:
  default:
    mMeth = DBConnection::Con_WebSock;
    break;
  }
  SetIFLabel();
  QByteArray bytes = interfaceLabel->text().toLatin1();
  mConMeth = string(bytes.data());
  update();
}

}
