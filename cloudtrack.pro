
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
unix:CONFIG(debug, debug|release) {
  DEFINES += SATVIEW_DEBUG
  QMAKE_CXXFLAGS += -pedantic -Wextra
  TARGET = cloudtrackd
}
unix:CONFIG(release, debug|release) {
  DEFINES += SATVIEW_RELEASE
  DEFINES += QT_NO_DEBUG
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g
  TARGET = cloudtrack
}
win32:CONFIG(debug, debug|release) {
  LIBS += -lQtNetworkd
  DEFINES += SATVIEW_DEBUG
  TARGET = cloudtrackd
}
win32:CONFIG(release, debug|release) {
  DEFINES += SATVIEW_RELEASE
  LIBS += -lQtNetwork
  TARGET = cloudtrack
}


TEMPLATE = app

unix {
   INCLUDEPATH += /usr/local/include
   LIBPATH += /usr/local/lib
   DISTFILES += userman.txt userman.html
   LIBS += -lQtNetwork -lQtSql -lboost_program_options
}
win32 {
   LIBPATH += $$quote(d:/bernd/software/boost140/lib)
   INCLUDEPATH += $$quote(d:/bernd/software/boost140)
}
VERSION = 0.2.0

FORMS += control.ui imagewin.ui getstring.ui
SOURCES += satpiclist.cpp satpicbuf.cpp \
	dbconnect.cpp version.cpp blob-image.cpp \
	imagewin.cpp \
	trackmain.cpp \
	textbox.cpp \
	shapeframe.cpp \
	frametypes.cpp \
        checkpt.cpp \
	cmdoptions.cpp \
	controlpanel.cpp 


HEADERS += satpicbuf.h satpiclist.h dbconnect.h  \
	version.h satview-config.h blob-image.h sjdatasrc.h \
	satview-defaults.h \
	satview-debug.h \
	controlpanel.h \
	trackmain.h \
	imagewin.h \
	frametypes.h \
	shapeframe.h \
	textbox.h \
        checkpt.h \
	cmdoptions.h \
	berndsutil.h fault.h 

