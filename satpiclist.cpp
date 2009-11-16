
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
#include <time.h>
#include <iostream>


namespace satview {

  SatPicList* SatPicList::pTheOnly (0);

  SatPicList::SatPicList()
    :
     pDC(0)
  {
   
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
	  struct tm *pTM;
	  pTM = localtime(&t);
	  memcpy (&theTime,pTM,sizeof(theTime));
#else
      localtime_r (&t, &theTime);
#endif
      const int datelen = 256;
      char plain[datelen+sizeof(void*)];
      int len = strftime (plain, datelen,"%Y-%m-%d %T", &theTime);
      plain[len] = 0;
      pDC->SetDate(plain);
      string identstring = (as_secs + string (" - ")
			    + berndsutil::toString(pBuf->Serial()));
      pDC->SetIdentTag(identstring);
      cout << plain << endl;
      cout << identstring << endl;
    }
  }

  void
  SatPicList::Ditch()
  { 
    PicMap_Type::iterator it = mBufMap.begin();
    SatPicBuf * pBuf;
    while (it != mBufMap.end()) {
      pBuf = it->second;     
      delete pBuf;
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
  SatPicList::LoadDummy ()
  {
     SatPicBuf * pBuf;
     pBuf = new SatPicBuf (0,string("no image"), 
		string("0"), string("no images available"));
     mBufMap.insert(pair<unsigned long int, SatPicBuf*>(0,pBuf));
  }

  bool
  SatPicList::LoadFromDB()
  {
    bool haveDB = DBCon.ConnectDB (mServer, mDBName, mUser, mPass);
    if (haveDB) {
      SatPicBuf::SetDBCon(&DBCon);
      haveDB = DBCon.LoadIndex(mPicfilename);
      
      if (haveDB) {
	IndexRecord rec;
        SatPicBuf * pBuf;
        while (DBCon.ReadIndexRec( rec)) {

          if (rec.picname == mPicfilename) {
            pBuf = new SatPicBuf (rec.ident, rec.picname,
                                rec.storetime, rec.remark);
            mBufMap.insert(pair<unsigned long int, SatPicBuf*>
			 (rec.ident, pBuf));
	  }
        }       
        haveDB = mBufMap.size() > 0;
      }
    }
    if (!haveDB) { // run without any images
	std::cout << " no images " << std::endl;
	LoadDummy();
    }
    return haveDB;
  }


  unsigned long int
  SatPicList::NumImages()
  {
    return mBufMap.size();
  }

  void
  SatPicList::Rewind()
  {
    mIt = mBufMap.begin();
  }

  void
  SatPicList::ToEnd()
  {
    mIt = mBufMap.end();
    mIt--;
  }

  bool
  SatPicList::AtEnd()
  {
    return mIt == mBufMap.end();
  }

  SatPicBuf *
  SatPicList::Current()
  {
    if (mIt == mBufMap.end()) {
      mIt--;
      if (mIt == mBufMap.end()) {  // empty
        return 0;
      }
    }
    return mIt->second;
  }

  SatPicBuf *
  SatPicList::PostIncr()
  {
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
    if (mIt == mBufMap.end()) {
      return 0;
    }
    SatPicBuf * pBuf = mIt->second;
    mIt--;
    return pBuf;
  }

  void
  SatPicList::Skip(int n)
  {
    int i;
    if (n>=0) {
      for (i=1; i<=n; i++) {
	if (mIt == mBufMap.end()) {     
          return;
	}
        mIt++;
      }
    } else {
      for (i= -1; i>=n; i--) {	
        if (mIt == mBufMap.end()) {
          mIt--;
          return;
	}
        mIt--;
      }
    }
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

  void
  SatPicList::Wind (int n)
  {
    SatPicBuf * pBuf = Current();
    unsigned long int cur_time = 0;
    unsigned long int bound = cur_time;
    if (pBuf) {
      cur_time = pBuf->Ident();
      bound = cur_time + n;
    }

    PicMap_Type::iterator it = ( n > 0 
                   ? mBufMap.lower_bound(bound)
		   : mBufMap.upper_bound(bound));      
    if (it != mBufMap.end()) {
      mIt = it;
    }
  }
  
  SatPicList*
  SatPicList::Instance()
  {
    if (!pTheOnly) {      
       pTheOnly = new SatPicList;
    }
    return pTheOnly;
   }

}
