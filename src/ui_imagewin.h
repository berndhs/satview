/********************************************************************************
** Form generated from reading UI file 'imagewin.ui'
**
** Created: Wed Jan 20 13:14:22 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEWIN_H
#define UI_IMAGEWIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageBox
{
public:

    void setupUi(QWidget *ImageBox)
    {
        if (ImageBox->objectName().isEmpty())
            ImageBox->setObjectName(QString::fromUtf8("ImageBox"));
        ImageBox->resize(640, 480);

        retranslateUi(ImageBox);

        QMetaObject::connectSlotsByName(ImageBox);
    } // setupUi

    void retranslateUi(QWidget *ImageBox)
    {
        ImageBox->setWindowTitle(QApplication::translate("ImageBox", "CloudTrack Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ImageBox: public Ui_ImageBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEWIN_H
