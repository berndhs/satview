#ifndef CHECKPT_H
#define CHECKPT_H

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


#include <QMessageBox>
#include <string>
#include <QString>

class CheckPt  {

 public:
  CheckPt (std::string m);
  CheckPt (QString m);
  CheckPt (const char * m);
  CheckPt ();
  
  void DoPopupBox();
  
  static bool Silent(bool s=true) { silent=s; return silent; }
  static bool Verbose (bool v=true) { silent = !v; return !silent; }

  static void msg (std::string m)
           {
             CheckPt p(m);
             p.DoPopupBox();
           }
  static void msg (QString m)
           {
             CheckPt p(m);
             p.DoPopupBox();
           }
  static void msg (char * m)
           {
             CheckPt p(m);
             p.DoPopupBox();
           }
  static void msg (char *m, qint64 n)
           {
              QString num;
              num.setNum(n);
              QString mess(m);
              CheckPt p(mess + " " + num);
              p.DoPopupBox();
           }
  static void msg ()
           {
             CheckPt p("?");
             p.DoPopupBox();
           }
    

 private:
 
  static bool silent;


  QString theMsg;

};

#endif
