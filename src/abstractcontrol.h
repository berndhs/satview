#ifndef ABSTRACTCONTROL_H
#define ABSTRACTCONTROL_H
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

#include "imagewin.h"
#include "dbconnect.h"
#include "satpiclist.h"
#include <QObject>
#include <QWidget>
#include <QApplication>
#include "delib-debug.h"

namespace satview {

class AbstractControl : public QObject {

Q_OBJECT

public:

  AbstractControl ();
  
  bool Ready () { return  ready; }
  
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
    
  void Reset ();
  void Restart ();
  
  
  bool DBWaiting ();
  
  virtual void ShowVersion();
  virtual void LinkToBernd();
  
  virtual string TimeAndDate (time_t t);
  virtual void CannotLoadMsg ();

  
public slots:


    void update();
    void quit ();
    void show();
    void ConnectDB ();
    void ReloadDB ();
    void DoSwitchPicname(QByteArray & bytes);
    
    void DoStepFwd ();
    void DoStepBack ();
    void DoShowMove ();

    void DoStopMoving ();
    
    void IndexWaitWakeup();
    
    void PicArrive (QImage * pImg);
    
signals:

    void ReallyShowPic (QImage * pImg);
    void ShowIndexRec  (SatPicBuf * pBuf);
    void IndexArrived  ();
    void ShowMove ();
    
protected:

    void StartTimers ();
    void ShowPic (SatPicBuf *pBuf);
    void FwdSome   ();
    void BackSome  ();

    unsigned long int EndTime (long int diff);

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
           #if DELIBERATE_DEBUG
           qDebug() << " run state time limit " << timelimit;
           qDebug() << "     all " << allway << " show " << show 
                    << " back " << backwards << " stopped " << stopped;
           qDebug () << "    pBuf " << pBuf;
           #endif
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
                 #if DELIBERATE_DEBUG
                 qDebug() << "-- Pic State-- ";
                 qDebug() << "pBuf " << pBuf;
                 qDebug() << "pImg " << pImg;
                 qDebug() << "waiting " << waiting;
                 qDebug() << "failed " << failed;
                 #endif
                 }
    };

  
    QApplication  * pApp;

    QTimer        showTimer;
    int           showTimeDelay;
    int           noshowTimeDelay;
    int           currentDelay;
    

    RunState               mRunState;
    ShowPicState           mPicState;
    string                 mPicname;
    string                 mServer;
    string                 mConMeth;
    bool                   ready;
    string                 mDate;
    string                 mIdFancy;
    DBConnection::Method   mMeth;
    string                 mRemark;
    string                 mMinHours;
    string                 mMaxHours;

};

}


#endif
