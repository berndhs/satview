


#/****************************************************************
# * This file is distributed under the following license:
# *
# * Copyright (C) 2017, Bernd Stramm
# *
# *  This program is free software; you can redistribute it and/or
# *  modify it under the terms of the GNU General Public License
# *  as published by the Free Software Foundation; either version 2
# *  of the License, or (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program; if not, write to the Free Software
# *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
# *  Boston, MA  02110-1301, USA.
# ****************************************************************/
CONFIG += qt debug_and_release
RESOURCES += ../viewer.qrc

message("version $$QT_MAJOR_VERSION of $$QT_VERSION")
greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets gui
  CONFIG += c++11

  message("QT_MAJOR_VERISON is big")
} else {
  message("QT_MAJOR_VERISON is less or equal 5")
}

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc

INCLUDEPATH += tmp/ui/
message ("wmake for viewer, INCLUDEPATH is $$INCLUDEPATH")

unix:CONFIG(debug, debug|release) {
  DEFINES += DELIBERATE_DEBUG=1
  QMAKE_CXXFLAGS += -Wall
  TARGET = cloudtrackd
}
unix:CONFIG(release, debug|release) {
  DEFINES += SATVIEW_RELEASE
  DEFINES += QT_NO_DEBUG
  DEFINES += DELIBERATE_DEBUG=0
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE += -Wall
  TARGET = cloudtrack
}
win32:CONFIG(debug, debug|release) {
  LIBS += -lQtNetworkd
  DEFINES += DELIBERATE_DEBUG=1
  TARGET = cloudtrackd
}
win32:CONFIG(release, debug|release) {
  DEFINES += SATVIEW_RELEASE
  DEFINES += DELIBERATE_DEBUG=0
  LIBS += -lQtNetwork
  TARGET = cloudtrack
}


TEMPLATE = app

unix {
   QT += core gui network sql
   DISTFILES += userman.txt userman.html
}
win32 {
   QT += network
}
VERSION = 0.5.0

FORMS += \
       ../ui/control.ui  \
       ../ui/imagewin.ui \
       ../ui/getstring.ui \
       ../ui/onepanel.ui \
       ../ui/server.ui \
       ../ui/DebugLog.ui

SOURCES += \  
        ../src/common.cpp \
        ../src/abstractcontrol.cpp \
        ../src/twopanelcontrol.cpp \
        ../src/onepanelcontrol.cpp \
   	    ../src/trackmain.cpp \
        ../src/satpiclist.cpp  \
        ../src/satpicbuf.cpp \
	      ../src/dbconnect.cpp \
        ../src/version.cpp \
        ../src/blob-image.cpp \
	      ../src/imagewin.cpp \
	      ../src/settingsmenu.cpp \
	      ../src/settingserver.cpp \
	../src/textbox.cpp \
	../src/shapeframe.cpp \
	../src/frametypes.cpp \
        ../src/checkpt.cpp \
	../src/clioptions.cpp \
	../src/cmdoptions.cpp \
        ../src/delib-debug.cpp \
        ../src/satviewsplash.cpp \
      


HEADERS += ../src/satpicbuf.h \
           ../src/satpiclist.h \
           ../src/dbconnect.h  \
	         ../src/version.h \
	         ../src/satview-config.h \
	         ../src/blob-image.h \
	         ../src/sjdatasrc.h \
         	../src/satview-defaults.h \
	../src/delib-debug.h \
	../src/abstractcontrol.h \
	../src/onepanelcontrol.h \
	../src/twopanelcontrol.h \
	../src/trackmain.h \
	../src/imagewin.h \
	../src/settingsmenu.h \
	../src/settingserver.h \
	../src/frametypes.h \
	../src/shapeframe.h \
	../src/textbox.h \
        ../src/checkpt.h \
	../src/clioptions.h \
	../src/cmdoptions.h \
        ../src/delib-debug.h \
	../src/berndsutil.h \
        ../src/fault.h \
        ../src/satviewsplash.h


