#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

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

#include "ui_control.h"
#include "ui_singlewin.h"
#include "imagewin.h"
#include "dbconnect.h"
#include "satpiclist.h"
#include <QApplication>
#include <QTimer>
#include <QPixmap>
#include <QDebug>
#include <string>

using namespace std;

namespace satview {

  /** @defgroup UI
   * User Interface - graphics, buttons and how they interface with the
   * rest of the programs.
   */

  class ControlPanel : public QDialog, 
  #if SATVIEW_SINGLEWIN
                       public Ui_SingleWin 
  #else
                       public Ui_ControlPanel 
  #endif
                       {

Q_OBJECT

  public:

    ControlPanel (QApplication *pA);
    ~ControlPanel();

    void DoRunBack (int secs, bool allway=false);
    void DoRunFwd  (int secs, bool allway=false);
    void DoWindBack (int secs, bool allway=false);
    void DoWindFwd (int secs, bool allway=false);   

    string Server();
    string SetServer (string sv);
    string Picname();
    string SetPicname (string pn);
    string ConMeth();
    string SetConMeth (string cm);
    string IdentTag();
    string SetIdentTag (string id);
    string Date();
    string SetDate (string dt);
    string Remark();
    string SetRemark (string rm);
    string MinAge ();
    string SetMinAge (string ma);
    string SetMinAge (unsigned long int ma);
    string MaxAge ();
    string SetMaxAge (string ma);
    string SetMaxAge (unsigned long int ma);
    
    bool   Again() { return wantRerun; }

    void Reset ();
    void Restart ();

    void ShowStatus ();


  public slots:

    void ShowVersion();
    void LinkToBernd();
    
    void ChangeSettings ();

    void NotImplemented ();
    void update();
    void quit ();
    void show();
    void DoAgain (bool doit = true);
    
    void ShowImgAgain ();

    void ConnectDB ();
    void ReloadDB ();
    void DoSwitchPicname();

    void DoStepFwd ();
    void DoStepBack ();
    void DoShowMove ();

    void DoStopMoving ();

    void ClearTrack ();
    void ClearFrame ();
    void FinishPolygon ();

    void DoRunBack ()      { DoRunBack (0,true); }
    void DoRunBackWeek ()  { DoRunBack (7*24*3600); }
    void DoRunBackDay ()   { DoRunBack (24*3600);   }

    void DoRunFwd ()      { DoRunFwd (0,true); }
    void DoRunFwdWeek ()  { DoRunFwd (7*24*3600); }
    void DoRunFwdDay ()   { DoRunFwd (24*3600);   }

    void DoWindBack ()      { DoWindBack (0,true); }
    void DoWindBackWeek ()  { DoWindBack (7*24*3600); }
    void DoWindBackDay ()   { DoWindBack (24*3600);   }
 
    void DoWindFwd ()      { DoWindFwd (0,true); }
    void DoWindFwdWeek ()  { DoWindFwd (7*24*3600); }
    void DoWindFwdDay ()   { DoWindFwd (24*3600);   }
 
    void ToggleConn();
    void IndexWaitWakeup();

    void PicArrive (QImage *pImg);


  private:

    class RunState {
      public:
      unsigned long int  timelimit;
      bool               allway;
      bool               show;
      bool               backwards;
      bool               stopped;
      SatPicBuf         *pBuf;
      RunState(): timelimit(0),
                  allway(false),
                  show(false),
                  backwards(false),
                  stopped(false),
                  pBuf(0)
                  {}
      void Debug() {
           qDebug() << " run state time limit " << timelimit;
           qDebug() << "     all " << allway << " show " << show 
                    << " back " << backwards << " stopped " << stopped;
           qDebug () << "    pBuf " << pBuf;
      }
    };

    class ShowPicState {
      public:
      SatPicBuf         *pBuf;
      QImage            *pImg;
      bool               waiting;
      bool               failed;
      ShowPicState (): pBuf(0),pImg(0),waiting(false),failed(false){}
      void SetNewBuf (SatPicBuf *pB) {
                 pBuf = pB; pImg = 0; 
                 waiting = false; failed=false;
                 }
      void Debug () { 
                 qDebug() << "-- Pic State-- ";
                 qDebug() << "pBuf " << pBuf;
                 qDebug() << "pImg " << pImg;
                 qDebug() << "waiting " << waiting;
                 qDebug() << "failed " << failed;
                 }
    };

    void StartTimers ();
    void ConnectButtons ();
    void ShowPic (SatPicBuf *pBuf);
    void ShowIndexRec (SatPicBuf *pBuf);
    void ReallyShowPic();
    
    void ChangeStatusLabel (QString status);

    bool DBWaiting ();

    string GetBoxString (QPlainTextEdit * pBox);
    void SetIFLabel ();
   

    void FwdSome   ();
    void BackSome  ();

    unsigned long int EndTime (long int diff);

    QApplication  * pApp;

#if SATVIEW_SINGLEWIN
    ImageWin      * pStaticDisplay;
#else
    ImageWin      * pDisplay;
#endif


    QTimer        updateTimer;
    int           updateDelay;
    QTimer        showTimer;
    int           showTimeDelay;
    int           noshowTimeDelay;
    int           currentDelay;
    bool          wantRerun;
    

    RunState               mRunState;
    ShowPicState           mPicState;
    string                 mPicname;
    string                 mServer;
    string                 mConMeth;
    QString                mMethWebLabel;
    QString                mMethDirLabel;
    QString                mStateRunningText;
    QPixmap                mStateRunningPix;
    QString                mStateStoppedText;
    QPixmap                mStateStoppedPix;
    QString                mStateLoadingText;
    QPixmap                mStateLoadingPix;
    
    QPixmap                imagePix;
    
    string                 mDate;
    string                 mIdFancy;
    DBConnection::Method   mMeth;
    string                 mRemark;
    string                 mMinHours;
    string                 mMaxHours;

  };


}

#endif
