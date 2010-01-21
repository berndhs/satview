/********************************************************************************
** Form generated from reading UI file 'getstring.ui'
**
** Created: Wed Jan 20 13:14:22 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GETSTRING_H
#define UI_GETSTRING_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_getStringDialog
{
public:
    QDialogButtonBox *buttonBox;
    QPlainTextEdit *textValueBox;
    QLabel *boxLabel;

    void setupUi(QDialog *getStringDialog)
    {
        if (getStringDialog->objectName().isEmpty())
            getStringDialog->setObjectName(QString::fromUtf8("getStringDialog"));
        getStringDialog->resize(380, 106);
        buttonBox = new QDialogButtonBox(getStringDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(140, 50, 221, 41));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        textValueBox = new QPlainTextEdit(getStringDialog);
        textValueBox->setObjectName(QString::fromUtf8("textValueBox"));
        textValueBox->setGeometry(QRect(200, 10, 161, 31));
        boxLabel = new QLabel(getStringDialog);
        boxLabel->setObjectName(QString::fromUtf8("boxLabel"));
        boxLabel->setGeometry(QRect(20, 10, 171, 31));

        retranslateUi(getStringDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), getStringDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), getStringDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(getStringDialog);
    } // setupUi

    void retranslateUi(QDialog *getStringDialog)
    {
        getStringDialog->setWindowTitle(QApplication::translate("getStringDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        boxLabel->setText(QApplication::translate("getStringDialog", "File Name", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class getStringDialog: public Ui_getStringDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GETSTRING_H
