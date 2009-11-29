
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

#include "satpicbuf.h"
#include "berndsutil.h"
#include "dbconnect.h"


#include "blob-image.h"

#include <sstream>
#include <iostream>
#include <fstream>
#include <QDebug>


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
    pDBCon->SetBlobConsumer(this);
    mImageLen = pDBCon->ReadImageData (rec, simage);
    if (mImageLen > 0) {
      mBlob = new char[mImageLen + sizeof(void*)];
      memcpy (mBlob, simage.c_str(), mImageLen);
    }
    return mImageLen > 0;
  }

  void
  SatPicBuf::ReceiveBlob (char * data, size_t len)
  {
    if (len > 0) {
      mImageLen = len;
      mBlob = new char[len + sizeof(void*)];
      memcpy (mBlob, data, len);
      mHaveBlob = true;
    }
  }

  /** @brief Get_Image - get an RGB image if we have one,
   * otherwise make one from decomressing the blob from the 
   * database. If we don't have the blob either, try to get it.
   */


  QImage * 
  SatPicBuf::Get_Image()
  {
    if (mHaveRGB && (mDisplayBuf != 0)) {
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
    QImage * pImage = mDisplayBuf;
    mHaveRGB = (pImage != 0);
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
