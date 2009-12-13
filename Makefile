
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


QMAKE = qmake-qt4
MAKE = make
RM = rm

MAKE_CLOUD = Make.cloud
MAKE_COPYDB = Make.copydb
ORIGIN_CLOUD = cloud.pro
ORIGIN_COPYDB = copydb.pro

all: copydb cloudtrack cloudtrackd

force:
	$(MAKE) -i ultraclean
	$(MAKE) -B all

fresh:
	$(MAKE) -i clean
	$(MAKE) all

fresh.debug:
	$(MAKE) -i clean.cloud
	$(MAKE) debug

fresh.release:
	$(MAKE) -i clean.cloud
	$(MAKE) release

clean: clean.copydb clean.cloud

ultraclean:
	make -i clean
	$(RM) $(MAKE_CLOUD) $(MAKE_COPYDB)

debug: cloudtrackd

release: cloudtrack

$(MAKE_COPYDB): $(ORIGIN_COPYDB)
	$(QMAKE) $(ORIGIN_COPYDB) -o $(MAKE_COPYDB)

$(MAKE_CLOUD): $(ORIGIN_CLOUD)
	$(QMAKE) $(ORIGIN_CLOUD) -o $(MAKE_CLOUD)


clean.copydb:
	$(MAKE) -f $(MAKE_COPYDB) clean
	$(RM) $(MAKE_COPYDB)

clean.cloud:
	$(MAKE) -f $(MAKE_CLOUD) clean
	$(RM) $(MAKE_CLOUD)

copydb: $(MAKE_COPYDB) $(ORIGIN_COPYDB)
	$(MAKE) -f $(MAKE_COPYDB)

cloudtrack: $(MAKE_CLOUD) $(ORIGIN_CLOUD)
	$(MAKE) -f $(MAKE_CLOUD) release

cloudtrackd: $(MAKE_CLOUD) $(ORIGIN_CLOUD)
	$(MAKE) -f $(MAKE_CLOUD) debug

install:
	$(MAKE) -f make.util install

tar:
	$(MAKE) -f make.snap arch



