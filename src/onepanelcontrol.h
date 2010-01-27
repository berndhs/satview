#ifndef ONEPANELCONTROL_H
#define ONEPANELCONTROL_H

//
//  Copyright (C) 2010 - Bernd H Stramm 
//
// This program is distributed under the terms of 
// the GNU General Public License version 3 
//
// This software is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty 
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//


#include "abstractcontrol.h"
#include "ui_singlewin.h"
#include <QPixmap>

namespace satview {

class OnePanelControl: public AbstractControl, public Ui_SingleWin {

Q_OBJECT

public:

  OnePanelControl (QApplication *pA);
  
public slots:

    void DoRunBack ()      { AbstractControl::DoRunBack (0,true); }

    void DoRunFwd ()      { AbstractControl::DoRunFwd (0,true); }

    void DisplayPic (QImage *pImg);

private:

   QPixmap   imagePix;

};

}

#endif
