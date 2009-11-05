
//
//  Copyright (C) 2009 - Bernd H Stramm
//

#include "satview.h"
#include "satpicbuf.h"
#include "berndsutil.h"
#include "dbconnect.h"


#include "blob-image.h"

#include <sstream>
#include <iostream>
#include <fstream>


namespace satview {

  int SatPicBuf::SerialCount(0);

  SatPicBuf::SatPicBuf()
    :mIdent(0),
     mPicname(""),
     mTimeStamp(""),
     mRemark(""),
     mHaveIndex(false),
     mHaveBlob(false),
     mHaveRGB(false),
     mImageLen(0),
     mBlob(0),
     mDisplayBuf(0)
  {
    mSerial = ++SerialCount;
  }

  SatPicBuf::SatPicBuf (unsigned long int id, string pn, string st, string rem)
    :mIdent(id),
     mPicname(pn),
     mTimeStamp(st),
     mRemark(rem),
     mHaveIndex(true),
     mHaveBlob(false),
     mHaveRGB(false),
     mImageLen(0),
     mBlob(0),
     mDisplayBuf(0)
  {
    mSerial = ++ SerialCount;
  }

  SatPicBuf::~SatPicBuf()
  {
    DitchBufs();
  }

  void     
  SatPicBuf::DitchBufs()
  {  
    if (mHaveRGB) {
      Forget_Image();
    }
    if (mHaveBlob) {
      mHaveBlob = false;
      mImageLen = 0;
      delete[] mBlob;
    }
  }


  void
  SatPicBuf::Forget_Image() // keep blob
  {
    mHaveRGB = false;
    if (mDisplayBuf) {
      delete mDisplayBuf;
      mDisplayBuf = 0;
    }
  }

  bool
  SatPicBuf::LoadBlob()
  {
    if (!pDBCon) { // don't have, can't get it
      return false;
    }

    string id_s = berndsutil::toString(mIdent);

    string simage;
    IndexRecord rec;
    rec.ident = mIdent;
    rec.picname = mPicname;
    mImageLen = pDBCon->ReadImageData (rec, simage);
    if (mImageLen > 0) {
      mBlob = new char[mImageLen + sizeof(void*)];
      memcpy (mBlob, simage.c_str(), mImageLen);
    }
    return mImageLen > 0;
  }

  /** @brief Get_Image - get an RGB image if we have one,
   * otherwise make one from decomressing the blob from the 
   * database. If we don't have the blob either, try to get it.
   */


  Fl_RGB_Image * 
  SatPicBuf::Get_Image(const int delay)
  {
    if (mHaveRGB && (mDisplayBuf != 0)) {
      if (delay > 0) {
	usleep (delay);   // don't go to fast, spoils the movie
      }
      return mDisplayBuf;
    }
    if (!mHaveBlob) {
      if (! mHaveIndex) {
        return 0;
      }
      mHaveBlob = LoadBlob ();
    }
    if (!mHaveBlob) {
      return 0;      // LoadBlob must have failed
    }

    #if SATVIEW_USE_TEMPFILE
    std::ofstream tmpfile;
    string tmpname(TMP_JPG_FILE);
    tmpfile.open(tmpname.c_str(), ios::out | ios::binary);
    tmpfile.write (mBlob,mImageSize);
	if (tmpfile.bad()) {
		cout << "cannot write temp file " << tmpname << endl;
	}
    tmpfile.close();
    #endif 

    sjdata blob_box;
    blob_box.indata = mBlob;
    blob_box.inlen = mImageLen;
    blob_box.cur_pos = 0; 
    mDisplayBuf =  new Blob_Image(blob_box);    
    Fl_RGB_Image * pImage = mDisplayBuf;
    mHaveRGB = (pImage != 0);
    if (delay > 0) {
      usleep (delay);
    }
    return pImage;
  }

  /** @brief This needs access to the Connection, to load the 
   * images that were not cached.
   */

  DBConnection * SatPicBuf::pDBCon(0);

  void SatPicBuf::SetDBCon (DBConnection *pC)
  {
    pDBCon = pC;
  }


}
