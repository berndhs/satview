
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
  DEFINES ++ DELIBERATE_DEBUG=0
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
VERSION = 0.4.0

FORMS += \
       ../ui/control.ui  \
       ../ui/imagewin.ui \
       ../ui/getstring.ui \
       ../ui/onepanel.ui

SOURCES += \  
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
	../src/textbox.cpp \
	../src/shapeframe.cpp \
	../src/frametypes.cpp \
        ../src/checkpt.cpp \
	../src/clioptions.cpp \
	../src/cmdoptions.cpp \
        ../src/delib-debug.cpp \
      


HEADERS += ../src/satpicbuf.h \
           ../src/satpiclist.h \
           ../src/dbconnect.h  \
	         ../src/version.h \
	         ../src/satview-config.h \
	         ../src/blob-image.h \
	         ../src/sjdatasrc.h \
         	../src/satview-defaults.h \
	../src/satview-debug.h \
	../src/abstractcontrol.h \
	../src/onepanelcontrol.h \
	../src/twopanelcontrol.h \
	../src/trackmain.h \
	../src/imagewin.h \
	../src/frametypes.h \
	../src/shapeframe.h \
	../src/textbox.h \
        ../src/checkpt.h \
	../src/clioptions.h \
	../src/cmdoptions.h \
        ../src/delib-debug.h \
	../src/berndsutil.h \
        ../src/fault.h 


