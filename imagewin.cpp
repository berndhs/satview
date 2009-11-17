#include "imagewin.h"
#include "controlpanel.h"
#include <string>
#include <iostream>
#include <QMessageBox>
#include <QTimer>


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

using namespace std;

namespace satview {

  ImageWin::ImageWin (ControlPanel * pCon)
    :pControl(0),
     hasclicked(0),
     painter(0),
     maxOldPoints(10)
  {
    pControl = pCon;
    setupUi(this);
    updateTimer = new QTimer(this);
    connect (updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer->start (100);
  }

  ImageWin::~ImageWin ()
  {
    quit();
  }

  void
  ImageWin::quit()
  {
    if (updateTimer) {
      updateTimer->stop();
      delete updateTimer;
      updateTimer = 0;
    }
  }

  void
  ImageWin::update()
  {
    QLabel::update();
  }

  void
  ImageWin::NotImplemented ()
  {
    cout << "not implemented" << endl;
    QMessageBox msgBox;
    QTimer::singleShot(15000, &msgBox, SLOT(accept()));
    msgBox.setText("not implemented at this time");
    msgBox.exec();
  }

  void
  ImageWin::ShowImage (QImage * pImg)
  {
    QLabel::setPixmap(QPixmap::fromImage(*pImg));
    QLabel::update();
    QLabel::show();
  }

  void
  ImageWin::mousePressEvent (QMouseEvent *pME)
  {
    if (hasclicked > 0) {
      oldPoints.push_front(clickedSpot);
      if (oldPoints.size() > maxOldPoints) {
        oldPoints.pop_back();
      }
    }
    clickedSpot = pME->pos();
    hasclicked ++;
    if (oldPoints.size() > 0) {
      QLine newSeg (oldPoints.front(), clickedSpot);
      oldLines.push_front (newSeg);
      if (oldLines.size() > maxOldPoints ) {
        oldLines.pop_back();
      }
    }
  }

  void
  ImageWin::ClearTrack ()
  {
    hasclicked = 0;
    oldPoints.clear();
    oldLines.clear();
  }

  void
  ImageWin::paintEvent (QPaintEvent *pPE)
  {
    this->QLabel::paintEvent(pPE);
    if (!painter) {
      painter = new QPainter (this);
    } else {
      painter->begin(this);
    }
    if (hasclicked > 0) {
      painter->save();
      painter->translate(clickedSpot);
      painter->setBrush (QColor(200,0,50,50));
      painter->setPen   (QColor(255,0,0,255));
      painter->drawEllipse(QPoint(0,0),20,20);
      painter->restore();
    }
    if (hasclicked > 1) {
      PointList::iterator pit = oldPoints.begin();
      painter->setBrush (QColor(0,200,50,50));
      painter->setPen   (QColor(0,255,0,255));
      while (pit != oldPoints.end()) {
        painter->save();
	painter->translate(*pit);
        painter->drawEllipse(QPoint(0,0),15,15);
        pit++;
        painter->restore();
      }
      painter->setBrush (QColor(0,50,200,50));
      painter->setPen   (QColor(0,50,255,255));
      LineList::iterator lit = oldLines.begin();
      while (lit != oldLines.end()) {
        painter->save();
        painter->drawLine(*lit);
        painter->restore();
        lit++;
      }
    }
    painter->end();
  }

}
