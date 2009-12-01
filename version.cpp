#include "version.h"
#include <string>
#include <QMessageBox>
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

namespace satview {

  std::string CloudTrackVersion("0.3.5");

  std::string ProgramName("CloudTrack");
  
  std::string Version ()
  { 
    return std::string(ProgramName + " " 
                   + CloudTrackVersion + " " + std::string(__DATE__) + " "
		       + std::string(__TIME__));
  }

  void ShowVersionWindow ()
  {
    QString versionMessage(Version().c_str());
    QMessageBox box;
    box.setText (versionMessage);
    QTimer::singleShot(30000,&box,SLOT(accept()));
    box.exec();
  }

}
