#include "trackmain.h"
#include "satpiclist.h"
#include "version.h"
#include "satview-defaults.h"
#include "clioptions.h"
#include "delib-debug.h"
#include "satviewsplash.h"
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


string server;
DBConnection::Method method;

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
  server = server0;
  string IFarg("dir");
  method = DBConnection::Con_Web;
  string dbType("QMYSQL");
  string db("weather");
  string user("weather");
  string pass("quetzalcoatl");
  string path(SATVIEW_DEFAULT_PATH);
  
  unsigned long int maxAge = 30 * 24; // 30 days
  
  unsigned long int minAge = 0;
  
  bool versionOnly(false);
  bool onewindow (false);


  deliberate::UseMyOwnMessageHandler();
  
  QApplication App(argc, argv);
  
  satview::Splash splash ;
  splash.show ();

  
  CliOptions opt(satview::MyName().c_str());
  bool cliOk = opt.Parse (argc,argv);
  if (!cliOk) {
    cout << satview::MyName() << ":" << endl;
    opt.Usage();
    exit(1);
  }

  versionOnly = opt.WantVersion();
  opt.SetImage(DefaultFile);
  opt.SetServerInbound(server);
  opt.SetInterface(IFarg);
  opt.SetPath(path);
  opt.SetMinHours(minAge);
  opt.SetMaxHours(maxAge);
  opt.SetDBType (dbType);
  opt.SetMobile (onewindow);

  /** @brief some elementary command line, 2 choices of image names.
   * More later when we have time.
   */

  satview::CLIVersion();
  if (versionOnly) {
    return(0);
  }
  if (opt.WantHelp()) {
    std::cout << satview::MyName() << ":" << endl;
    opt.Usage();
    exit(0);
  }


  try {
    string conmeth;
    if (IFarg == "dir") {
      method = DBConnection::Con_MySql;
      conmeth = "dir";
    } else if (IFarg == "web") {
      method = DBConnection::Con_Web;
      conmeth = "web";
    } else {
      conmeth = "none";
    }
    bool again(false);
    do {
      AbstractControl  * Control(0);
      if (onewindow) {
      qDebug () << " one panel";
        Control = new OnePanelControl (&App);
      } else {
      qDebug () << " two panel";
        Control = new TwoPanelControl (&App);
      }
      QObject::connect (Control, SIGNAL (IndexArrived()),
                        &splash, SLOT (Finish ()));
      Control->SetPicname(DefaultFile);
      Control->SetServer(server);
      Control->SetConMeth(conmeth);
      Control->SetMinAge (minAge);
      Control->SetMaxAge (maxAge);
      SatPicList::Instance()->SetPath (path);
      SatPicList::Instance()->SetDBType (dbType);
      SatPicList::Instance()->SetDBParams(method,
                                         server,
					 db,
					 user,
					 pass,
                                         DefaultFile);
      SatPicList::Instance()->SetMinAge(minAge);
      SatPicList::Instance()->SetMaxAge(maxAge);
      SatPicList::Instance()->LoadFromDB();
      SatPicList::Instance()->SetControl (Control);
      SatPicList::Instance()->Start();
      Control->DoWindFwd(0,true);
      Control->DoStepFwd();
      Control->show();
      splash.raise();
      App.processEvents();
      App.exec();
      again = false;
      SatPicList::Destroy();
      delete Control;
    } while (again);
    exit(0);
  } catch (berndsutil::Fault &F) {
    cout << F.String() << endl;
    exit (1);
  }
}
