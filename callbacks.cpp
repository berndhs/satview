
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


#include "satview.h"
#include "viewmain.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <stack>

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#if SATVIEW_USE_SDL
#include <SDL.h>
#include <SDL_image.h>
#endif
#if SATVIEW_USE_FLTK
#include <FL/Fl_RGB_Image.h>
#endif

#include "fault.h"
#include "satpiclist.h"
#include "satpicbuf.h"
#include "berndsutil.h"

/** @brief this file contains all the callbacks, they come from the FLTK
 * object.
 */

using namespace satview;
using namespace std;



static std::stack<string> calls;


void DirectCheck_cb (Fl_Round_Button *pW, void *nada)
{
  int val = pW->value();
  Fl_Button * pOther = static_cast<Fl_Button*> (nada);
  if (val != 0) {
    pW->value(1);
    if (pOther) {
      pOther->value(0);
      SatPicList::Instance()->SetMethod(DBConnection::Con_MySqlCPP);
    }
  } else {
    pW->value(0);
    if (pOther) {
      pOther->value(1);
      SatPicList::Instance()->SetMethod(DBConnection::Con_WebSock);
    }
  }
    
}

void WebCheck_cb (Fl_Round_Button *pW, void *nada)
{
  int val = pW->value();
  Fl_Button * pOther = static_cast<Fl_Button*> (nada);
  if (val != 0) {
    pW->value(1);
    if (pOther) {
      pOther->value(0);
      SatPicList::Instance()->SetMethod(DBConnection::Con_WebSock);
    }
  } else {
    pW->value(0);
    if (pOther) {
      pOther->value(1);
      SatPicList::Instance()->SetMethod(DBConnection::Con_MySqlCPP);
    }
  }
}

void DitchDB_cb (Fl_Button *pW, void * nada)
{
  SatPicList::Instance()->Ditch();
  SatPicList::Instance()->Rewind();
}

void ReloadDB_cb (Fl_Button *pW, void *nada)
{
  SatPicList::Instance()->Ditch();
  SatPicList::Instance()->LoadFromDB();
  SatPicList::Instance()->Rewind();
  cout << " list has " << SatPicList::Instance()->NumImages() << " entries" << endl;
}

void ServerName_cb (Fl_Input* pW, void *nada)
{
  SatPicList::Instance()->SetServer(string(pW->value()));
}

void ConnectDB_cb (Fl_Button* pW, void* nada)
{
  SatPicList::Instance()->ConnectDB();
}


/** @brief This is a central point - the function that displays a satellite
 * picture. It is the point of connection between the display implementation
 * and the image database.
 */

void ShowPic (SatPicBuf *pBuf)
{
  static SatPicBuf * pOldBuf(0);

  if (pBuf) {
    Fl_RGB_Image *pI = pBuf->Get_Image();
    if (pI) {
      
      DC.PicBox->image(pI);
      DC.PicBox->redraw();
     
      Fl::flush();

      time_t t = pBuf->Ident();
      DC.Seconds->value((berndsutil::toString(t)
                         + string(" - ")
                         + berndsutil::toString(pBuf->Serial())).c_str() 
                        );
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
      int len = strftime (plain, datelen,"%c", &theTime);
      DC.ChangeDate->value(plain,len);
      DC.Filename->value(pBuf->PicName().c_str());
      DC.Selection->show();
      DC.ImageWin->show();
      if (pOldBuf) {
        if (pOldBuf != pBuf) {
          pOldBuf->Forget_Image();
          pOldBuf = pBuf;
	}
      }     
    }
  }
}

unsigned long int EndTime(long int diff)
{
  SatPicBuf * curBuf = SatPicList::Instance()->Current();
  unsigned long end_t;
  if (curBuf) {
    end_t = curBuf->Ident() + diff;
  } else   if (diff < 0) {
    end_t = 0;
  } else {
    end_t = static_cast<unsigned long int>(~0);
  }
  return end_t;
}

