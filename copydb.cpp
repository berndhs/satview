

#include "dbconnect.h"
#include "berndsutil.h"
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
unsigned long int max_age ( 24 * secs_in_hour );
string  source_server ("www.bernd-stramm.com");
string  dest_server   ("localhost");

string version("Version 1.0");
string help("[max-age-in-hours] [S0 | S1 | S2] ");

void
check_args (int argc, char * argv[])
{
  string s_arg;
  signed long int maybe_number(0);
  int a(1);
  while (a < argc) {
    s_arg = string(argv[a]); 
    if (s_arg == "-v" || s_arg == "--version") {
      cout << argv[0] << " " << version << endl;
      exit(0);
    }
    if (s_arg == "-h" || s_arg == "--help") {
      cout << help << endl;
      exit(0);
    }
    if (s_arg == "S0") {
      source_server = "localhost";
    } else if (s_arg == "S1") {
      source_server = "www.bernd-stramm.com";
    } else if (s_arg == "S2") {
      source_server = "192.168.1.152";
    } else {
      try {
        maybe_number = berndsutil::fromString<unsigned long long int>(s_arg);
      } catch (...) {
        maybe_number = 0;
      }
      if (maybe_number > 0) {
        max_age = maybe_number * secs_in_hour;
      } else {
        cerr << argv[0] << ":  ";
        cerr << "don't understand argument " << s_arg << endl;
        cerr << "usage:  " << argv[0] << " " << help << endl;
       exit(1);
     }
    }
    a++;
  }
}

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
  check_args (argc, argv);
  unsigned long int now = time(0);
  unsigned long int deadline = now - max_age;
  time_t dl = time_t(deadline);

  cout << "Trying " << source_server << " -> " << dest_server << endl;
  cout << " newer than " << ctime(&dl) << " = "
       << max_age/secs_in_hour << " hours (" 
       << max_age << " secs)" 
       << endl;
  
  DBin.SetMethod(DBConnection::Con_WebSock);
  bool inok = DBin.ConnectDB(source_server,"weather",
                           "weather","quetzalcoatl");
  if (!inok) {
    cout << " can't open input DB " << endl;
    return (1);
  }
  DBout.SetMethod(DBConnection::Con_MySqlCPP);
  bool outok = DBout.ConnectDB(dest_server,"weather",
			     "weather","quetzalcoatl");
  if (!outok) {
    cout << "can't open output DB " << endl;
    return (2);
  } 
 
  IndexRecord r;
  bool gotindex(false);
  gotindex = DBin.LoadIndex("ECIR.JPG");
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
    if (r.ident < deadline) {
      num_older++;
      continue;
    }
    copy_rec(r, readfailed, writefailed, numcopied);
    break;
  }
  cout << num_older << " records skipped" << endl;
  while (DBin.ReadIndexRec(r)) {
    if (r.ident < deadline) {
      continue;
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
