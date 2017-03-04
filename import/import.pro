

#/****************************************************************
# * This file is distributed under the following license:
# *
# * Copyright (C) 2017, Bernd Stramm
# *
# *  This program is free software; you can redistribute it and/or
# *  modify it under the terms of the GNU General Public License
# *  as published by the Free Software Foundation; either version 2
# *  of the License, or (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program; if not, write to the Free Software
# *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
# *  Boston, MA  02110-1301, USA.
# ****************************************************************/

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

