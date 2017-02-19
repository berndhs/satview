#ifndef TWOPANELCONTROL_H
#define TWOPANELCONTROL_H

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

#include "abstractcontrol.h"
#include "settingsmenu.h"
#include "ui_control.h"
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

  class TwoPanelControl : public AbstractControl, public Ui_ControlPanel 
                       {

Q_OBJECT

  public:

    TwoPanelControl (QGuiApplication *pA);
    ~TwoPanelControl ();
 

    void ShowStatus ();


  public slots:

    
    void ChangeSettings ();

    void NotImplemented ();
    void update();
    void quit ();
    void show();
    
    string SetConMeth (string cm);
    string ConMeth ();
    
    void ShowImgAgain ();

    void ConnectDB ();
    void ReloadDB ();
    void DoSwitchPicname();


    void ClearTrack ();
    void ClearFrame ();
    void FinishPolygon ();

    void DoRunBack ()      { AbstractControl::DoRunBack (0,true); }
    void DoRunBackWeek ()  { AbstractControl::DoRunBack (7*24*3600); }
    void DoRunBackDay ()   { AbstractControl::DoRunBack (24*3600);   }

    void DoRunFwd ()      { AbstractControl::DoRunFwd (0,true); }
    void DoRunFwdWeek ()  { AbstractControl::DoRunFwd (7*24*3600); }
    void DoRunFwdDay ()   { AbstractControl::DoRunFwd (24*3600);   }

    void DoWindBack ()      { AbstractControl::DoWindBack (0,true); }
    void DoWindBackWeek ()  { AbstractControl::DoWindBack (7*24*3600); }
    void DoWindBackDay ()   { AbstractControl::DoWindBack (24*3600);   }
 
    void DoWindFwd ()      { AbstractControl::DoWindFwd (0,true); }
    void DoWindFwdWeek ()  { AbstractControl::DoWindFwd (7*24*3600); }
    void DoWindFwdDay ()   { AbstractControl::DoWindFwd (24*3600);   }

    void DisplayPic (QImage *pImg);
    void DoShowMove ();


   void ShowVersion();
   void LinkToBernd();
   
  
  private:

    void TwoStartTimers ();
    void ConnectButtons ();
    void ShowIndexRec (SatPicBuf *pBuf);
    
    void ChangeStatusLabel (QString status);

    string GetBoxString (QPlainTextEdit * pBox);
    void SetIFLabel ();
   


    ImageWin      * pDisplay;
    
    QWidget       *myWindow;
    
    
    QTimer        updateTimer;
    int           updateDelay;

    QString                mMethWebLabel;
    QString                mMethDirLabel;
    QString                mStateRunningText;
    QPixmap                mStateRunningPix;
    QString                mStateStoppedText;
    QPixmap                mStateStoppedPix;
    QString                mStateLoadingText;
    QPixmap                mStateLoadingPix;
    
    QPixmap                imagePix;
    

  };


}

#endif
