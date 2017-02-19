#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

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

#include <QMenu>
#include <QPoint>

namespace satview {

enum SettingsAction { ST_none, ST_server, ST_IF, ST_pic, ST_range };

class SettingsMenu : public QMenu {

Q_OBJECT

public:

  SettingsMenu (QWidget *parent);
  
  
  SettingsAction Exec (const QPoint here);
  
private:

  QAction  *changeServer;
  QAction  *changeIF;
  QAction  *changePicname;
  QAction  *changeRange;

};

};

#endif

