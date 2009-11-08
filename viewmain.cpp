#include "viewmain.h"
#include "satpiclist.h"
#include "satview.h"
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

DisplayControl DC;

#define SATVIEW_USE_SDL 0
#if SATVIEW_USE_SDL
static SDL_Surface *pSDL;

SDL_Surface*
SDL_Screen ()
{
  return pSDL;
}

SDL_Surface*
SetScreen (SDL_Surface * pS)
{
  pSDL = pS;
  return pSDL;
}

void
sdl_init()
{
   int stat = SDL_Init(SDL_INIT_VIDEO);
   if (stat != 0) {
     throw berndsutil::Fault("SDL init problem");
   }
   atexit (SDL_Quit);
   SetScreen (SDL_SetVideoMode(640,480,32,SDL_ANYFORMAT | SDL_DOUBLEBUF));
  
}
#endif

string DefaultFile(SATVIEW_DEFAULT_PICNAME);
string server0(SATVIEW_DEFAULT_S0);
string server1(SATVIEW_DEFAULT_S1);
string server2(SATVIEW_DEFAULT_S2);


string server;
DBConnection::Method method;

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
  server = server1;
  method = DBConnection::Con_WebSock;
  string db("weather");
  string user("weather");
  string pass("quetzalcoatl");

  checkargs(argc,argv);

  /** @brief some elementary command line, 2 choices of image names.
   * More later when we have time.
   */

  std::cout << "viewsat " << satview::Version() << std::endl;

#if SATVIEW_USE_SDL
  sdl_init();
#endif
  try {
      Fl::visual(FL_DOUBLE | FL_INDEX | FL_RGB8);
      Fl::get_system_colors();

      DC.Filename->maximum_size(256);
      DC.ChangeDate->maximum_size(256);
      DC.ChangeDate->value("                                     ");
      DC.Seconds->maximum_size(256);
      DC.Seconds->value("0");
      DC.Filename->value(DefaultFile.c_str());
      DC.ServerName->value(server.c_str());
      DC.DirectCheck->value(method == DBConnection::Con_MySqlCPP);
      DC.WebCheck->value(method == DBConnection::Con_WebSock);

      SatPicList::Instance()->SetDBParams(method,
                                         server,
					 db,
					 user,
					 pass,
					 DefaultFile);
      SatPicList::Instance()->LoadFromDB();
      SatPicList::Instance()->SetControl (&DC);
      SatPicList::Instance()->Start();
      WindAll_cb(0,0);
      DC.Selection->show();
      DC.ImageWin->show();
      ret = Fl::run();
      return 42;
  } catch (berndsutil::Fault &F) {
   
    cout << F.String() << endl;
  }
}
