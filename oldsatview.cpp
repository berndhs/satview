
//
//  Copyright (C) 2009 - Bernd H Stramm
//


// generated by Fast Light User Interface Designer (fluid) version 1.0109

#include "satview.h"
/** @brief This is generated from FLUID, for the most part.
*/
#include "version.h"
#include "berndsutil.h"

DisplayControl::DisplayControl() {
  { Selection = new Fl_Double_Window(340, 410, "Selection");
    Selection->user_data((void*)(this));
    { Filename = new Fl_Input(70, 10, 195, 25, "Image ");
      Filename->callback((Fl_Callback*)Filename_cb);
    } // Fl_Input* Filename
    { ChangeImage = new Fl_Button(270, 5, 60, 35, "Load New");
      ChangeImage->labelsize(10);
      ChangeImage->callback((Fl_Callback*)ChangeImage_cb);
      ChangeImage->align(FL_ALIGN_WRAP);
    } // Fl_Button* ChangeImage
    { ChangeDate = new Fl_Input(70, 60, 195, 25, "Date ");
      ChangeDate->callback((Fl_Callback*)ChangeDate_cb);
    } // Fl_Input* ChangeDate
    { Seconds = new Fl_Input(70, 35, 195, 25, "Seconds ");
    } // Fl_Input* Seconds
    { Quit = new Fl_Button(225, 180, 45, 45, "Quit");
      Quit->box(FL_PLASTIC_UP_BOX);
      Quit->down_box(FL_PLASTIC_DOWN_BOX);
      Quit->color((Fl_Color)4);
      Quit->selection_color((Fl_Color)1);
      Quit->callback((Fl_Callback*)Quit_cb);
    } // Fl_Button* Quit
    { Fl_Group* o = new Fl_Group(15, 95, 200, 215);
      o->box(FL_THIN_UP_BOX);
      o->color(FL_DARK2);
      { Run = new Fl_Button(125, 100, 75, 30, "Run");
        Run->callback((Fl_Callback*)RunForward_cb);
      } // Fl_Button* Run
      { StepForward = new Fl_Button(130, 140, 65, 25, "Step");
        StepForward->callback((Fl_Callback*)Step_cb, (void*)(new berndsutil::Store<int>(1)));
      } // Fl_Button* StepForward
      { StepBack = new Fl_Button(35, 140, 65, 25, "Back");
        StepBack->callback((Fl_Callback*)Step_cb, (void*)(new berndsutil::Store<int>(-1)));
      } // Fl_Button* StepBack
      { RunBack = new Fl_Button(30, 100, 75, 30, "Run Back");
        RunBack->callback((Fl_Callback*)RunBack_cb, (void*)(-1));
      } // Fl_Button* RunBack
      { Rewind = new Fl_Button(25, 275, 78, 30, "To Start");
        Rewind->callback((Fl_Callback*)Rewind_cb);
      } // Fl_Button* Rewind
      { JumpForward = new Fl_Button(115, 175, 95, 30, "+ 6 hours");
        JumpForward->callback((Fl_Callback*)Jump_cb, (void*)(new berndsutil::Store<int>(60*60*6)));
      } // Fl_Button* JumpForward
      { JumpBack = new Fl_Button(20, 175, 95, 30, "- 6 hours");
        JumpBack->callback((Fl_Callback*)Jump_cb, (void*)(new berndsutil::Store<int>(-60*60*6)));
      } // Fl_Button* JumpBack
      { JumpForwardDay = new Fl_Button(115, 205, 95, 30, "+ 1 day");
        JumpForwardDay->callback((Fl_Callback*)Jump_cb, (void*)(new berndsutil::Store<int>(60*60*24)));
      } // Fl_Button* JumpForwardDay
      { JumpBackDay = new Fl_Button(20, 205, 95, 30, "- 1 day");
        JumpBackDay->callback((Fl_Callback*)Jump_cb, (void*)(new berndsutil::Store<int>(-60*60*24)));
      } // Fl_Button* JumpBackDay
      { JumpForwardWeek = new Fl_Button(115, 235, 95, 30, "+ 1 week");
        JumpForwardWeek->callback((Fl_Callback*)Jump_cb, (void*)(new berndsutil::Store<int>(60*60*24*7)));
      } // Fl_Button* JumpForwardWeek
      { JumpBackWeek = new Fl_Button(20, 235, 95, 30, "- 1 week");
        JumpBackWeek->callback((Fl_Callback*)Jump_cb, (void*)(new berndsutil::Store<int>(-60*60*24*7)));
      } // Fl_Button* JumpBackWeek
      { RewindEnd = new Fl_Button(125, 275, 78, 30, "To End");
        RewindEnd->callback((Fl_Callback*)Rewind_End_cb);
      } // Fl_Button* RewindEnd
      o->end();
    } // Fl_Group* o
    { Fl_Group* o = new Fl_Group(5, 315, 330, 85);
      o->box(FL_THIN_UP_BOX);
      o->color((Fl_Color)147);
      { DitchDB = new Fl_Button(20, 355, 65, 40, "Clear Data");
        DitchDB->color(FL_RED);
        DitchDB->selection_color((Fl_Color)208);
        DitchDB->callback((Fl_Callback*)DitchDB_cb);
        DitchDB->align(FL_ALIGN_WRAP);
      } // Fl_Button* DitchDB
      { ReloadDB = new Fl_Button(195, 355, 65, 40, "Reload Data");
        ReloadDB->box(FL_THIN_UP_BOX);
        ReloadDB->color(FL_LIGHT1);
        ReloadDB->callback((Fl_Callback*)ReloadDB_cb);
        ReloadDB->align(FL_ALIGN_WRAP);
      } // Fl_Button* ReloadDB
      { ConnectDB = new Fl_Button(105, 365, 70, 30, "Connect");
        ConnectDB->box(FL_THIN_UP_BOX);
        ConnectDB->callback((Fl_Callback*)ConnectDB_cb);
      } // Fl_Button* ConnectDB
      { ServerName = new Fl_Input(30, 320, 230, 25, "Server");
        ServerName->color((Fl_Color)70);
        ServerName->callback((Fl_Callback*)ServerName_cb);
        ServerName->align(FL_ALIGN_BOTTOM);
        ServerName->when(FL_WHEN_CHANGED);
      } // Fl_Input* ServerName
      { WebCheck = new Fl_Round_Button(265, 330, 70, 15, "Web");
        WebCheck->down_box(FL_ROUND_DOWN_BOX);
        WebCheck->callback((Fl_Callback*)WebCheck_cb, (void*)(DirectCheck));
      } // Fl_Round_Button* WebCheck
      { DirectCheck = new Fl_Round_Button(265, 350, 25, 25, "Direct");
        DirectCheck->down_box(FL_ROUND_DOWN_BOX);
        DirectCheck->callback((Fl_Callback*)DirectCheck_cb, (void*)(WebCheck));
      } // Fl_Round_Button* DirectCheck
      o->end();
    } // Fl_Group* o
    Selection->end();
    Selection->resizable(Selection);
  } // Fl_Double_Window* Selection
  { ImageWin = new Fl_Double_Window(700, 580, "Image");
    ImageWin->user_data((void*)(this));
    { PicBox = new Fl_Box(5, 3, 685, 571);
      Fl_Group::current()->resizable(PicBox);
    } // Fl_Box* PicBox
    ImageWin->end();
  } // Fl_Double_Window* ImageWin
}
