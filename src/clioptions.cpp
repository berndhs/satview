#include "clioptions.h"
#include <iostream>
#include "delib-debug.h"

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

using namespace std;
using namespace deliberate;

namespace satview {

CliOptions::CliOptions (QString pgmname)
{  
  Init(pgmname);
  defaultWestImage = "WCIR.JPG";
  defaultEastImage = "ECIR.JPG";
  QString Wmsg = QString("show GOES ") + defaultWestImage + QString(" images");
  QString Emsg = QString("show GOES ") + defaultEastImage + QString(" images");
  AddSoloOption ("help","h","give help message and exit");
  AddSoloOption ("version","v","give version and exit");
  AddSoloOption ("web","w","use web interface");
  AddSoloOption ("dir","d","user direct sql interface");
  AddSoloOption ("sqlite","L","use SQLITE database");
  AddSoloOption ("mysql","Y","use MySql database (default)");
  AddSoloOption ("eastcoast","E",Emsg);
  AddSoloOption ("westcoast","W",Wmsg);
  AddStrOption ("server","s","input server");
  AddStrOption ("path","P","path on web server");
  AddStrOption ("outserver","o","output server");
  AddIntOption ("minhours","m", "minimum image age to consider");
  AddIntOption ("maxhours","M", "maximum image age to consider");
  AddStrOption ("image","i", "image name");
  AddSoloOption ("onewindow","1","use single window GUI for mobile devices");
        
}


bool
CliOptions::SetImage (string & image)
{
  bool seenImage = SeenOpt ("image");
  bool seenWest = SeenOpt ("westcoast");
  bool seenEast = SeenOpt ("eastcoast");
  if (seenImage) {
     image = mOptValues["image"]->theValue.toString().toStdString();
     return true;
  }
  if (seenWest) {
    image = defaultWestImage.toStdString();
    return true;
  }
  if (seenEast) {
    image = defaultEastImage.toStdString();
    return true;
  }
  return false;
}

bool
CliOptions::SetInterface (string & interface)
{
  bool seenWeb = SeenOpt ("web");
  bool seenDir = SeenOpt ("dir");
  if (seenWeb) {
     interface = "web";
     return true;
  }
  if (seenDir > 0) {
     interface = "dir";
     return true;
  }
  return false;
}

bool
CliOptions::SetDBType (string &dbtype)
{
  bool seenLite = SeenOpt ("sqlite");
  bool seenMy   = SeenOpt ("mysql");
  if (seenLite) {
    dbtype = "QSQLITE";
    return true;
  }
  if (seenMy) {
    dbtype = "QMYSQL";
    return true;
  }
  return false;
}

bool
CliOptions::SetMobile (bool & mobile)
{
  bool seenMobile = SeenOpt ("onewindow");
  if (seenMobile) {
    mobile = true;
    return true;
  }
  return false;
}

bool
CliOptions::SetServerInbound (string & server)
{
  bool seenServer = SeenOpt ("server");
  bool changedIt (false);
  if (seenServer) {
    server = mOptValues["server"]->theValue.toString().toStdString();
    changedIt = true;
  }
  return changedIt;
}

bool
CliOptions::SetPath (string & path)
{
  bool seenPath = SeenOpt ("path");
  if (seenPath) {
    path = mOptValues["path"]->theValue.toString().toStdString();
    return true;
  }
  return false;
}

bool
CliOptions::SetServerOutbound (string & server)
{
  bool seenOutServer = SeenOpt ("outserver");
  if (seenOutServer) {
    server = mOptValues["outserver"]->theValue.toString().toStdString();
    return true;
  }
  return false;
}

bool
CliOptions::SetMinHours (unsigned long int & hours)
{
  bool seenMin = SeenOpt ("minhours");
  if (seenMin) {
    hours = mOptValues["minhours"]->theValue.toInt();
    return true;
  }
  return false;
}

bool
CliOptions::SetMaxHours (unsigned long int &hours)
{
  bool seenMax = SeenOpt ("maxhours");
  if (seenMax) {
    hours = mOptValues["maxhours"]->theValue.toInt();
    return true;
  }
  return false;
}


} // namespace
