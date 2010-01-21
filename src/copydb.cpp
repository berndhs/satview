#include "copydb.h"
#include "copydb-config.h"

#include "dbconnect.h"
#include "berndsutil.h"

#include "cmdoptions.h"

#include <iostream>
#include <time.h>
#include <stdlib.h>

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

using namespace satview;
using namespace std;

DBConnection DBin;
DBConnection DBout;
unsigned long int secs_in_hour (60*60);
unsigned long int max_hours (24);
unsigned long int min_hours (0);
string  source_server ("www.bernd-stramm.com");
string  dest_server   ("localhost");

string version("Version 1.1");


void
copy_rec (IndexRecord &r, 
          int &readfailed, int &writefailed, int &numcopied)
{   
  bool gotdata (false);
  bool writeok (false); 
  string imgdata;
  gotdata = DBin.ReadImageData (r,imgdata);
  if (gotdata) {
    writeok = DBout.InsertRec (r,imgdata);
    if (writeok) {
    cout << "copied " << r.String() << endl;
      numcopied ++;
    } else {
      cout << "problem with " << r.String() << endl;
      writefailed ++;
    }
  } else {
    readfailed ++;
  }
}

int
main (int argc, char*argv[])
{
#if 0
  check_args (argc, argv);
#endif

  string interface("web");
  CmdOptions opts(argv[0]);
  bool ok = opts.Parse (argc, argv);
  if (!ok || opts.WantHelp()) {
     opts.Usage();
     exit(0);
  }
  if (opts.WantVersion()) {
     cout << argv[0] << "" << version << endl;
     exit(0);
  }
  
  opts.SetInterface (interface);
  opts.SetServerInbound (source_server);
  bool changed = opts.SetServerOutbound (dest_server);
  
  opts.SetMinHours (min_hours);
  opts.SetMaxHours (max_hours);
  unsigned long int max_age ( max_hours * secs_in_hour );
  unsigned long int min_age ( min_hours * secs_in_hour );
  unsigned long int now = time(0);
  unsigned long int too_old = now - max_age;
  unsigned long int too_recent = now - min_age;
  time_t dl = time_t(too_old);
  time_t cutoff = time_t (too_recent);

  cout << "Trying " << source_server << " -> " << dest_server << endl;
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
  if (interface == "web") {
    DBin.SetMethod(DBConnection::Con_Web);
  } else {
    DBin.SetMethod(DBConnection::Con_MySql);
  }
  bool inok = DBin.ConnectDB(source_server,"weather",
                           "weather","quetzalcoatl");
  if (!inok) {
    cout << " can't open input DB " << endl;
    return (1);
  }
  DBout.SetMethod(DBConnection::Con_MySql);
  bool outok = DBout.ConnectDB(dest_server,"weather",
			     "weather","quetzalcoatl");
  if (!outok) {
    cout << "can't open output DB " << endl;
    return (2);
  } 
 
  IndexRecord r;
  bool gotindex(false);
  gotindex = DBin.LoadIndex("ECIR.JPG", false, too_old, too_recent);
  if (!gotindex) {
    cout << "can't get input index" << endl;
    return (3);
  }
  int    numcopied(0);
  int    readfailed(0);
  int    writefailed(0);
  cout << "skipping older records..." ;
  cout.flush();
  int   num_older(0);
  while (DBin.ReadIndexRec(r)) {
    if (r.ident < too_old) {
      num_older++;
      continue;
    }
    if (r.ident > too_recent) {
      break;
    }
    copy_rec(r, readfailed, writefailed, numcopied);
    break;
  }
  cout << num_older << " old records skipped" << endl;
  while (DBin.ReadIndexRec(r)) {
    if (r.ident < too_old) {
      continue;
    }
    if (r.ident > too_recent) {
      break; // finished
    }
    copy_rec (r, readfailed, writefailed, numcopied);
  }
  cout << endl;
  cout << " copied ok:    " << numcopied << endl;
  cout << " read fail:    " << readfailed << endl;
  cout << " write fail:   " << writefailed << endl;
  DBin.Disconnect();
  DBout.Disconnect();
  return 0;
}
