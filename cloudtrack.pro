#
#
# top level qmake pro file for cloudtrack
#
#


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
CONFIG += debug_and_release

greaterThan(QT_MAJOR_VERSION, 4) {
  QT += widgets gui core
  CONFIG += c++11
}
UI_DIR = tmp/ui
MOC_DIR = tmp/moc
OBJECTS_DIR = tmp/obj
RCC_DIR = tmp/rcc

INCLUDEPATH += tmp/ui/

SUBDIRS = viewer copier import

TEMPLATE = subdirs

MAKEFILE = Makefile


