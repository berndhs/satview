#include "imagewin.h"
#include "controlpanel.h"
#include "textbox.h"
#include <string>
#include <iostream>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>

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
     polyCount(0),
     hasclicked(0),
     painter(0),
     maxOldPoints(100)
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
    if (updateTimer) {
      delete updateTimer;
      updateTimer = 0;
    }
    if (painter) {
      delete painter;
      painter = 0;
    }
  }

  void
  ImageWin::quit()
  {
    if (updateTimer) {
      updateTimer->stop();
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
  ImageWin::SetImage (QImage * pImg, FrameTag tag)
  {
    pCurImg = pImg;
    curTag  = tag;
    currentFrame.SetTag(curTag);
    QLabel::setPixmap(QPixmap::fromImage(*pImg));
    QLabel::update();
    QLabel::show();
  }

  void
  ImageWin::mousePressEvent (QMouseEvent *pME)
  {
    Qt::MouseButton button = pME->button();
    switch (button) {
    case  Qt::LeftButton: 
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
        break;
    case Qt::RightButton:
      FinishPolygon();
      break;
    default:
      // ignore other buttons
      break;
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
  ImageWin::ClearFrame ()
  {
    ClearTrack();
    shapes.clear();
  }

   
  void
  ImageWin::FinishPolygon ()
  {
    if (oldPoints.size() > 1) {
      QLine newline(clickedSpot, oldPoints.back());
      oldLines.push_front(newline);
      polyCount++;
      ShapeTag sTag(polyCount);
      TextBox  getTagName(this);
      QString msg("Tag for polygon ");
      QString num(sTag.String().c_str());
      msg.append(num);
      getTagName.SetLabel(msg);
      getTagName.SetDefault (QString(sTag.String().c_str()));
      getTagName.exec();
      QString thename = getTagName.GetText();
      QByteArray bytes=thename.toLatin1();
      
      Polygon poly;
      poly.SetTag(sTag);
      poly.SetName(string(bytes.data()));
      PointList::iterator pit = oldPoints.begin();
      while (pit != oldPoints.end()) {
        poly.AddPoint(*pit);
        pit++;
      }
      poly.AddPoint(clickedSpot);
      shapes.push_front(poly);
      ClearTrack();
      // wait until paintevent to draw this thing
    }
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
    // draw stored shapes
    ShapeList::iterator oldshapes = shapes.begin();
    while (oldshapes != shapes.end()) {
      oldshapes->Draw(this,painter);
      oldshapes++;
    }
    painter->end();
  }

}
