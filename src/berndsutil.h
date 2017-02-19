#ifndef BERNDSUTIL_H
#define BERNDSUTIL_H

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


#include <iostream>
#include <string>
#include <sstream>



namespace berndsutil {

template<class T>
    std::string toString(const T& t)
{
     std::ostringstream stream;
     stream << t;
     return stream.str();
}

template<class T>
    T fromString(const std::string& s)
{
     std::istringstream stream (s);
     T t;
     stream >> t;
     return t;
}

/** @brief Store is for callback functions that only allow a void* argument
 */

template <class T>
class Store {
public:
  Store (T initval){ myval = initval;}
  Store(){}
  Store* operator()(T object) { myval = object; return this; }
  T Value() { return myval; }
private:
  T myval;
};


} // namespace

#endif
