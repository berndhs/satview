#include "version.h"
#include <string>
#include <iostream>
#include <QMessageBox>
#include <QTimer>
#include <stdio.h>

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

namespace satview {

  std::string ProgramVersion("0.5.0pre");

  std::string ProgramName("CloudTrack");
  std::string copyright ("Copyright (C) 2017 Bernd H Stramm");
   
  std::string Version ()
  { 
    return std::string(ProgramName + " Version " 
                   + ProgramVersion + " " + std::string(__DATE__) + " "
		       + std::string(__TIME__)
		       + std::string("\r\n")
		       + copyright);
  }

  std::string MyName()
  {
    return ProgramName;
  }

  void ShowVersionWindow ()
  {
    QString versionMessage(Version().c_str());
    QMessageBox box;
    box.setText (versionMessage);
    QTimer::singleShot(30000,&box,SLOT(accept()));
    box.exec();
  }
  
  void CLIVersion ()
  {
  // printf ("%s\n",Version().c_str()); 
   std::cout << Version().c_str() << std::endl;
  }
  
 
}