void RunForward_cb (Fl_Button *pW, void * nada)
{
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  while (pBuf) {
    ShowPic(pBuf);
    pBuf = SatPicList::Instance()->PostIncr();
  }
}

void RunForTo_cb (Fl_Button *pW, void * howmany)
{
  berndsutil::Store<int> * pS = static_cast<berndsutil::Store<int>* >(howmany);
  int to = EndTime(pS ? pS->Value() : 0);
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  while (pBuf && (pBuf->Ident() <= to)) {
    ShowPic(pBuf);
    pBuf = SatPicList::Instance()->PostIncr();
  }
  if (!pBuf) { // fell off end
    WindAll_cb(pW,howmany);
  }
}

void RewindAll_cb (Fl_Button *pW, void * nada)
{
  SatPicList::Instance()->Rewind();
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void RunBack_cb (Fl_Button *pW, void * nada)
{
  SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
  while (pBuf) {
    ShowPic(pBuf);
    pBuf = SatPicList::Instance()->PostDecr();
  }
}

void RunBackTo_cb (Fl_Button *pW, void * howmany)
{
  berndsutil::Store<int> * pS = static_cast<berndsutil::Store<int>* >(howmany);
  int to = EndTime(pS ? pS->Value() : 0);
  SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
  while (pBuf && (pBuf->Ident() >= to)) {
    ShowPic(pBuf);
    pBuf = SatPicList::Instance()->PostDecr();
  }
  if (!pBuf) { // fell off end
    RewindAll_cb (pW, howmany);
  }
}
void WindAll_cb  (Fl_Button *pW, void * nada)
{
  SatPicList::Instance()->ToEnd();
  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void RewindTo_cb (Fl_Button *pW, void * howmany)
{
  berndsutil::Store<int> * pS = static_cast<berndsutil::Store<int>* >(howmany);
  int to = EndTime(pS ? pS->Value() : 0);
 SatPicBuf *pBuf = SatPicList::Instance()->PostDecr();
 while (pBuf && (pBuf->Ident() >= to)) {
    pBuf = SatPicList::Instance()->PostDecr();
  }
  if (pBuf) {
    ShowPic(pBuf);
  }
}

void WindForeTo_cb (Fl_Button *pW, void * howmany)
{
  berndsutil::Store<int> * pS = static_cast<berndsutil::Store<int>* >(howmany);
  int to = EndTime(pS ? pS->Value() : 0);
  SatPicBuf *pBuf = SatPicList::Instance()->PostIncr();
  while (pBuf && (pBuf->Ident() <= to)) {
    pBuf = SatPicList::Instance()->PostIncr();
  }
  if (pBuf) {
    ShowPic(pBuf);
  }
}


void Step_cb (Fl_Button *pW, void * howmany)
{
  berndsutil::Store<int> * pS = static_cast<berndsutil::Store<int>* >(howmany);
  int n = (pS ? pS->Value() : 0);
 
  SatPicList::Instance()->Skip(n);

  SatPicBuf *pBuf = SatPicList::Instance()->Current();
  
  if (pBuf) {
    ShowPic(pBuf);
  }

}

void StopMovie_cb (Fl_Button *pW, void * nada)
{
}

void ChangeDate_cb (Fl_Input *pW, void *nada)
{
}

void ChangeSecs_cb (Fl_Input *pW, void *nada)
{
}

void Filename_cb (Fl_Input *pw, void *nada)
{
}

 void ChangeImage_cb (Fl_Button *pW, void *nada)
 {
  string newname (DC.Filename->value());
  SatPicList::Instance()->SetFilename(newname);
  SatPicList::Instance()->Ditch();
  SatPicList::Instance()->LoadFromDB();
  SatPicList::Instance()->Rewind();
  DC.Filename->value(SatPicList::Instance()->Filename().c_str());
  cout << " list has " << SatPicList::Instance()->NumImages() << " entries" << endl;
 }

void  Quit_cb (Fl_Button *pw, void *nada)
{
  exit(0); 
}
