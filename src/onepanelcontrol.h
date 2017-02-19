#ifndef ONEPANELCONTROL_H
#define ONEPANELCONTROL_H

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


#include "abstractcontrol.h"
#include "settingsmenu.h"
#include "settingserver.h"
#include "textbox.h"
#include "ui_onepanel.h"
#include <QPixmap>
#include <QAction>
#include <QMenuBar>
#include <QMainWindow>

namespace satview {

class OnePanelControl: public AbstractControl, public Ui_OnePanel {

Q_OBJECT

public:

  OnePanelControl (QGuiApplication *pA);
  
public slots:

    void DoRunBack ()      { AbstractControl::DoRunBack (0,true); }

    void DoRunFwd ()      { AbstractControl::DoRunFwd (0,true); }

    void DisplayPic (QImage *pImg);
    
    void NewPicname ();
    void CloseNewPicname ();
    void DoCallSwitch ();
    void Settings ();
    void Reload ();

private:

   void Additional ();
   void ConnectThings ();

   QMainWindow  *mainWin;
   QPixmap   imagePix;
   QAction   *picnameAction;
   QByteArray newNameBytes;
   TextBox   *picnameBox;
   QAction   *settingsAction;
   SettingsMenu *settingsMenu;
   QAction   *reloadAction;
   
   SettingServer setServer;

};

}

#endif
