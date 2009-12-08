
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
unix:LIBS += -lccgnu2 -lQtSql -lboost_program_options
win32:LIBS += -lws2_32
unix:copydb.path = $(HOME)/bin/
unix:INSTALLS += copydb

DEFINES += DO_COPYDB=1

FORMS += 
SOURCES += dbconnect.cpp copydb.cpp cmdoptions.cpp


HEADERS +=  dbconnect.h cmdoptions.h

