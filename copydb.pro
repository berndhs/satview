
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
CONFIG += debug
TEMPLATE = app
TARGET = copydb
MAKEFILE = Make.copydb
VERSION = 0.2.0
DPENDPATH += .
DISFILES += Doxyfile
INCLUDEPATH += /usr/local/include 
LIBPATH += /usr/local/lib
unix:LIBS += -lmysqlcppconn -lccgnu2
win32:LIBS += -lws2_32

FORMS += 
SOURCES += dbconnect.cpp copydb.cpp


HEADERS +=  dbconnect.h

