#include "abstractcontrol.h"

//
//  Copyright (C) 2017 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//



#include <QTimer>
#include <QString>
#include <QByteArray>
#include <QDesktopServices>
#include <QMessageBox>
#include "version.h"

using namespace std;

namespace satview  {


AbstractControl::AbstractControl ()
:pApp(0),
 ready(false),
 mMeth(DBConnection::Con_None)
{
    ready  = false;

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

    connect (&showTimer, SIGNAL(timeout()), this, SLOT(DoShowMove()));

    
    StartTimers ();
    
    ready = true;

}

void
AbstractControl::StartTimers ()
{
  showTimeDelay = 100;
  noshowTimeDelay = 1;
  currentDelay = noshowTimeDelay;
  showTimer.start(currentDelay);
}

void
AbstractControl::ShowVersion ()
{
  satview::ShowVersionWindow ();
}

void
AbstractControl::quit()
{
  if (pApp) {
    pApp->quit();
  }
}

void
AbstractControl::show()
{
}

bool
AbstractControl::DBWaiting()
{
  return SatPicList::Instance()->DBConnect()->Waiting();
}

void
AbstractControl::LinkToBernd()
{
  QDesktopServices::openUrl
    (QUrl(QString("http://www.bernd-stramm.com/freestuff.php")));
}

void
AbstractControl::PicArrive (QImage * pImg)
{
  mPicState.pImg = pImg;
  mPicState.waiting = false;
  emit ReallyShowPic(pImg);
}

string
AbstractControl::TimeAndDate (time_t t)
{
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
  return string (plain);
}

void
AbstractControl::DoSwitchPicname (QByteArray & bytes)
{
  mPicname = string(bytes.data());
  SatPicList::Instance()->SetFilename(mPicname);
  SatPicList::Instance()->Ditch();
  SatPicList::Instance()->LoadFromDB();
  AbstractControl::DoWindFwd(0,true);
  AbstractControl::DoStepFwd();
  update();
}

void
AbstractControl::ShowPic (SatPicBuf * pBuf)
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
      emit ReallyShowPic(mPicState.pImg);
    } else {
      emit ShowIndexRec(pBuf);
    }
  }

}
unsigned long int
AbstractControl::EndTime(long int diff)
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
AbstractControl::IndexWaitWakeup()
{
  mRunState.stopped = true;
  SatPicList::Instance()->ToEnd();
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
    emit IndexArrived ();
  }
}

void
AbstractControl::DoStepFwd ()
{ 
  if (DBWaiting()) {
    return;
  }
  mRunState.stopped = true;
  SatPicList::Instance()->PostIncr();
  SatPicBuf *pBuf = SatPicList::Instance()->Current(false);
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void
AbstractControl::DoStepBack ()
{ 
  if (DBWaiting()) {
    return;
  }
  mRunState.stopped = true;
  SatPicList::Instance()->PostDecr();
  SatPicBuf *pBuf = SatPicList::Instance()->Current(false);
  if (pBuf) {
    ShowPic(pBuf);
  }
}


void
AbstractControl::DoWindBack (int secs, bool allway)
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
  currentDelay = noshowTimeDelay;
  showTimer.setInterval(noshowTimeDelay);
}


void
AbstractControl::DoRunBack (int secs, bool allway)
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
  currentDelay = showTimeDelay;
  showTimer.setInterval(showTimeDelay);
}

void
AbstractControl::DoWindFwd (int secs, bool allway)
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
  currentDelay = noshowTimeDelay;
  showTimer.setInterval(noshowTimeDelay);
}


void
AbstractControl::DoRunFwd (int secs, bool allway)
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
  currentDelay = showTimeDelay;
  showTimer.setInterval(showTimeDelay);
}

void
AbstractControl::FwdSome ()
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
}

void
AbstractControl::BackSome ()
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
}

void
AbstractControl::DoStopMoving ()
{
  mRunState.stopped = true;
}

void
AbstractControl::DoShowMove ()
{
  if (!mRunState.stopped) {
    showTimer.stop();     /** don't want to get called twice */
    if (mRunState.backwards) {
      BackSome();
    } else {
      FwdSome();
    }
    showTimer.start(currentDelay);
    if (mRunState.show) {
      emit ShowMove ();
    } 
  }
}

void
AbstractControl::ConnectDB ()
{
}


void
AbstractControl::CannotLoadMsg ()
{
  QMessageBox msgBox;
  QTimer::singleShot(15000, &msgBox, SLOT(accept()));
  string badmsg = "Cannot Load from A-Server '" + mServer
     + "' using Method '" + mConMeth + "'";
  msgBox.setText(badmsg.c_str());
  msgBox.exec();
}

void
AbstractControl::ReloadDB ()
{
  SatPicList::Instance()->SetServer(mServer);
  SatPicList::Instance()->SetFilename(mPicname);
  SatPicList::Instance()->SetMethod(mMeth);
  SatPicList::Instance()->Ditch();
  bool loadedok = SatPicList::Instance()->LoadFromDB();
  if (!loadedok && !DBWaiting()) {
    CannotLoadMsg ();
  }
  if (!DBWaiting()) {
    DoWindFwd(0,true);
    DoStepFwd();
  }
  update();
}

string
AbstractControl::Server ()
{
  return mServer;
}

string
AbstractControl::SetServer (string sv)
{
  QString newname(sv.c_str());
  mServer = sv;
  return mServer;
}

string
AbstractControl::Picname ()
{
  return mPicname;
}

string
AbstractControl::SetPicname (string pn)
{
  QString newname(pn.c_str());
  mPicname = pn;
  return mPicname;
}

string
AbstractControl::Remark ()
{
  return mRemark;
}

string
AbstractControl::SetRemark (string rm)
{
  QString newremark(rm.c_str());
  mRemark = rm;
  return mRemark;
}

string
AbstractControl::IdentTag ()
{
  return mIdFancy;
}

string
AbstractControl::SetIdentTag (string id)
{
  QString newtext (id.c_str());
  mIdFancy = id;
  return mIdFancy;
}

string
AbstractControl::Date ()
{
  return mDate;
}

string
AbstractControl::SetDate (string dt)
{
  QString newdate(dt.c_str());
  mDate = dt;
  return mDate;
}

string
AbstractControl::SetMinAge (unsigned long int ma)
{
  return SetMinAge(berndsutil::toString(ma));
}

string
AbstractControl::SetMinAge (string ma)
{
  mMinHours = ma;
  QString age(ma.c_str());
  return mMinHours;
}

string
AbstractControl::MinAge ()
{
  return mMinHours;
}

string
AbstractControl::SetMaxAge (unsigned long int ma)
{
  return SetMaxAge (berndsutil::toString(ma));
}

string 
AbstractControl::SetMaxAge (string ma)
{
  mMaxHours = ma;
  QString age(ma.c_str());

  return mMaxHours;
}

string
AbstractControl::MaxAge ()
{
  return mMaxHours;
}

string
AbstractControl::SetConMeth (string cm)
{
  bool is_dir = (cm == "dir");
  mConMeth = cm;
  mMeth = DBConnection::Con_Web;    // safer
  if (is_dir) {
    mMeth = DBConnection::Con_MySql;
  }
  return mConMeth;
}

string
AbstractControl::ConMeth ()
{
  return mConMeth;
}

void
AbstractControl::update ()
{
}


}
