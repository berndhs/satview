#ifndef SATPICLIST_H
#define SATPICLIST_H

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

//


#include <map>


#include "satpicbuf.h"
#include "fault.h"
#include "dbconnect.h"

#if SATVIEW_USE_QNET
#include <QObject>
#endif


namespace satview {

/** @brief container for satpics
 *   This buffers the reading from the database, if necessary
 */


class AbstractControl;

class SatPicList
#if SATVIEW_USE_QNET
            : public QObject
#endif
             {

#if SATVIEW_USE_QNET
Q_OBJECT
#endif

 private:

  SatPicList();
 

 public:


  ~SatPicList();

  void SetDBParams (const DBConnection::Method &m,
                   string server, string db, 
                   string user, string pass, 
                   string picname);
  void SetDBType (const string dbType);

  void SetServer (string server);
  void SetPath   (string path);
  void SetFilename (string filename);
  void SetMinAge (unsigned long int minhours)
                  { mMinHours = minhours;
                    mMinSecs = minhours * 60 * 60;}
  unsigned long int MinAge () { return mMinHours;}
  
  void SetMaxAge (unsigned long int maxhours)
                  { mMaxHours = maxhours;
                    mMaxSecs = maxhours * 60 * 60;}
  unsigned long int MaxAge () { return mMaxHours; }
  
  string Filename () { return mPicfilename; }
  void SetMethod (const DBConnection::Method m);
  DBConnection::Method Method();

  DBConnection * DBConnect() { return &DBCon; }
  bool LoadFromDB();
  bool ConnectDB();


  void Ditch();

  unsigned long int   NumImages();

  void SetControl (AbstractControl *dcon) { pDC = dcon; }

  SatPicBuf*  ToStart();
  SatPicBuf*  ToEnd();
  SatPicBuf*  Current(bool snap = true);  // snap - move to last if fails
  SatPicBuf*  PostIncr();
  SatPicBuf*  PostDecr();
  bool        AtEnd();


  SatPicBuf* Find (unsigned long int sec_stamp); // doesn't affect iterators
  SatPicBuf* FindSet (unsigned long int sec_stamp);  // DOES affect iterators

  void Start();
  
#if SATVIEW_USE_QNET
  public slots:
  
  void LoadFromIndex();
#endif

 private:

  typedef std::multimap<unsigned long int, SatPicBuf*> PicMap_Type;
 
  string     mPicfilename;

  string     mServer;
  string     mDBName;
  string     mUser;
  string     mPass;
  string     mPath;
  
  unsigned long int mMinHours;
  unsigned long int mMinSecs;
  unsigned long int mMaxHours;
  unsigned long int mMaxSecs;

  DBConnection DBCon;


  PicMap_Type  mBufMap;

  PicMap_Type::iterator  mIt;

  AbstractControl* pDC;
  bool ConnectDB (string server, string db, string user, string pass);
  void DisconnectDB ();
  void LoadDummy();
  SatPicBuf             * pDummyBuf;
  PicMap_Type::iterator   mDummyIt;
  bool                    mHaveDummy;

  static SatPicList  *pTheOnly;

 public:

  static SatPicList  *Instance();
  static void Destroy();

};
}

#endif
