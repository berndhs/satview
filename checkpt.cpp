#include "checkpt.h"
#include <QTimer>

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

CheckPt::CheckPt (std::string msg)
{
  theMsg = QString(msg.c_str());
}

CheckPt::CheckPt (QString msg)
  :theMsg(msg)
{}

CheckPt::CheckPt (const char * msg)
  :theMsg(msg)
{}

void
CheckPt::DoPopupBox()
{
  QMessageBox box;
  box.setText("Check: " + theMsg);
  QTimer::singleShot(10000,&box,SLOT(accept()));
  box.exec();
}


