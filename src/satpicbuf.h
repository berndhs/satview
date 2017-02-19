
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


#ifndef SATPICBUF_H
#define SATPICBUF_H
#include "satview-config.h"
#include "sjdatasrc.h"

#include <string>

#include "blob-image.h"

#include "dbconnect.h"

#if SATVIEW_USE_QNET

#include <QObject>

#endif

using namespace std;

namespace satview {

  /** @brief This class provides buffers for images, one per object.
   * This is not a container for a bunch of images, just ony.
   */

class SatPicBuf 
#if SATVIEW_USE_QNET
       : public QObject
#endif
                 {

#if SATVIEW_USE_QNET
Q_OBJECT
#endif

 public:

  SatPicBuf();
  ~SatPicBuf();



  SatPicBuf (unsigned long int id, string pn, string st, string rem);

  QImage * Get_Image();  // don't call delete, call Forget_Image instead

  void           Forget_Image();  
  

  unsigned long int Ident()   { return mIdent; }

  int      Serial()           { return mSerial; }
  string   PicName()          { return mPicname; }
  string   TimeStamp()        { return mTimeStamp; }
  string   Remark()           { return mRemark; }
  bool     HaveRecord()       { return mHaveIndex; }
  bool     HaveData()         { return mHaveBlob; }
  bool     ImageLen()         { return mImageLen; }

  void     DitchBufs();
  
  string   String() { return string("SatPicBuf ")
                             + berndsutil::toString(mIdent)
                             + " " + mPicname
                             + " " + mTimeStamp
                             + " " + mRemark
                             + " haveIndex " + berndsutil::toString(mHaveIndex)
                             + " haveBlob " + berndsutil::toString(mHaveBlob)
                             + " imglen " + berndsutil::toString(mImageLen)
                             ;}
  
#if SATVIEW_USE_QNET

  public slots:
  
  void    ReceiveBlob (char * data, quint64 len);
  
  signals:
  
  void    ImageArrival (QImage *pI);
  
#endif
  

 private:

  bool    LoadBlob();
  QImage* UnpackImage();

   static int     SerialCount;
   int            mSerial;
   unsigned long int      mIdent;
   string        mPicname;
   string        mTimeStamp;   // date as string
   string        mRemark;
   bool          mHaveIndex;  // do we have the index record?
   bool          mHaveBlob;   // do we have the compressed image?
   bool          mHaveRGB;    // do we have the displayable image?


   unsigned long int  mImageLen;
   char *             mBlob;
   Blob_Image        *mDisplayBuf;

   friend class SatPicList;

   static void SetDBCon (DBConnection * pDB);

   static DBConnection * pDBCon;


   
};

}
#endif
