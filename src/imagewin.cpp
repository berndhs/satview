#include "imagewin.h"
#include "abstractcontrol.h"
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

  ImageWin::ImageWin (AbstractControl * pCon, const bool standalone)
    :pControl(0),
     polyCount(0),
     hasclicked(0),
     painter(0),
     maxOldPoints(100)
  {
    pControl = pCon;
    if (standalone) {   
      setupUi(this);
    } 
    setScaledContents (true);
    updateTimer = new QTimer(this);
    connect (updateTimer, SIGNAL(timeout()), this, SLOT(update()));
    updateTimer->start (100);
  }
  
  ImageWin::ImageWin (QWidget * parent, const bool standalone)
  :pControl (0),
     polyCount(0),
     hasclicked(0),
     painter(0),
     maxOldPoints(100)
  {
    pControl = dynamic_cast<AbstractControl*>(parent);   
    if (standalone) {   
      setupUi(this);
    } else {
      setParent (parent);
    }
    setScaledContents (true);
    updateTimer = new QTimer (this);
    connect (updateTimer, SIGNAL (timeout()), this, SLOT (update()));
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
    SetImage (pImg, false);
    curTag  = tag;
    currentFrame.SetTag(curTag);
    QLabel::update();
  }
  
  void
  ImageWin::SetImage (QImage * pImg, const bool showit)
  {
    pCurImg = pImg;
    QLabel::setPixmap(QPixmap::fromImage(*pImg));
    if (showit) {
      QLabel::update();
    }
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
  ImageWin::DrawText (QPainter * paint) 
  {
    QColor col (255,0,0,255);
    paint->setBrush (col);
    QFont font = paint->font();
    font.setPixelSize (40);
    paint->setFont (font);
    int flags = Qt::AlignLeft | Qt::TextSingleLine;
    paint->drawText (QPoint(50,50), textMessage);
  }

  void
  ImageWin::paintEvent (QPaintEvent *pPE)
  {
    this->QLabel::paintEvent(pPE);
    if (!painter) {
      painter = new QPainter (this);
    } else {
      painter->begin(this);
    }\
    if (textMessage.length() > 0 ) {
      DrawText (painter);
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
