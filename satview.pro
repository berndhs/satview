
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
LIBS +=

FORMS += control.ui
SOURCES += satview.cpp viewmain.cpp satpiclist.cpp satpicbuf.cpp callbacks.cpp \
	dbconnect.cpp version.cpp blob-image.cpp sjdatasrc.cpp \
dcomp/jaricom.cpp \
dcomp/jcomapi.cpp \
dcomp/jdapimin.cpp \
dcomp/jdapistd.cpp \
dcomp/jdarith.cpp \
dcomp/jdatasrc.cpp \
dcomp/jdcoefct.cpp \
dcomp/jdcolor.cpp \
dcomp/jddctmgr.cpp \
dcomp/jdhuff.cpp \
dcomp/jdinput.cpp \
dcomp/jdmainct.cpp \
dcomp/jdmarker.cpp \
dcomp/jdmaster.cpp \
dcomp/jdmerge.cpp \
dcomp/jdpostct.cpp \
dcomp/jdsample.cpp \
dcomp/jerror.cpp \
dcomp/jidctflt.cpp \
dcomp/jidctfst.cpp \
dcomp/jidctint.cpp \
dcomp/jmemmgr.cpp \
dcomp/jmemnobs.cpp \
dcomp/jquant1.cpp \
dcomp/jquant2.cpp \
dcomp/jutils.cpp \
dcomp/rdcolmap.cpp \
dcomp/cdjpeg.cpp \
dcomp/wrgif.cpp 

HEADERS += satview.h viewmain.h satpicbuf.h satpiclist.h dbconnect.h  \
	version.h satview-config.h blob-image.h sjdatasrc.h \
	berndsutil.h fault.h \
dcomp/allj.h \
dcomp/cderror.h \
dcomp/cdjpeg.h \
dcomp/jaricom.h \
dcomp/jconfig.h \
dcomp/jdct.h \
dcomp/jerror.h \
dcomp/jinclude.h \
dcomp/jmemsys.h \
dcomp/jmorecfg.h \
dcomp/jpegint.h \
dcomp/jpeglib.h \
dcomp/jversion.h 

