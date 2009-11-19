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
QMAKE = qmake-qt4
PROJFILE = cloudtrack.pro

all: 
	@echo "making qt stuff"; make Make.cloud
	@echo "making target"; make realtarget
	@echo "making copydb"; make copydb

realtarget:
	@echo "using Make.cloud to make target"; make -f Make.cloud all

copydb:
	make -f Make.copydb

clean:
	make -f Make.cloud clean

dist:
	make -f Make.cloud dist

Make.cloud: $(PROJFILE)
	$(QMAKE) -o Make.cloud $(PROJFILE)

Make.copydb: copydb.pro
	$(QMAKE) copydb.pro

qt:	
	make Make.cloud

docu:
	doxygen
	
