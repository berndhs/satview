#include "settingsmenu.h"
#include "delib-debug.h"
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

/** @brief Settings Menu
  */
  
  
namespace satview {

SettingsMenu::SettingsMenu (QWidget * parent)
{
  //setParent (parent);
  changeServer = addAction (tr("Server/Path"));
  changeIF = addAction (tr("Data Source"));
  changePicname = addAction (tr("Image Name"));
  changeRange = addAction (tr("Time Stamp Range"));
}

SettingsAction
SettingsMenu::Exec (const QPoint here)
{
  show ();
  QAction * result = exec (here);
  hide ();
  qDebug () << " exec settings done " << result;
  if (result == changeServer) {
    return ST_server;
  } else if (result == changeIF) {
    return ST_IF;
  } else if (result == changePicname) {
    return ST_pic;
  } else if (result == changeRange) {
    return ST_range;
  } 
  return ST_none;
}

}
