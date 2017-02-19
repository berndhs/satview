/** 
 * copymain.cpp
 *
 * Import records between weather pic databases
 *
 */
 
//
//  Importright (C) 2017 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//

#include "import.h"
#include "copydb-config.h"
#include "delib-debug.h"

#include "dbconnect.h"
#include "berndsutil.h"

#include "clioptions.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

#include <QGuiApplication>


unsigned long int secs_in_hour (60*60);
unsigned long int max_hours (24);
unsigned long int min_hours (0);
string  source_path (".");
string  dest_server   ("localhost");
string  dest_path ("");

string version("Version 0.1");

using namespace satview;

int
main (int argc, char*argv[])
{
  deliberate::UseMyOwnMessageHandler ();
  string interface("web");
  CliOptions opts(argv[0]);
  opts.AddSoloOption ("outmysql","Q","destination server is MySql");
  opts.AddSoloOption ("outsqlite","T","destination server is Sqlite");
  opts.AddStrOption ("outpath","O","destination path (or sqlite directory)");
  opts.AddSoloOption ("nodisplay","X", "don't show status distplay");
  bool ok = opts.Parse (argc, argv);
  if (!ok || opts.WantHelp()) {
     opts.Usage();
     exit( ok ? 0 : 1);
  }
  if (opts.WantVersion()) {
     cout << argv[0] << "" << version << endl;
     exit(0);
  }
  bool nodisplay = opts.SeenOpt ("nodisplay");
  
  QGuiApplication App (argc, argv, !nodisplay);
//  QGuiApplication App (argc, argv);
  ImportEngine * engine = new ImportEngine(&App, !nodisplay);
  
  opts.SetInterface (interface);
  opts.SetServerOutbound (dest_server);
  
  opts.SetMinHours (min_hours);
  opts.SetMaxHours (max_hours);
  unsigned long int max_age ( max_hours * secs_in_hour );
  unsigned long int min_age ( min_hours * secs_in_hour );
  unsigned long int now = time(0);
  unsigned long int too_old = now - max_age;
  unsigned long int too_recent = now - min_age;
  time_t dl = time_t(too_old);
  time_t cutoff = time_t (too_recent);
  
  QString dpath (dest_path.c_str());
  opts.SetStringOpt ("outpath",dpath);
  dest_path = dpath.toStdString();
  
  QString srcPath (source_path.c_str());
  opts.SetStringOpt ("path",srcPath);

  cout << "Trying " << srcPath.toStdString() << " -> " << dest_server << endl;
  cout << " newer than " << ctime(&dl) << " = "
       << max_hours << " hours (" 
       << max_age << " secs)" 
       << endl;
  cout << " older than " << ctime(&cutoff) << " = "
       << min_hours << " hours ("
       << min_age << " secs)"
       << endl;
       
  if (min_age > max_age) {
     cout << " min > max, nothing to do" << endl;
     exit (1);
  }
 
  ImportEngine::DBType  outType (ImportEngine::T_mysql);
  if (opts.SeenOpt ("outsqlite")) {
    outType = ImportEngine::T_sqlite;
  } else if (opts.SeenOpt("outmysql")) {
    outType = ImportEngine::T_mysql;
  }
  engine->SetSourceDir (srcPath);
  engine->SetDest   (dest_server.c_str(), dest_path.c_str(),
                    outType, "weather", "weather","quetzalcoatl");
  engine->StartEngine ("",too_old,too_recent);

  return App.exec ();
}


