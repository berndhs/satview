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

MAKEFILE = Makefile

DOC = Doxyfile userman.txt
TOOLS = $(MAKEFILE) satview.fl $(DOC) copydb.cpp

OBJ=$(SRC:.cpp=.o)

FLTKLIB= -lfltk_images -lpng -lz -lfltk

LIBS = -L$(HOME)/lib  \
	 -lberndsutil \
	-L./dcomp \
	-lsatjpeg\
	-L/usr/local/lib \
	-lmysqlcppconn \
	-lccgnu2 \
	 $(FLTKLIB)

INC_PATH =  /usr/local/include/

INC = -I $(INC_PATH)/cppconn/ \
	-I $(HOME)/include

EXTRA_CXX_FLAGS=`fltk-config --cxxflags`

CXX = g++
CXXOPTIM = -O2
CXXFLAGS = -g $(CXXOPTIM) -Wall -Wextra -pedantic -Wno-long-long \
	 $(EXTRA_CXX_FLAGS)
TESTCXXFLAGS= -v -g  -L/usr/lib64 -lfltk_images -lpng -lz -ljpeg -lfltk \
		-I/usr/include/freetype2 \
		 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE \
		-D_THREAD_SAFE -D_REENTRANT



TARGET = viewsat

all: $(TARGET) 

version:
	touch version.h
	make all

$(TARGET): $(OBJ) $(MAKEFILE)
	$(CXX) $(CXXFLAGS) $(OBJ) $(LIBS) -o $(TARGET)

tmptest: tmptest.cpp Makefile
	$(CXX) $(CXXFLAGS) tmptest.cpp $(LIBS)   -o tmptest -lfltk_images -lfltk -lpng -lz -ljpeg

copydb: copydb.o dbconnect.o
	$(CXX) copydb.o dbconnect.o $(LIBS) -o copydb


# special files: these come from FLTK tool fluid

satview.cpp: satview.fl
	fluid -c satview.fl

satview.h: satview.fl
	fluid -c satview.fl

install:
	install $(TARGET) $(HOME)/bin

docu:
	doxygen

userman.html: userman.txt
	asciidoc userman.txt

backup:
	make snapshot

snapshot:
	tar zcvf $(TARGET).`date -u +"%Y%m%d-%H%M%S-u"`.tar.gz $(SRC) $(HDR) $(TOOLS)

tar:
	tar zcvf $(TARGET).tar.gz $(SRC) $(HDR) $(TOOLS)
	ls -l $(TARGET).tar.gz

clean:
	rm $(TARGET) $(OBJ)

depend:  
	echo "" > dep
	for f in $(SRC) ; do \
	   $(CXX)  -M $$f $(INC) >> dep; \
	   done



.SUFFIXES:

%.o: %.cpp $(MAKEFILE)
	$(CXX) -c $(CXXFLAGS) $(INC) $<

include dep
