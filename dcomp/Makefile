
CXX = g++
CFLAGS = -g -O2

INFILES = \
jaricom.cpp \
jcomapi.cpp \
jdapimin.cpp \
jdapistd.cpp \
jdarith.cpp \
jdatasrc.cpp \
jdcoefct.cpp \
jdcolor.cpp \
jddctmgr.cpp \
jdhuff.cpp \
jdinput.cpp \
jdmainct.cpp \
jdmarker.cpp \
jdmaster.cpp \
jdmerge.cpp \
jdpostct.cpp \
jdsample.cpp \
jerror.cpp \
jidctflt.cpp \
jidctfst.cpp \
jidctint.cpp \
jmemmgr.cpp \
jmemnobs.cpp \
jquant1.cpp \
jquant2.cpp \
jutils.cpp \
rdcolmap.cpp \
cdjpeg.cpp \
wrgif.cpp

INC = -I. -I$(HOME)/include

SRC = $(INFILES)
HDR = allj.h cderror.h cdjpeg.h jaricom.h jconfig.h jdct.h \
	jerror.h jinclude.h jmemsys.h jmorecfg.h jpegint.h jpeglib.h \
	jversion.h 

OBJ= $(SRC:.cpp=.o)

TARGET = libsatjpeg.a

all: $(TARGET)

$(TARGET): $(OBJ) Makefile
	ar rcs $(TARGET) $(OBJ)

clean:
	rm $(TARGET) $(OBJ)

snapshot:
	tar zcvf $(TARGET).`date -u +"%Y%m%d-%H%M%S-u"`.tar.gz $(SRC) $(HDR) Makefile

.SUFFIXES:

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC) -c $<
