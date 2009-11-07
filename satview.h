
//
//  Copyright (C) 2009 - Bernd H Stramm
//


// generated by Fast Light User Interface Designer (fluid) version 1.0109

#ifndef satview_h
#define satview_h
#include <FL/Fl.H>
/** @brief This is generated from FLUID, for the most part.
*/
#include "berndsutil.h"
#define SATVIEW_USE_FLTK 1
#define SATVIEW_USE_SDL 0
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
extern void DitchDB_cb(Fl_Button*, void*);
extern void ReloadDB_cb(Fl_Button*, void*);
extern void ConnectDB_cb(Fl_Button*, void*);
#include <FL/Fl_Input.H>
extern void ServerName_cb(Fl_Input*, void*);
#include <FL/Fl_Round_Button.H>
extern void WebCheck_cb(Fl_Round_Button*, void*);
extern void DirectCheck_cb(Fl_Round_Button*, void*);
extern void Filename_cb(Fl_Input*, void*);
extern void ChangeImage_cb(Fl_Button*, void*);
extern void ChangeDate_cb(Fl_Input*, void*);
extern void Quit_cb(Fl_Button*, void*);
extern void RunForward_cb(Fl_Button*, void*);
extern void RunForTo_cb(Fl_Button*, void*);
extern void Step_cb(Fl_Button*, void*);
extern void RunBack_cb(Fl_Button*, void*);
extern void RunBackTo_cb(Fl_Button*, void*);
extern void RewindAll_cb(Fl_Button*, void*);
extern void RewindTo_cb(Fl_Button*, void*);
extern void WindAll_cb(Fl_Button*, void*);
extern void WindForeTo_cb(Fl_Button*, void*);
#include <FL/Fl_Box.H>

class DisplayControl {
public:
  DisplayControl();
  Fl_Double_Window *Selection;
  Fl_Button *DitchDB;
  Fl_Button *ReloadDB;
  Fl_Button *ConnectDB;
  Fl_Input *ServerName;
  Fl_Round_Button *WebCheck;
  Fl_Round_Button *DirectCheck;
  Fl_Input *Filename;
  Fl_Button *ChangeImage;
  Fl_Input *ChangeDate;
  Fl_Input *Seconds;
  Fl_Button *Quit;
  Fl_Button *RunForward;
  Fl_Button *Run6;
  Fl_Button *RunDay;
  Fl_Button *RunWeek;
  Fl_Button *StepFore;
  Fl_Button *RunBackAll;
  Fl_Button *RunBack6;
  Fl_Button *RunBackDay;
  Fl_Button *RunBackWeek;
  Fl_Button *StepBack;
  Fl_Button *RewindAll;
  Fl_Button *Rewind6;
  Fl_Button *RewindDay;
  Fl_Button *RewindWeek;
  Fl_Button *WindAll;
  Fl_Button *Wind6;
  Fl_Button *WIndDay;
  Fl_Button *WindWeek;
  Fl_Double_Window *ImageWin;
  Fl_Box *PicBox;
};
#endif
