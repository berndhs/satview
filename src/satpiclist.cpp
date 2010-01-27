
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

#include "satpiclist.h"
#include "abstractcontrol.h"
#include <time.h>
#include <iostream>
#if SATVIEW_USE_QNET
#include <QObject>
#endif

namespace satview {

  SatPicList* SatPicList::pTheOnly (0);

  SatPicList::SatPicList()
    :
     pDC(0)
  {
    mMaxHours = 0;
    mMaxSecs = 0;
    mMinHours = 0;
    mMinSecs = 0;
    mHaveDummy = false;
    LoadDummy();
#if SATVIEW_USE_QNET
    connect (&DBCon, SIGNAL(IndexArrival()),this,SLOT(LoadFromIndex()));
#endif
  }

  SatPicList::~SatPicList()
  {
    Ditch();
  }

  void
  SatPicList::Start()
  {
    if (pDC) {
      // fill in start values
      ToEnd();
      PicMap_Type::iterator it = mBufMap.end();
      it --;
      if (it == mBufMap.end()) { // no data
	return;
      }
      SatPicBuf * pBuf = it->second;
 
      pDC->SetPicname(pBuf->PicName().c_str());
      time_t t = pBuf->Ident();
      string as_secs = berndsutil::toString(t);
      struct tm theTime;
#ifdef _MSC_VER
      localtime_s(&theTime,&t);
#else
      localtime_r (&t, &theTime);
#endif
      const int datelen = 256;
      char plain[datelen+sizeof(void*)];
      int len = strftime (plain, datelen,"%Y-%m-%d %H:%M:%S", &theTime);
      plain[len] = 0;
      pDC->SetDate(plain);
      string identstring = (as_secs + string (" - ")
			    + berndsutil::toString(pBuf->Serial()));
      pDC->SetIdentTag(identstring);
    }
  }

  void
  SatPicList::Ditch()
  { 
    PicMap_Type::iterator it = mBufMap.begin();
    SatPicBuf * pBuf;
    while (it != mBufMap.end()) {
      pBuf = it->second;     
      if ( pBuf && pBuf != pDummyBuf) {
        delete pBuf;
      }
      it->second = 0;
      it++;
    }
    mBufMap.clear();
    DisconnectDB();
  }


  bool
  SatPicList::ConnectDB (string server, string db, string user, string pass)
  {
    bool ok = DBCon.ConnectDB (server, db, user, pass);
    SatPicBuf::SetDBCon(&DBCon);
    return ok;
  }

  void
  SatPicList::DisconnectDB()
  {
    DBCon.Disconnect();
  }

  bool
  SatPicList::ConnectDB ()
  {
    DisconnectDB();
    return ConnectDB(mServer, mDBName, mUser, mPass);
  }

  void
  SatPicList::SetServer (string server)
  {
    mServer = server;
  }
  
  void
  SatPicList::SetPath (string path)
  {
    DBCon.SetWebPath(path);
    mPath = path;
  }

  void
  SatPicList::SetFilename (string filename)
  {
    mPicfilename = filename;
  }

  void
  SatPicList::SetMethod (const DBConnection::Method m)
  {
    DBCon.SetMethod(m);
  }

  DBConnection::Method
  SatPicList::Method()
  {
    return DBCon.GetMethod();
  }

  void 
  SatPicList::SetDBParams (
       const DBConnection::Method &m,
       string server, 
       string db, 
       string user, 
       string pass, 
       string picname)
  {
    mServer = server;
    mUser   = user;
    mPass   = pass;
    mDBName = db;
    mPicfilename = picname;
    DBCon.SetMethod(m);
  }
  
  void
  SatPicList::SetDBType (const string dbType)
  {
    DBCon.SetDBType (dbType);
  }

  void
  SatPicList::LoadDummy ()
  {
     if (mHaveDummy) {
       return;
     }
     if (pDummyBuf == 0) {
       pDummyBuf = new SatPicBuf (0,string("no image"),
                       string("0"),string("no images available"));
     }
     if (pDummyBuf) {
       mDummyIt = mBufMap.insert
                  (pair<unsigned long int, SatPicBuf*>(0,pDummyBuf));
       mHaveDummy = true;
     }
  }

