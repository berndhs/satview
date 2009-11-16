#include "trackmain.h"
#include "satpiclist.h"
#include "version.h"
#include "satview-defaults.h"

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

//

#include <iostream>

using namespace std;
using namespace satview;


string DefaultFile(SATVIEW_DEFAULT_PICNAME);
string server0(SATVIEW_DEFAULT_S0);
string server1(SATVIEW_DEFAULT_S1);
string server2(SATVIEW_DEFAULT_S2);


string server;
DBConnection::Method method;

string myname = "cloudtrack";

void
  checkargs (int argc, char*argv[])
{

  int a;
  for (a = 1; a<argc; a++) {
 
    string ar = string(argv[a]);
    if (ar == "W") {
      DefaultFile = "WCIR.JPG";
    } 
    if (ar == "E") {
      DefaultFile = "ECIR.JPG";
    }
    if (ar == "S0") {
      server = server0;
    }
    if (ar == "S1") {
      server = server1;
    }
    if (ar == "S2") {
      server = server2;
    }
    if (ar == "web") {
      method = DBConnection::Con_WebSock;
    }
    if (ar == "dir") {
      method = DBConnection::Con_MySqlCPP;
    }
    
  }
}
/** @brief This program loads a bunch of weather satellite pictures
 * from a database, and displays them one by one. It has options for 
 * stepping forwards, backwards, stepping by 6 hours (subject to 
 * availability of pictures) and also run a movie forwards and backwards.
 */

/** @brief on MS Visual C++ 2008 I had problems with this main program.
 *  It was calling this SDL_main instead of main. Weird.
 */

int
main (int argc, char*argv[])
{
  int ret;
;
  server = server0;
  method = DBConnection::Con_MySqlCPP;
  string db("weather");
  string user("weather");
  string pass("quetzalcoatl");

  checkargs(argc,argv);

  /** @brief some elementary command line, 2 choices of image names.
   * More later when we have time.
   */

  std::cout << myname << satview::Version() << std::endl;

  QApplication App(argc, argv);

  ControlPanel Control (&App);

  try {
    string conmeth;
    if (method == DBConnection::Con_MySqlCPP) {
      conmeth = "dir";
    } else if (method == DBConnection::Con_WebSock) {
      conmeth = "web";
    } else {
      conmeth = "none";
    }
      Control.SetPicname(DefaultFile);
      Control.SetServer(server);
      Control.SetConMeth(conmeth);

      SatPicList::Instance()->SetDBParams(method,
                                         server,
					 db,
					 user,
					 pass,
					 DefaultFile);
      SatPicList::Instance()->LoadFromDB();
      SatPicList::Instance()->SetControl (&Control);
      SatPicList::Instance()->Start();
      Control.DoWindFwd(0,true);
      Control.DoStepFwd();
      Control.show();
      App.processEvents();
      App.exec();
      return 42;
  } catch (berndsutil::Fault &F) {
   
    cout << F.String() << endl;
  }
}
