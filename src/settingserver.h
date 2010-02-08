#ifndef SETTINGSERVER_H
#define SETTINGSERVER_H

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
#include <QDialog>
#include "ui_server.h"

namespace satview {

class SettingServer : public QDialog, public Ui_settingServer {

Q_OBJECT

public:

  SettingServer ();
  
  QString NewName ();
  void SetOldname (QString old);

public slots:

  void DoOk ();
  void DoNotOk ();

private:

  QString newName;



};

}

#endif
