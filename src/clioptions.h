#ifndef CLIOPTIONS_H
#define CLIOPTIONS_H

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

#include <QString>
#include <string>
#include "cmdoptions.h"

using namespace std;

using namespace deliberate;

namespace satview {


class CliOptions : public CmdOptions {

public:

CliOptions (const QString pgmname);


/** @brief these SetSomething functions set the option variable
  * if it was specified on the command line, otherwise they leave
  * the parameter alone. The bool return says whether or not the 
  * parameter was changed
  */
bool SetInterface (string & interface);
bool SetImage (string &image);
bool SetServerInbound (string & servIn);
bool SetServerOutbound (string & servOut);
bool SetPath (string & path);
bool SetMinHours (unsigned long int & hours);
bool SetMaxHours (unsigned long int & hours);
bool SetDBType (string &dbtype);
bool SetMobile (bool & mobile);

private:

   QString defaultWestImage;
   QString defaultEastImage;
   
};


}


#endif