  void
  SatPicList::LoadFromIndex ()
  {
    IndexRecord rec;
    SatPicBuf * pBuf;
    if (DBCon.Waiting()){
      return;
    }
    while (DBCon.ReadIndexRec( rec)) {
       if (rec.picname == mPicfilename) {
         pBuf = new SatPicBuf (rec.ident, rec.picname,
                             rec.storetime, rec.remark);
         mBufMap.insert(pair<unsigned long int, SatPicBuf*>
                   (rec.ident, pBuf));
       }
     } 
    if (pDC) {
      pDC->IndexWaitWakeup();
    }
  }

  bool
  SatPicList::LoadFromDB()
  {
    bool haveDB = DBCon.ConnectDB (mServer, mDBName, mUser, mPass);
    if (haveDB) {
      SatPicBuf::SetDBCon(&DBCon);
      unsigned long int now = time(NULL);
      unsigned long int fromSec = ( mMaxSecs > now ? 0
                                                   : now - mMaxSecs);
      unsigned long int toSec = now - mMinSecs;
      
      haveDB = DBCon.LoadIndex(mPicfilename, false, fromSec, toSec);
      
      if (haveDB ) {      
        LoadFromIndex();
        haveDB = mBufMap.size() > 0;
      }
    }
    if (!haveDB) { // run without any images
    	LoadDummy();
    }
    ToEnd();
    return haveDB;
  }


  unsigned long int
  SatPicList::NumImages()
  {
    return mBufMap.size();
  }

  SatPicBuf*
  SatPicList::ToStart()
  {
    mIt = mBufMap.begin();
    if (mIt == mDummyIt) {
      mIt++;
    }
    if (mIt == mBufMap.end()) {
      return 0;
    } else {
      return mIt->second;
    }
  }

  SatPicBuf*
  SatPicList::ToEnd()
  {
    mIt = mBufMap.end();
    if (mBufMap.size() > 0 ) {
      mIt--;
    }
    return Current();
  }

  bool
  SatPicList::AtEnd()
  {
    return mIt == mBufMap.end();
  }

  SatPicBuf *
  SatPicList::Current(bool snap)
  {
    if (mBufMap.size() < 1) {
      return 0;
    }
    if (mIt == mBufMap.end() && snap) {
      mIt--;
    }
    if (mIt == mBufMap.end()) {
      return 0;
    }
    SatPicBuf * pBuf = mIt->second;
    if (pBuf == pDummyBuf) {
      return 0;
    }
    return pBuf;
  }

  SatPicBuf *
  SatPicList::PostIncr()
  {
    if (mIt == mDummyIt) { // could be dummy at front
      mIt++;
    }
    if (mIt == mBufMap.end()) {
      return 0;
    }
    SatPicBuf * pBuf = mIt->second;
    mIt++;
    return pBuf;
  }

  SatPicBuf *
  SatPicList::PostDecr()
  {
    if (mIt == mDummyIt) {
      return 0;
    }
    if (mIt == mBufMap.end()) {
      mIt--;
      return 0;
    }
    SatPicBuf * pBuf = mIt->second;
    mIt--;
    return pBuf;
  }

  SatPicBuf *
  SatPicList::Find (unsigned long int sec_stamp)
  {
    PicMap_Type::iterator it;
    it = mBufMap.find(sec_stamp);
    if (it == mBufMap.end()) {
      return 0;
    }
    return it->second;
  }

  SatPicBuf *
  SatPicList::FindSet (unsigned long int sec_stamp)
  {
    PicMap_Type::iterator it;
    it = mBufMap.find(sec_stamp);
    if (it == mBufMap.end()) {
      return 0;
    }
    mIt = it;
    return it->second;
  }
  
  
  SatPicList*
  SatPicList::Instance()
  {
    if (pTheOnly == 0) {
       pTheOnly = new SatPicList;
    }
    return pTheOnly;
   }
   
   void
   SatPicList::Destroy()
   {
     if (pTheOnly) {
       delete pTheOnly;
       pTheOnly = 0;
     }
   }

}
