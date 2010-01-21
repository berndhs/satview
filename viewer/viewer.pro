
# //
# //  Copyright (C) 2009 - Bernd H Stramm
# //
#
# // This program is distributed under the terms of 
# // the GNU General Public License version 3 
# //
# // This software is distributed in the hope that it will be useful, 
# // but WITHOUT ANY WARRANTY; without even the implied warranty 
# // of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
# //

CONFIG += qt debug_and_release
RESOURCES += ../viewer.qrc
MAKEFILE = Make.cloud

unix:CONFIG(debug, debug|release) {
  DEFINES += SATVIEW_DEBUG=1
  QMAKE_CXXFLAGS += -Wall
  TARGET = cloudtrackd
}
unix:CONFIG(release, debug|release) {
  DEFINES += SATVIEW_RELEASE
  DEFINES += QT_NO_DEBUG
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE += -Wall
  TARGET = cloudtrack
}
win32:CONFIG(debug, debug|release) {
  LIBS += -lQtNetworkd
  DEFINES += SATVIEW_DEBUG=1
  TARGET = cloudtrackd
}
win32:CONFIG(release, debug|release) {
  DEFINES += SATVIEW_RELEASE
  LIBS += -lQtNetwork
  TARGET = cloudtrack
}


TEMPLATE = app

unix {
   QT += core gui network sql
   #INCLUDEPATH += /usr/local/include
   #LIBPATH += /usr/local/lib
   DISTFILES += userman.txt userman.html
   LIBS += -lboost_program_options
}
win32 {
   QT += network
   LIBPATH += $$quote(d:/bernd/software/boost140/lib)
   INCLUDEPATH += $$quote(d:/bernd/software/boost140)
}
VERSION = 0.4.0

FORMS += ../ui/control.ui ../ui/imagewin.ui ../ui/getstring.ui

SOURCES += ../src/satpiclist.cpp ../src/satpicbuf.cpp \
	../src/dbconnect.cpp ../src/version.cpp ../src/blob-image.cpp \
	../src/imagewin.cpp \
	../src/trackmain.cpp \
	../src/textbox.cpp \
	../src/shapeframe.cpp \
	../src/frametypes.cpp \
        ../src/checkpt.cpp \
	../src/cmdoptions.cpp \
        ../src/clouddebug.cpp \
	../src/controlpanel.cpp 


HEADERS += ../src/satpicbuf.h ../src/satpiclist.h ../src/dbconnect.h  \
	../src/version.h ../src/satview-config.h ../src/blob-image.h ../src/sjdatasrc.h \
	../src/satview-defaults.h \
	../src/satview-debug.h \
	../src/controlpanel.h \
	../src/trackmain.h \
	../src/imagewin.h \
	../src/frametypes.h \
	../src/shapeframe.h \
	../src/textbox.h \
        ../src/checkpt.h \
	../src/cmdoptions.h \
        ../src/clouddebug.h \
	../src/berndsutil.h ../src/fault.h 

