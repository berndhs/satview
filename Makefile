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

SRC = satview.cpp viewmain.cpp satpiclist.cpp satpicbuf.cpp callbacks.cpp \
	dbconnect.cpp version.cpp blob-image.cpp sjdatasrc.cpp

HDR = satview.h viewmain.h satpicbuf.h satpiclist.h dbconnect.h  \
	version.h satview-config.h blob-image.h sjdatasrc.h

DCOMP = ./dcomp
DCOMPMOD = libsatjpeg
DCOMPLIB = $(DCOMPMOD).a
DCOMPSHIP = $(DCOMP)/*.cpp $(DCOMP)/*.h $(DCOMP)/Makefile

INSTALLDIR=$(HOME)/bin

MAKEFILE = Makefile

DOC = Doxyfile userman.txt COPYING
TOOLS = $(MAKEFILE) satview.fl $(DOC) copydb.cpp

PICS = controlwin.png imagewin.png

ALLSHIP = $(SRC) $(HDR) $(TOOLS) $(DCOMPSHIP) userman.html $(PICS)

OBJ=$(SRC:.cpp=.o)

FLTKLIB= -lfltk_images -lpng -lz -lfltk

LIBS = -L./dcomp \
	-lsatjpeg\
	-L/usr/local/lib \
	-lmysqlcppconn \
	-lccgnu2 \
	 $(FLTKLIB)

INC_PATH =  /usr/local/include/

INC = -I $(INC_PATH)/cppconn/ \

EXTRA_CXX_FLAGS=`fltk-config --cxxflags`

CXX = g++
CXXOPTIM = -O2
CXXFLAGS = -g $(CXXOPTIM) -Wall -Wextra -pedantic -Wno-long-long \
	 $(EXTRA_CXX_FLAGS)

TARGET = viewsat

all: $(DCOMP)/$(DCOMPLIB) $(TARGET)

$(DCOMP)/$(DCOMPLIB):
	cd $(DCOMP); make all

version:
	touch version.h
	make all

$(TARGET): $(OBJ) $(MAKEFILE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $(TARGET)


copydb: copydb.o dbconnect.o
	$(CXX) copydb.o dbconnect.o $(LIBS) -o copydb


# special files: these come from FLTK tool fluid

satview.cpp: satview.fl
	fluid -c satview.fl

satview.h: satview.fl
	fluid -c satview.fl

install:
	install $(TARGET) $(INSTALLDIR)

docu:
	doxygen

userman.html: userman.txt
	asciidoc -n userman.txt

backup:
	make snapshot

snapshot:
	tar zcvf $(TARGET).`date -u +"%Y%m%d-%H%M%S-u"`.tar.gz $(SRC) $(HDR) $(TOOLS)

tar:
	tar zcvf $(TARGET).tar.gz $(SRC) $(HDR) $(TOOLS)
	ls -l $(TARGET).tar.gz

ship:
	tar zcvf $(TARGET).all.tar.gz $(ALLSHIP)

clean:
	rm $(TARGET) $(OBJ)

dep:
	echo "" > dep

depend:  
	echo "" > dep
	for f in $(SRC) ; do \
	   $(CXX)  -M $$f $(INC) >> dep; \
	   done



.SUFFIXES:

%.o: %.cpp $(MAKEFILE)
	$(CXX) -c $(CXXFLAGS) $(INC) $<

include dep
