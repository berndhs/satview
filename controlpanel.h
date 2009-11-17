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
#include "imagewin.h"
#include "dbconnect.h"
#include <QApplication>
#include <string>

using namespace std;

namespace satview {

  /** @defgroup UI
   * User Interface - graphics, buttons and how they interface with the
   * rest of the programs.
   */

  class ControlPanel : public QDialog, public Ui_ControlPanel {

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

    void ShowStatus ();

    void Wait (double secs);

  public slots:

    void NotImplemented ();
    void quit ();
    void show();

    void ConnectDB ();
    void ReloadDB ();
    void DoSwitchPicname();

    void DoStepFwd ();
    void DoStepBack ();

    void ClearTrack ();

    void DoRunBack ()      { DoRunBack (0,true); }
    void DoRunBackWeek ()  { DoRunBack (7*24*3600); }
    void DoRunBackDay ()   { DoRunBack (24*3600);   }
    void DoRunBackHours () { DoRunBack (6*3600);   }

    void DoRunFwd ()      { DoRunFwd (0,true); }
    void DoRunFwdWeek ()  { DoRunFwd (7*24*3600); }
    void DoRunFwdDay ()   { DoRunFwd (24*3600);   }
    void DoRunFwdHours () { DoRunFwd (6*3600);   }

    void DoWindBack ()      { DoWindBack (0,true); }
    void DoWindBackWeek ()  { DoWindBack (7*24*3600); }
    void DoWindBackDay ()   { DoWindBack (24*3600);   }
    void DoWindBackHours () { DoWindBack (6*3600);   }

    void DoWindFwd ()      { DoWindFwd (0,true); }
    void DoWindFwdWeek ()  { DoWindFwd (7*24*3600); }
    void DoWindFwdDay ()   { DoWindFwd (24*3600);   }
    void DoWindFwdHours () { DoWindFwd (6*3600);   }

    void ToggledConn(bool is_checked);


  private:

    void StartImage ();
    void ShowPic (SatPicBuf *pBuf);
    string GetBoxString (QPlainTextEdit * pBox);

    unsigned long int EndTime (long int diff);

    QApplication  * pApp;

    ImageWin      * pDisplay;

    string                 mPicname;
    string                 mServer;
    string                 mConMeth;
    string                 mDate;
    string                 mIdFancy;
    DBConnection::Method   mMeth;

  };


}

#endif
