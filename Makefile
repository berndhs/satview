# Makefile for the satview program

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



#
#
#
 
TARGET = cloudtrack
INSTALLDIR = $(HOME)/bin
QMAKE = qmake-qt4
PROJFILE = cloudtrack.pro

ALLSRC = \
blob-image.cpp \
controlpanel.cpp \
copydb.cpp \
dbconnect.cpp \
frametypes.cpp \
imagewin.cpp \
satpicbuf.cpp \
satpiclist.cpp \
shapeframe.cpp \
sjdatasrc.cpp \
textbox.cpp \
trackmain.cpp \
version.cpp 

ALLHDR = \
berndsutil.h \
blob-image.h \
controlpanel.h \
dbconnect.h \
fault.h \
frametypes.h \
imagewin.h \
satpicbuf.h \
satpiclist.h \
satview-config.h \
satview-defaults.h \
shapeframe.h \
sjdatasrc.h \
textbox.h \
trackmain.h \
version.h

ALLUI = control.ui imagewin.ui getstring.ui

ALLMAKE = Makefile cloudtrack.pro copydb.pro

ALLDOC = Doxyfile userman.txt userman.html

ALLARCHIVE = $(ALLSRC) $(ALLHDR) $(ALLUI) $(ALLMAKE) $(ALLDOC)

all: 
	@echo "making qt stuff"; make Make.cloud
	@echo "making target"; make realtarget
	@echo "making copydb"; make copydb

realtarget:
	@echo "using Make.cloud to make target"; make -f Make.cloud all

copydb:
	make -f Make.copydb

tar:
	tar zcvf cloudtrack.tar.gz $(ALLARCHIVE)

clean:
	make -f Make.cloud clean

dist:
	make -f Make.cloud dist

install:
	install -p cloudtrack copydb $(INSTALLDIR)

Make.cloud: $(PROJFILE)
	$(QMAKE) -o Make.cloud $(PROJFILE)

Make.copydb: copydb.pro
	$(QMAKE) copydb.pro

qt:	
	make Make.cloud

docu:
	doxygen
	
