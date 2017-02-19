
# //
# //  Copyright (C) 2017 - Bernd H Stramm
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

UI_DIR = tmp/ui
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc

INCLUDEPATH += tmp/ui/

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets gui core
  CONFIG += c++11
}

CONFIG(debug, debug|release) {
  TARGET = dbimportd
  DEFINES += DELIBERATE_DEBUG=1
}
CONFIG(release, debug|release) {
  TARGET = dbimport
  DEFINES += DELIBERATE_DEBUG=0
}

TEMPLATE = app
QT += core gui network sql
FORMS += ../ui/import.ui
VERSION = 0.3.0
DPENDPATH += .
DEFINES += DO_COPYDB=1

SOURCES += ../src/dbconnect.cpp \
	   ../src/common.cpp \
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

