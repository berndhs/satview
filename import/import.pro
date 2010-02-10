
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
CONFIG += qt app debug_and_release

CONFIG(debug, debug|release) {
  TARGET = importd
  DEFINES += DELIBERATE_DEBUG=1
}
CONFIG(release, debug|release) {
  TARGET = import
  DEFINES += DELIBERATE_DEBUG=0
}

TEMPLATE = app
QT += core gui network sql
FORMS += ../ui/import.ui
VERSION = 0.3.0
DPENDPATH += .
DEFINES += DO_COPYDB=1

SOURCES += ../src/dbconnect.cpp \
           ../src/import.cpp \
           ../src/importmain.cpp \
           ../src/clioptions.cpp \
           ../src/delib-debug.cpp \
           ../src/cmdoptions.cpp


HEADERS +=  ../src/dbconnect.h \
            ../src/clioptions.h \
            ../src/delib-debug.h \
            ../src/import.h \
            ../src/cmdoptions.h

