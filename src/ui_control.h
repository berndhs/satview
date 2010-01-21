/********************************************************************************
** Form generated from reading UI file 'control.ui'
**
** Created: Wed Jan 20 13:14:22 2010
**      by: Qt User Interface Compiler version 4.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTROL_H
#define UI_CONTROL_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ControlPanel
{
public:
    QPushButton *newNameButton;
    QPushButton *quitButton;
    QGroupBox *backupBox;
    QPushButton *runBackButton;
    QPushButton *backWeekButton;
    QPushButton *backDayButton;
    QPushButton *backStepButton;
    QGroupBox *forwardBox;
    QPushButton *runForwardButton;
    QPushButton *forwardWeekButton;
    QPushButton *forwardDayButton;
    QPushButton *forwardStepButton;
    QGroupBox *windBackBox;
    QPushButton *windBackWeekButton;
    QPushButton *windBackDayButton;
    QPushButton *windBackAllButton;
    QGroupBox *windFwdBox;
    QPushButton *windFwdWeekButton;
    QPushButton *windFwdDayButton;
    QPushButton *windFwdAllButton;
    QPlainTextEdit *picnameBox;
    QPlainTextEdit *idtagBox;
    QLabel *picnameLabel;
    QLabel *idtagLabel;
    QPlainTextEdit *dateBox;
    QLabel *dateLabel;
    QGroupBox *serverGroup;
    QPlainTextEdit *serverBox;
    QPushButton *connectServerButton;
    QPushButton *reloadServerButton;
    QPushButton *changeInterfaceButton;
    QLabel *interfaceLabel;
    QPlainTextEdit *minHoursBox;
    QPlainTextEdit *maxHoursBox;
    QGroupBox *ageGroup;
    QLabel *minLabel;
    QLabel *maxLabel;
    QGroupBox *DrawBox;
    QPushButton *clearTrackButton;
    QPushButton *finishPolygonButton;
    QPushButton *saveFrameButton;
    QCheckBox *staticDraw;
    QPushButton *clearFrameButton;
    QPushButton *stopButton;
    QPushButton *versionButton;
    QPushButton *authorButton;
    QPlainTextEdit *commentBox;
    QPushButton *restartButton;
    QLabel *runStatusLabel;
    QPushButton *reloadImgButton;

    void setupUi(QDialog *ControlPanel)
    {
        if (ControlPanel->objectName().isEmpty())
            ControlPanel->setObjectName(QString::fromUtf8("ControlPanel"));
        ControlPanel->resize(356, 480);
        newNameButton = new QPushButton(ControlPanel);
        newNameButton->setObjectName(QString::fromUtf8("newNameButton"));
        newNameButton->setGeometry(QRect(220, 4, 61, 32));
        QFont font;
        font.setPointSize(8);
        newNameButton->setFont(font);
        quitButton = new QPushButton(ControlPanel);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));
        quitButton->setGeometry(QRect(280, 50, 70, 40));
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(255, 19, 236, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 147, 247, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Light, brush2);
        QBrush brush3(QColor(255, 83, 241, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush3);
        QBrush brush4(QColor(127, 9, 118, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush4);
        QBrush brush5(QColor(170, 12, 157, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        QBrush brush6(QColor(255, 255, 255, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush7(QColor(255, 137, 245, 255));
        brush7.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush7);
        QBrush brush8(QColor(255, 255, 220, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Light, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Inactive, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush7);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Light, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Mid, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush4);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        quitButton->setPalette(palette);
        backupBox = new QGroupBox(ControlPanel);
        backupBox->setObjectName(QString::fromUtf8("backupBox"));
        backupBox->setGeometry(QRect(90, 330, 85, 150));
        runBackButton = new QPushButton(backupBox);
        runBackButton->setObjectName(QString::fromUtf8("runBackButton"));
        runBackButton->setGeometry(QRect(3, 20, 79, 31));
        backWeekButton = new QPushButton(backupBox);
        backWeekButton->setObjectName(QString::fromUtf8("backWeekButton"));
        backWeekButton->setGeometry(QRect(3, 60, 79, 28));
        backDayButton = new QPushButton(backupBox);
        backDayButton->setObjectName(QString::fromUtf8("backDayButton"));
        backDayButton->setGeometry(QRect(3, 90, 79, 28));
        backStepButton = new QPushButton(backupBox);
        backStepButton->setObjectName(QString::fromUtf8("backStepButton"));
        backStepButton->setGeometry(QRect(3, 120, 79, 28));
        forwardBox = new QGroupBox(ControlPanel);
        forwardBox->setObjectName(QString::fromUtf8("forwardBox"));
        forwardBox->setGeometry(QRect(180, 330, 85, 150));
        runForwardButton = new QPushButton(forwardBox);
        runForwardButton->setObjectName(QString::fromUtf8("runForwardButton"));
        runForwardButton->setGeometry(QRect(3, 20, 79, 31));
        forwardWeekButton = new QPushButton(forwardBox);
        forwardWeekButton->setObjectName(QString::fromUtf8("forwardWeekButton"));
        forwardWeekButton->setGeometry(QRect(3, 60, 79, 28));
        forwardDayButton = new QPushButton(forwardBox);
        forwardDayButton->setObjectName(QString::fromUtf8("forwardDayButton"));
        forwardDayButton->setGeometry(QRect(3, 90, 79, 28));
        forwardStepButton = new QPushButton(forwardBox);
        forwardStepButton->setObjectName(QString::fromUtf8("forwardStepButton"));
        forwardStepButton->setGeometry(QRect(3, 120, 79, 28));
        windBackBox = new QGroupBox(ControlPanel);
        windBackBox->setObjectName(QString::fromUtf8("windBackBox"));
        windBackBox->setGeometry(QRect(0, 330, 85, 120));
        windBackWeekButton = new QPushButton(windBackBox);
        windBackWeekButton->setObjectName(QString::fromUtf8("windBackWeekButton"));
        windBackWeekButton->setGeometry(QRect(3, 60, 79, 28));
        windBackDayButton = new QPushButton(windBackBox);
        windBackDayButton->setObjectName(QString::fromUtf8("windBackDayButton"));
        windBackDayButton->setGeometry(QRect(3, 90, 79, 28));
        windBackAllButton = new QPushButton(windBackBox);
        windBackAllButton->setObjectName(QString::fromUtf8("windBackAllButton"));
        windBackAllButton->setGeometry(QRect(3, 20, 79, 28));
        windFwdBox = new QGroupBox(ControlPanel);
        windFwdBox->setObjectName(QString::fromUtf8("windFwdBox"));
        windFwdBox->setGeometry(QRect(270, 330, 85, 120));
        windFwdWeekButton = new QPushButton(windFwdBox);
        windFwdWeekButton->setObjectName(QString::fromUtf8("windFwdWeekButton"));
        windFwdWeekButton->setGeometry(QRect(3, 60, 79, 28));
        windFwdDayButton = new QPushButton(windFwdBox);
        windFwdDayButton->setObjectName(QString::fromUtf8("windFwdDayButton"));
        windFwdDayButton->setGeometry(QRect(3, 90, 79, 28));
        windFwdAllButton = new QPushButton(windFwdBox);
        windFwdAllButton->setObjectName(QString::fromUtf8("windFwdAllButton"));
        windFwdAllButton->setGeometry(QRect(3, 20, 79, 28));
        picnameBox = new QPlainTextEdit(ControlPanel);
        picnameBox->setObjectName(QString::fromUtf8("picnameBox"));
        picnameBox->setGeometry(QRect(60, 4, 161, 32));
        idtagBox = new QPlainTextEdit(ControlPanel);
        idtagBox->setObjectName(QString::fromUtf8("idtagBox"));
        idtagBox->setGeometry(QRect(60, 36, 211, 32));
        picnameLabel = new QLabel(ControlPanel);
        picnameLabel->setObjectName(QString::fromUtf8("picnameLabel"));
        picnameLabel->setGeometry(QRect(10, 4, 51, 31));
        idtagLabel = new QLabel(ControlPanel);
        idtagLabel->setObjectName(QString::fromUtf8("idtagLabel"));
        idtagLabel->setGeometry(QRect(10, 36, 51, 31));
        dateBox = new QPlainTextEdit(ControlPanel);
        dateBox->setObjectName(QString::fromUtf8("dateBox"));
        dateBox->setGeometry(QRect(60, 68, 211, 32));
        dateLabel = new QLabel(ControlPanel);
        dateLabel->setObjectName(QString::fromUtf8("dateLabel"));
        dateLabel->setGeometry(QRect(10, 70, 41, 31));
        serverGroup = new QGroupBox(ControlPanel);
        serverGroup->setObjectName(QString::fromUtf8("serverGroup"));
        serverGroup->setGeometry(QRect(2, 130, 350, 131));
        serverBox = new QPlainTextEdit(serverGroup);
        serverBox->setObjectName(QString::fromUtf8("serverBox"));
        serverBox->setGeometry(QRect(2, 14, 344, 31));
        connectServerButton = new QPushButton(serverGroup);
        connectServerButton->setObjectName(QString::fromUtf8("connectServerButton"));
        connectServerButton->setGeometry(QRect(10, 90, 68, 38));
        reloadServerButton = new QPushButton(serverGroup);
        reloadServerButton->setObjectName(QString::fromUtf8("reloadServerButton"));
        reloadServerButton->setGeometry(QRect(100, 90, 68, 38));
        changeInterfaceButton = new QPushButton(serverGroup);
        changeInterfaceButton->setObjectName(QString::fromUtf8("changeInterfaceButton"));
        changeInterfaceButton->setGeometry(QRect(250, 90, 71, 38));
        interfaceLabel = new QLabel(serverGroup);
        interfaceLabel->setObjectName(QString::fromUtf8("interfaceLabel"));
        interfaceLabel->setGeometry(QRect(180, 100, 61, 20));
        interfaceLabel->setAlignment(Qt::AlignCenter);
        minHoursBox = new QPlainTextEdit(serverGroup);
        minHoursBox->setObjectName(QString::fromUtf8("minHoursBox"));
        minHoursBox->setGeometry(QRect(50, 50, 81, 31));
        minHoursBox->setReadOnly(true);
        maxHoursBox = new QPlainTextEdit(serverGroup);
        maxHoursBox->setObjectName(QString::fromUtf8("maxHoursBox"));
        maxHoursBox->setGeometry(QRect(180, 50, 160, 31));
        maxHoursBox->setReadOnly(true);
        ageGroup = new QGroupBox(serverGroup);
        ageGroup->setObjectName(QString::fromUtf8("ageGroup"));
        ageGroup->setGeometry(QRect(4, 41, 340, 41));
        minLabel = new QLabel(ageGroup);
        minLabel->setObjectName(QString::fromUtf8("minLabel"));
        minLabel->setGeometry(QRect(10, 18, 41, 21));
        maxLabel = new QLabel(ageGroup);
        maxLabel->setObjectName(QString::fromUtf8("maxLabel"));
        maxLabel->setGeometry(QRect(140, 18, 30, 18));
        DrawBox = new QGroupBox(ControlPanel);
        DrawBox->setObjectName(QString::fromUtf8("DrawBox"));
        DrawBox->setGeometry(QRect(2, 260, 251, 71));
        clearTrackButton = new QPushButton(DrawBox);
        clearTrackButton->setObjectName(QString::fromUtf8("clearTrackButton"));
        clearTrackButton->setGeometry(QRect(2, 30, 60, 40));
        finishPolygonButton = new QPushButton(DrawBox);
        finishPolygonButton->setObjectName(QString::fromUtf8("finishPolygonButton"));
        finishPolygonButton->setGeometry(QRect(122, 30, 60, 40));
        saveFrameButton = new QPushButton(DrawBox);
        saveFrameButton->setObjectName(QString::fromUtf8("saveFrameButton"));
        saveFrameButton->setGeometry(QRect(182, 30, 60, 40));
        staticDraw = new QCheckBox(DrawBox);
        staticDraw->setObjectName(QString::fromUtf8("staticDraw"));
        staticDraw->setGeometry(QRect(50, 10, 71, 23));
        clearFrameButton = new QPushButton(DrawBox);
        clearFrameButton->setObjectName(QString::fromUtf8("clearFrameButton"));
        clearFrameButton->setGeometry(QRect(62, 30, 60, 40));
        stopButton = new QPushButton(ControlPanel);
        stopButton->setObjectName(QString::fromUtf8("stopButton"));
        stopButton->setGeometry(QRect(310, 298, 41, 32));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush9(QColor(238, 0, 0, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Button, brush9);
        QBrush brush10(QColor(255, 102, 102, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Light, brush10);
        QBrush brush11(QColor(246, 51, 51, 255));
        brush11.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Midlight, brush11);
        QBrush brush12(QColor(119, 0, 0, 255));
        brush12.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Dark, brush12);
        QBrush brush13(QColor(159, 0, 0, 255));
        brush13.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Mid, brush13);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush);
        palette1.setBrush(QPalette::Active, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Active, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Active, QPalette::Shadow, brush);
        QBrush brush14(QColor(246, 127, 127, 255));
        brush14.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush14);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Active, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Light, brush10);
        palette1.setBrush(QPalette::Inactive, QPalette::Midlight, brush11);
        palette1.setBrush(QPalette::Inactive, QPalette::Dark, brush12);
        palette1.setBrush(QPalette::Inactive, QPalette::Mid, brush13);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Base, brush6);
        palette1.setBrush(QPalette::Inactive, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Inactive, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush14);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::ToolTipText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Button, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Light, brush10);
        palette1.setBrush(QPalette::Disabled, QPalette::Midlight, brush11);
        palette1.setBrush(QPalette::Disabled, QPalette::Dark, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Mid, brush13);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::BrightText, brush6);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush12);
        palette1.setBrush(QPalette::Disabled, QPalette::Base, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Window, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::Shadow, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush9);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipBase, brush8);
        palette1.setBrush(QPalette::Disabled, QPalette::ToolTipText, brush);
        stopButton->setPalette(palette1);
        versionButton = new QPushButton(ControlPanel);
        versionButton->setObjectName(QString::fromUtf8("versionButton"));
        versionButton->setGeometry(QRect(300, 0, 50, 20));
        QFont font1;
        font1.setPointSize(6);
        versionButton->setFont(font1);
        authorButton = new QPushButton(ControlPanel);
        authorButton->setObjectName(QString::fromUtf8("authorButton"));
        authorButton->setGeometry(QRect(300, 20, 50, 20));
        authorButton->setFont(font1);
        commentBox = new QPlainTextEdit(ControlPanel);
        commentBox->setObjectName(QString::fromUtf8("commentBox"));
        commentBox->setGeometry(QRect(60, 100, 211, 32));
        restartButton = new QPushButton(ControlPanel);
        restartButton->setObjectName(QString::fromUtf8("restartButton"));
        restartButton->setGeometry(QRect(280, 97, 70, 36));
        runStatusLabel = new QLabel(ControlPanel);
        runStatusLabel->setObjectName(QString::fromUtf8("runStatusLabel"));
        runStatusLabel->setGeometry(QRect(264, 270, 80, 22));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Sans Serif"));
        font2.setPointSize(10);
        font2.setBold(false);
        font2.setWeight(50);
        runStatusLabel->setFont(font2);
        runStatusLabel->setAutoFillBackground(true);
        runStatusLabel->setFrameShape(QFrame::Panel);
        runStatusLabel->setFrameShadow(QFrame::Plain);
        runStatusLabel->setLineWidth(1);
        runStatusLabel->setPixmap(QPixmap(QString::fromUtf8(":/statusDone.png")));
        runStatusLabel->setAlignment(Qt::AlignCenter);
        reloadImgButton = new QPushButton(ControlPanel);
        reloadImgButton->setObjectName(QString::fromUtf8("reloadImgButton"));
        reloadImgButton->setGeometry(QRect(257, 298, 51, 32));

        retranslateUi(ControlPanel);

        QMetaObject::connectSlotsByName(ControlPanel);
    } // setupUi

    void retranslateUi(QDialog *ControlPanel)
    {
        ControlPanel->setWindowTitle(QApplication::translate("ControlPanel", "CloudTrack Control", 0, QApplication::UnicodeUTF8));
        newNameButton->setText(QApplication::translate("ControlPanel", "Change \n"
"Image", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        quitButton->setToolTip(QApplication::translate("ControlPanel", "Exit the Program", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        quitButton->setText(QApplication::translate("ControlPanel", "Quit", 0, QApplication::UnicodeUTF8));
        backupBox->setTitle(QApplication::translate("ControlPanel", "Back", 0, QApplication::UnicodeUTF8));
        runBackButton->setText(QApplication::translate("ControlPanel", "All", 0, QApplication::UnicodeUTF8));
        backWeekButton->setText(QApplication::translate("ControlPanel", "Week", 0, QApplication::UnicodeUTF8));
        backDayButton->setText(QApplication::translate("ControlPanel", "Day", 0, QApplication::UnicodeUTF8));
        backStepButton->setText(QApplication::translate("ControlPanel", "Step", 0, QApplication::UnicodeUTF8));
        forwardBox->setTitle(QApplication::translate("ControlPanel", "Forward", 0, QApplication::UnicodeUTF8));
        runForwardButton->setText(QApplication::translate("ControlPanel", "All", 0, QApplication::UnicodeUTF8));
        forwardWeekButton->setText(QApplication::translate("ControlPanel", "Week", 0, QApplication::UnicodeUTF8));
        forwardDayButton->setText(QApplication::translate("ControlPanel", "Day", 0, QApplication::UnicodeUTF8));
        forwardStepButton->setText(QApplication::translate("ControlPanel", "Step", 0, QApplication::UnicodeUTF8));
        windBackBox->setTitle(QApplication::translate("ControlPanel", "Wind Back", 0, QApplication::UnicodeUTF8));
        windBackWeekButton->setText(QApplication::translate("ControlPanel", "Week", 0, QApplication::UnicodeUTF8));
        windBackDayButton->setText(QApplication::translate("ControlPanel", "Day", 0, QApplication::UnicodeUTF8));
        windBackAllButton->setText(QApplication::translate("ControlPanel", "Alll", 0, QApplication::UnicodeUTF8));
        windFwdBox->setTitle(QApplication::translate("ControlPanel", "Wind Fwd", 0, QApplication::UnicodeUTF8));
        windFwdWeekButton->setText(QApplication::translate("ControlPanel", "Week", 0, QApplication::UnicodeUTF8));
        windFwdDayButton->setText(QApplication::translate("ControlPanel", "Day", 0, QApplication::UnicodeUTF8));
        windFwdAllButton->setText(QApplication::translate("ControlPanel", "Alll", 0, QApplication::UnicodeUTF8));
        picnameLabel->setText(QApplication::translate("ControlPanel", "Image\n"
"Name", 0, QApplication::UnicodeUTF8));
        idtagLabel->setText(QApplication::translate("ControlPanel", "ID tag\n"
"(secs)", 0, QApplication::UnicodeUTF8));
        dateLabel->setText(QApplication::translate("ControlPanel", "Date", 0, QApplication::UnicodeUTF8));
        serverGroup->setTitle(QApplication::translate("ControlPanel", "Server", 0, QApplication::UnicodeUTF8));
        connectServerButton->setText(QApplication::translate("ControlPanel", "Connect", 0, QApplication::UnicodeUTF8));
        reloadServerButton->setText(QApplication::translate("ControlPanel", "Reload\n"
" Data", 0, QApplication::UnicodeUTF8));
        changeInterfaceButton->setText(QApplication::translate("ControlPanel", "Change\n"
" Interface", 0, QApplication::UnicodeUTF8));
        interfaceLabel->setText(QApplication::translate("ControlPanel", "Interface", 0, QApplication::UnicodeUTF8));
        ageGroup->setTitle(QApplication::translate("ControlPanel", "Age", 0, QApplication::UnicodeUTF8));
        minLabel->setText(QApplication::translate("ControlPanel", "min ", 0, QApplication::UnicodeUTF8));
        maxLabel->setText(QApplication::translate("ControlPanel", "max", 0, QApplication::UnicodeUTF8));
        DrawBox->setTitle(QApplication::translate("ControlPanel", "Draw", 0, QApplication::UnicodeUTF8));
        clearTrackButton->setText(QApplication::translate("ControlPanel", "Clear\n"
"Current", 0, QApplication::UnicodeUTF8));
        finishPolygonButton->setText(QApplication::translate("ControlPanel", "Finish\n"
" Polygon", 0, QApplication::UnicodeUTF8));
        saveFrameButton->setText(QApplication::translate("ControlPanel", "Save \n"
"Frame", 0, QApplication::UnicodeUTF8));
        staticDraw->setText(QApplication::translate("ControlPanel", "Static", 0, QApplication::UnicodeUTF8));
        clearFrameButton->setText(QApplication::translate("ControlPanel", "Clear\n"
"Frame", 0, QApplication::UnicodeUTF8));
        stopButton->setText(QApplication::translate("ControlPanel", "Stop", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        versionButton->setToolTip(QApplication::translate("ControlPanel", "Show Version", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        versionButton->setText(QApplication::translate("ControlPanel", "Version", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        authorButton->setToolTip(QApplication::translate("ControlPanel", "Link to Author's Website", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        authorButton->setText(QApplication::translate("ControlPanel", "Author", 0, QApplication::UnicodeUTF8));
        restartButton->setText(QApplication::translate("ControlPanel", "Restart \n"
"Viewer", 0, QApplication::UnicodeUTF8));
        runStatusLabel->setText(QString());
        reloadImgButton->setText(QApplication::translate("ControlPanel", "Reload\n"
"Image", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class ControlPanel: public Ui_ControlPanel {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTROL_H
