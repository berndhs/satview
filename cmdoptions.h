#ifndef CMDOPTIONS_H
#define CMDOPTIONS_H

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

#include <string>
#include <boost/program_options.hpp>

using namespace std;

namespace bpo = boost::program_options;

namespace satview {


class CmdOptions {

public:

CmdOptions (const string pgmname);

bool Parse (int argc, char*argv[]);

void Usage ();

bool WantHelp ();
bool WantVersion ();

/** @brief these SetSomething functions set the option variable
  * if it was specified on the command line, otherwise they leave
  * the parameter alone. The bool return says whether or not the 
  * parameter was changed
  */
bool SetInterface (string & interface);
bool SetImage (string &image);
bool SetServerInbound (string & servIn);
bool SetServerOutbound (string & servOut);
bool SetMinHours (int & hours);
bool SetMaxHours (int & hours);

private:

   string mPgm;
   
   bpo::options_description             mDashOpts;
   bpo::positional_options_description  mSimpleOpts;
   bpo::variables_map                   mOptValues;
   
   string defaultWestImage;
   string defaultEastImage;
   
};


}


#endif
