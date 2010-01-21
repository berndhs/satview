
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
CONFIG += debug_and_release

CONFIG(debug, debug|release) {
  TARGET = copydbd
}
CONFIG(release, debug|release) {
  TARGET = copydb
}

TEMPLATE = app
VERSION = 0.3.0
DPENDPATH += .
DISFILES += Doxyfile
INCLUDEPATH += /usr/local/include 
LIBPATH += /usr/local/lib
unix:LIBS += -lccgnu2 -lQtSql -lboost_program_options
win32:LIBS += -lws2_32 -lboost_program_
unix:copydb.path = $(HOME)/bin/
unix:INSTALLS += copydb

DEFINES += DO_COPYDB=1

FORMS += 
SOURCES += ../src/dbconnect.cpp ../src/copydb.cpp ../src/cmdoptions.cpp


HEADERS +=  ../src/dbconnect.h ../src/cmdoptions.h

