#include "settingserver.h"

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

namespace satview {

SettingServer::SettingServer ()
{
  setupUi (this);
  newName = "";
  connect (okButton, SIGNAL (clicked()), this, SLOT (DoOk()));
}

void
SettingServer::DoOk ()
{
  newName = serverText->text();
  accept ();
}

void
SettingServer::SetOldname (QString old)
{
  serverText->setText (old);
  newName = old;
}

void
SettingServer::DoNotOk ()
{
}

QString
SettingServer::NewName ()
{
  return newName;
}

}

