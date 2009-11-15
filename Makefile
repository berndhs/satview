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

all: Make.qt $(TARGET)

$(TARGET): Make.qt Makefile
	make -f Make.qt $(TARGET)

clean:
	make -f Make.qt clean

Make.qt: $(PROJFILE)
	$(QMAKE) -o Make.qt $(PROJFILE)

qt:	Make.qt

docu:
	doxygen
	
