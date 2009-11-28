
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
  QMAKE_CXXFLAGS += -pedantic -Wextra
  TARGET = cloudtrackd
}
win32:CONFIG(debug, debug|releast) {
  LIBS += -lws2_32
  TARGET = cloudtrackd
}
unix:CONFIG(release, debug|release) {
  QMAKE_CFLAGS_RELEASE -= -g
  QMAKE_CXXFLAGS_RELEASE -= -g
  TARGET = cloudtrackr
}
win32:CONFIG(release, debug|release) {
  LIBS += -lws2_32
  TARGET = cloudtrackr
}


TEMPLATE = app

unix {
   INCLUDEPATH += /usr/local/include
   LIBPATH += /usr/local/lib
   LIBS += -lmysqlcppconn -lccgnu2 -lQtNetwork
   DISTFILES += userman.txt userman.html
}
win32 {
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
	controlpanel.cpp 


HEADERS += satpicbuf.h satpiclist.h dbconnect.h  \
	version.h satview-config.h blob-image.h sjdatasrc.h \
	satview-defaults.h \
	controlpanel.h \
	trackmain.h \
	imagewin.h \
	frametypes.h \
	shapeframe.h \
	textbox.h \
	berndsutil.h fault.h 

