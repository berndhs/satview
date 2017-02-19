//
//  Copyright (C) 2017 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
#ifndef FAULT_H
#define FAULT_H
#include <iostream>

using namespace std;

namespace berndsutil {

  /** @brief Exception class */

  class Fault {
    public:
      Fault():msg("?"),ident(-1){}
	Fault(string m):msg(m),ident(-1){}
      Fault(string m,int id):msg(m),ident(id){}
    string String() { return msg; }
    int    Ident() { return ident; }

  private:
      string msg;
      int   ident;
  }; 

}

#endif
