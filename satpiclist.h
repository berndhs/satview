#ifndef SATPICLIST_H
#define SATPICLIST_H

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


#include <map>

#include "controlpanel.h"
#include "satpicbuf.h"
#include "fault.h"
#include "dbconnect.h"


namespace satview {

/** @brief container for satpics
 *   This buffers the reading from the database, if necessary
 */



class SatPicList {

 private:

  SatPicList();
 

 public:


  ~SatPicList();

  void SetDBParams (const DBConnection::Method &m,
                   string server, string db, 
                   string user, string pass, 
                   string picname);

  void SetServer (string server);
  void SetFilename (string filename);
  string Filename () { return mPicfilename; }
  void SetMethod (const DBConnection::Method m);
  DBConnection::Method Method();

  bool LoadFromDB();
  bool ConnectDB();

  void Ditch();

  unsigned long int   NumImages();

  void SetControl (ControlPanel *dcon) { pDC = dcon; }

  void        Rewind();
  void        ToEnd();
  SatPicBuf*  Current();
  SatPicBuf*  PostIncr();
  SatPicBuf*  PostDecr();
  void        Skip(int n=1);
  bool        AtEnd();

  void        Wind (int n); // wind forward or back n seconds


  SatPicBuf* Find (unsigned long int sec_stamp); // doesn't affect iterators
  SatPicBuf* FindSet (unsigned long int sec_stamp);  // DOES affect iterators

  void Start();

 private:

  typedef std::multimap<unsigned long int, SatPicBuf*> PicMap_Type;
 
  string     mPicfilename;

  string     mServer;
  string     mDBName;
  string     mUser;
  string     mPass;

  DBConnection DBCon;


  PicMap_Type  mBufMap;

  PicMap_Type::iterator  mIt;

  ControlPanel* pDC;
  bool ConnectDB (string server, string db, string user, string pass);
  void DisconnectDB ();
  void LoadDummy();

  static SatPicList  *pTheOnly;

 public:

  static SatPicList  *Instance();

};
}

#endif
