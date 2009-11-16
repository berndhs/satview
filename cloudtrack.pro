
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
CONFIG += qt debug
TEMPLATE = app
TARGET = cloudtrack
DPENDPATH += .
INCLUDEPATH += /usr/local/include dcmop/
LIBPATH += /usr/local/lib
LIBS += -lmysqlcppconn -lccgnu2

FORMS += control.ui imagewin.ui
SOURCES += satpiclist.cpp satpicbuf.cpp \
	dbconnect.cpp version.cpp blob-image.cpp \
	imagewin.cpp \
	trackmain.cpp \
	controlpanel.cpp 
#  sjdatasrc.cpp \


HEADERS += satpicbuf.h satpiclist.h dbconnect.h  \
	version.h satview-config.h blob-image.h sjdatasrc.h \
	controlpanel.h \
	trackmain.h \
	imagewin.h \
	berndsutil.h fault.h 

