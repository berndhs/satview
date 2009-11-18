#ifndef IMAGEWIN_H
#define IMAGEWIN_H
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

  class ControlPanel;


class ImageWin : public QLabel, public Ui_ImageBox {

Q_OBJECT

  public:

    ImageWin (ControlPanel * pControl);
    ~ImageWin();

    void SetImage (QImage *pImg, FrameTag tag);

    void mousePressEvent (QMouseEvent *pME);
    void paintEvent      (QPaintEvent *pPE);
    void ClearTrack      ();
    void FinishPolygon();


  public slots:

    void quit ();
    void update ();


    void NotImplemented ();

  private:

    ControlPanel *pControl;

    QImage       *pCurImg;
    FrameTag      curTag;

    ShapeFrame    currentFrame;

    ShapeList     shapes;

    unsigned int  polyCount;

    QPoint        clickedSpot;
    PointList     oldPoints;
    LineList      oldLines;
    QPoint        lastSpot;
    int           hasclicked;
    QPainter     *painter;
    int           maxOldPoints;

    QTimer       *updateTimer;


};

}
#endif
