
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
TARGET = viewsat
DPENDPATH += .
INCLUDEPATH += .
FORMS += control.ui
SOURCES += satview.cpp viewmain.cpp satpiclist.cpp satpicbuf.cpp callbacks.cpp \
	dbconnect.cpp version.cpp blob-image.cpp sjdatasrc.cpp

HEADERS += satview.h viewmain.h satpicbuf.h satpiclist.h dbconnect.h  \
	version.h satview-config.h blob-image.h sjdatasrc.h \
	berndsutil.h fault.h

DCOMP = ./dcomp
