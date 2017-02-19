#include "checkpt.h"
#include <QTimer>
#include <QAbstractButton>
#include <QPushButton>

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

bool CheckPt::silent(false);

CheckPt::CheckPt (std::string msg)
{
  theMsg = QString(msg.c_str());
}

CheckPt::CheckPt (QString msg)
  :theMsg(msg)
{}

CheckPt::CheckPt (const char * msg)
  :theMsg(msg)
{}

void
CheckPt::DoPopupBox()
{
  if (!silent) {
    QMessageBox box;
    QAbstractButton *okButton = box.addButton(QMessageBox::Ok);
    QAbstractButton *crashButton = box.addButton("Crash",
                                    QMessageBox::ActionRole);
    QAbstractButton *exitButton = box.addButton("Exit",
                                    QMessageBox::ActionRole);
    QAbstractButton *silentButton = box.addButton("Silent",
                                    QMessageBox::ActionRole);
    box.setText("Check: " + theMsg);
    QTimer::singleShot(10000,&box,SLOT(accept()));
    box.exec();
    QAbstractButton *result = box.clickedButton();
    if (result == crashButton) {
       abort();
    }
    if (result == exitButton) {
       exit(1);
    }
    if (result == silentButton) {
       Silent();
    }
  }
}


