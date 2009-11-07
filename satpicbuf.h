
//
//  Copyright (C) 2009 - Bernd H Stramm
//


#ifndef SATPICBUF_H
#define SATPICBUF_H
#include "satview-config.h"
#include "satview.h"
#include "sjdatasrc.h"

#include <string>

#include <FL/Fl_RGB_Image.h>
#include "blob-image.h"

#include "dbconnect.h"

using namespace std;

namespace satview {

  /** @brief This class provides buffers for images, one per object.
   * This is not a container for a bunch of images, just ony.
   */

class SatPicBuf {

 public:

  SatPicBuf();
  ~SatPicBuf();



  SatPicBuf (unsigned long int id, string pn, string st, string rem);

  Fl_RGB_Image * Get_Image();  // don't call delete, call Forget_Image instead

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

 private:

  bool    LoadBlob();

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
