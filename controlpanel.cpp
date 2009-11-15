
#include "controlpanel.h"

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

#include <iostream>

using namespace std;

namespace satview  {

ControlPanel::ControlPanel (QApplication *pA)
{
    pApp = pA;

    setupUi(this);

    connect (newNameButton, SIGNAL(clicked()), this, SLOT(DoSwitchPicname()));
    connect (quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect (runBackButton, SIGNAL(clicked()), this, SLOT(DoRunBack()));
    connect (backWeekButton, SIGNAL(clicked()), this, SLOT(DoRunBackWeek()));
    connect (backDayButton, SIGNAL(clicked()), this, SLOT(DoRunBackDay()));
    connect (backHoursButton, SIGNAL(clicked()), this, SLOT(DoRunBackHours()));
    connect (backStepButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (runForwardButton, SIGNAL(clicked()), this, SLOT(DoRunFwd()));
    connect (forwardWeekButton, SIGNAL(clicked()), this, SLOT(DoRunFwdWeek()));
    connect (forwardDayButton, SIGNAL(clicked()), this, SLOT(DoRunFwdDay()));
    connect (forwardHoursButton, SIGNAL(clicked()), this, SLOT(DoRUnFwdHours()));
    connect (forwardStepButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (windBackButton, SIGNAL(clicked()), this, SLOT(DoWindBack()));
    connect (windBackWeekButton, SIGNAL(clicked()), this, SLOT(DoWindBackWeek()));
    connect (windBackDayButton, SIGNAL(clicked()), this, SLOT(DoWindBackDay()));
    connect (windBackHoursButton, SIGNAL(clicked()), this, SLOT(DoWindBackHours()));
    connect (windFwdButton, SIGNAL(clicked()), this, SLOT(DoWindFwd()));
    connect (windFwdWeekButton, SIGNAL(clicked()), this, SLOT(DoWindFwdWeek()));
    connect (windFwdDayButton, SIGNAL(clicked()), this, SLOT(DoWindFwdDay()));
    connect (windFwdHoursButton, SIGNAL(clicked()), this, SLOT(DoWindFwdHours()));
    connect (clearAllButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (connectServerButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (reloadServerButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (directConnButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));
    connect (webConnButton, SIGNAL(clicked()), this, SLOT(NotImplemented()));

}

void
ControlPanel::quit()
{
  if (pApp) {
    pApp->quit();
  }
}

void
ControlPanel::NotImplemented ()
{
  cout << "not implemented" << endl;
}

void
ControlPanel::DoRunBack (int secs, bool allway)
{
  NotImplemented();
}

void
ControlPanel::DoRunFwd (int secs, bool allway)
{
  NotImplemented();
}

void
ControlPanel::DoWindBack (int secs, bool allway)
{
  NotImplemented();
}

void
ControlPanel::DoWindFwd (int secs, bool allway)
{
  NotImplemented();
}

}
