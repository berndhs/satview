#include "cmdoptions.h"
#include <iostream>

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
namespace bpo = boost::program_options;

namespace satview {

CmdOptions::CmdOptions (string pgmname)
   :mPgm("")
{
  mPgm = pgmname;
  defaultWestImage = "WCIR.JPG";
  defaultEastImage = "ECIR.JPG";
  string Wmsg = string("show GOES ") + defaultWestImage + string(" images");
  string Emsg = string("show GOES ") + defaultEastImage + string(" images");
  mDashOpts.add_options()
        ("help,h","give help message and exit")
        ("version,v","give version and exit")
        ("web,w","use web interface")
        ("dir,d","user direct sql interface")
        ("eastcoast,E",Emsg.c_str())
        ("westcoast,W",Wmsg.c_str())
        ("server,s",bpo::value<string>(),"input server")
        ("path,P",bpo::value<string>(),"path on web server")
        ("outserver,o",bpo::value<string>(),"output server")
        ("minhours,m",bpo::value<int>(),"minimum image age to consider")
        ("maxhours,M",bpo::value<int>(),"maximum image age to consider")
        ("image,i",bpo::value<string>(),"image name")
        ;
  mSimpleOpts.add ("server",-1);   
}

bool
CmdOptions::Parse (int argc, char *argv[])
{
  try {
    bpo::store (bpo::command_line_parser (argc, argv).
                 options(mDashOpts).positional(mSimpleOpts).run(),
                 mOptValues);
    bpo::notify (mOptValues);
  } catch (...) {
    return false;
  }
  return true;  
}

void
CmdOptions::Usage ()
{
  cout << mPgm << ":" << endl;
  mDashOpts.print(cout);
}

bool
CmdOptions::SetImage (string & image)
{
  int seenImage = mOptValues.count("image");
  int seenWest = mOptValues.count("westcoast");
  int seenEast = mOptValues.count("eastcoast");
  if (seenImage > 0) {
     image = mOptValues["image"].as<string>();
     return true;
  }
  if (seenWest > 0) {
    image = defaultWestImage;
    return true;
  }
  if (seenEast > 0) {
    image = defaultEastImage;
    return true;
  }
  return false;
}

bool
CmdOptions::SetInterface (string & interface)
{
  int seenWeb = mOptValues.count("web");
  int seenDir = mOptValues.count("dir");
  if (seenWeb > 0) {
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
CmdOptions::SetServerInbound (string & server)
{
  int seenServer = mOptValues.count("server");
  if (seenServer > 0) {
    server = mOptValues["server"].as<string>();
    return true;
  }
  return false;
}

bool
CmdOptions::SetPath (string & path)
{
  int seenPath = mOptValues.count("path");
  if (seenPath > 0) {
    path = mOptValues["path"].as<string>();
    return true;
  }
  return false;
}

bool
CmdOptions::SetServerOutbound (string & server)
{
  int seenOutServer = mOptValues.count("outserver");
  if (seenOutServer > 0) {
    server = mOptValues["outserver"].as<string>();
    return true;
  }
  return false;
}

bool
CmdOptions::SetMinHours (int & hours)
{
  int seenMin = mOptValues.count("minhours");
  if (seenMin > 0) {
    hours = mOptValues["minhours"].as<int>();
    return true;
  }
  return false;
}

bool
CmdOptions::SetMaxHours (int &hours)
{
  int seenMax = mOptValues.count("maxhours");
  if (seenMax > 0) {
    hours = mOptValues["maxhours"].as<int>();
    return true;
  }
  return false;
}

bool
CmdOptions::WantHelp ()
{
  return mOptValues.count("help");
}

bool
CmdOptions::WantVersion ()
{
  return mOptValues.count("version");
}

} // namespace
