
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
#include <unistd.h>
#include "satpiclist.h"
#include "satpicbuf.h"

using namespace std;

namespace satview  {

ControlPanel::ControlPanel (QApplication *pA)
{
    pApp = pA;

    setupUi(this);

    pDisplay = new ImageWin(this);

    connect (newNameButton, SIGNAL(clicked()), this, SLOT(DoSwitchPicname()));
    connect (quitButton, SIGNAL(clicked()), this, SLOT(quit()));
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
    connect (windBackButton, SIGNAL(clicked()), this, SLOT(DoWindBack()));
    connect (windBackWeekButton, SIGNAL(clicked()), this, SLOT(DoWindBackWeek()));
    connect (windBackDayButton, SIGNAL(clicked()), this, SLOT(DoWindBackDay()));
    connect (windBackHoursButton, SIGNAL(clicked()), this, SLOT(DoWindBackHours()));
    connect (windFwdButton, SIGNAL(clicked()), this, SLOT(DoWindFwd()));
    connect (windFwdWeekButton, SIGNAL(clicked()), this, SLOT(DoWindFwdWeek()));
    connect (windFwdDayButton, SIGNAL(clicked()), this, SLOT(DoWindFwdDay()));
    connect (windFwdHoursButton, SIGNAL(clicked()), this, SLOT(DoWindFwdHours()));
    connect (clearAllButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (connectServerButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (reloadServerButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (directConnButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (webConnButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));

}

ControlPanel::~ControlPanel()
{
  quit();
}

void
ControlPanel::quit()
{
  if (pDisplay) {
    pDisplay->quit();
  }
  if (pApp) {
    pApp->quit();
  }
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
ControlPanel::Wait(double secs)
{
  usleep(secs * 1000 * 1000);
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
        pDisplay->ShowImage (pI);
        pDisplay->update();
      }
     
      time_t t = pBuf->Ident();
      SetIdentTag((berndsutil::toString(t)
                         + string(" - ")
                         + berndsutil::toString(pBuf->Serial())).c_str() 
                        );
      struct tm theTime;
#ifdef _MSC_VER
	  struct tm *pTM;
	  pTM = localtime(&t);
	  memcpy (&theTime,pTM,sizeof(theTime));
#else
      localtime_r (&t, &theTime);
#endif
      const int datelen = 256;
      char plain[datelen+sizeof(void*)];
      int len = strftime (plain, datelen,"%c", &theTime);
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
ControlPanel::NotImplemented ()
{
  cout << "not implemented" << endl;
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
  SatPicList::Instance()->Skip(1);
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void
ControlPanel::DoStepBack ()
{ 
  SatPicList::Instance()->Skip(-1);
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
  }
}



void
ControlPanel::DoRunBack (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
  while (pBuf && (allway || (pBuf->Ident() >= to))) {
    ShowPic(pBuf);
    pBuf = SatPicList::Instance()->PostDecr();
  }
  if (!pBuf) { // fell off end
    DoWindBack (0, true);
  }
}

void
ControlPanel::DoRunFwd (int secs, bool allway)
{
  unsigned long int to = EndTime (secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  while (pBuf && (allway || (pBuf->Ident() <= to))) {
    ShowPic(pBuf);
    pBuf = SatPicList::Instance()->PostIncr();
  }
  if (!pBuf) { // fell off end
    DoWindFwd (0, true);
  }
}

void
ControlPanel::DoWindBack (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
  while (pBuf && (allway || (pBuf->Ident() >= to))) {
    pBuf = SatPicList::Instance()->PostDecr();
  }
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void
ControlPanel::DoWindFwd (int secs, bool allway)
{
  unsigned long int to = EndTime ( - secs);
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  while (pBuf && (allway || (pBuf->Ident() <= to))) {
    pBuf = SatPicList::Instance()->PostIncr();
  }
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void
ControlPanel::ConnectDB ()
{
  NotImplemented();
}

void
ControlPanel::ReloadDB ()
{
  NotImplemented();
}

void
ControlPanel::DoSwitchPicname ()
{
  NotImplemented();
}

string
ControlPanel::Server ()
{
  return mServer;
}

string
ControlPanel::SetServer (string sv)
{
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
  mPicname = pn;
  return mPicname;
}

string
ControlPanel::IdentTag ()
{
  return string("0");
}

string
ControlPanel::SetIdentTag (string id)
{
  return string("0");
}

string
ControlPanel::Date ()
{
  return mDate;
}

string
ControlPanel::SetDate (string dt)
{
  mDate = dt;
  return mDate;
}

string
ControlPanel::SetConMeth (string cm)
{
  mConMeth = cm;
  return mConMeth;
}

string
ControlPanel::ConMeth ()
{
  return mConMeth;
}

}
