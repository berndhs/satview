#ifndef IMAGEWIN_H
#define IMAGEWIN_H
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

#include "ui_imagewin.h"
#include "satpicbuf.h"
#include "frametypes.h"
#include "shapeframe.h"

#include <QImage>
#include <QPoint>
#include <QLine>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QLabel>
#include <deque>

namespace satview {

class AbstractControl;

class ImageWin : public QLabel, public Ui_ImageBox {

Q_OBJECT

  public:

    ImageWin (AbstractControl * pControl, const bool standalone = false);
    ImageWin (QWidget * parent, const bool standalone = false);
    ~ImageWin();

    void SetImage (QImage *pImg, FrameTag tag);
    void SetImage (QImage *pImg, const bool showit = true);
    void SetText (QString txt) {textMessage = txt; }

    void mousePressEvent (QMouseEvent *pME);
    void paintEvent      (QPaintEvent *pPE);
    void ClearTrack      ();
    void ClearFrame      ();
    void FinishPolygon();


  public slots:

    void quit ();
    void update ();


    void NotImplemented ();

  private:
  
    void DrawText (QPainter * paint);

    AbstractControl *pControl;

    QImage       *pCurImg;
    FrameTag      curTag;
    QString       textMessage;
    ShapeFrame    currentFrame;

    ShapeList     shapes;

    unsigned int  polyCount;

    QPoint        clickedSpot;
    PointList     oldPoints;
    LineList      oldLines;
    QPoint        lastSpot;
    int           hasclicked;
    QPainter     *painter;
    unsigned int  maxOldPoints;

    QTimer       *updateTimer;


};

}
#endif
