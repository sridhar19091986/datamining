#-----------------------------------------------------------------------
# File    : makefile
# Contents: build fpgrowth program
# Author  : Christian Borgelt (modified by Li Ma)
# History : 2004.11.21 file created from eclat makefile
#           2004.12.10 general memory management system added
#           2009.9.26 modified by Li Ma
#-----------------------------------------------------------------------
CC      = gcc
 CFBASE = -ansi -I$(UTILDIR) -I$(APRIDIR) $(ADDFLAGS)
# CFBASE  = -ansi -Wall -pedantic -I$(UTILDIR) -I$(APRIDIR) $(ADDFLAGS)
# CFLAGS  = $(CFBASE) -DNDEBUG -O3
# CFLAGS  = $(CFBASE) -DNDEBUG -O3 -DBENCH
 CFLAGS  = $(CFBASE) -O3
# CFLAGS  = $(CFBASE) -g
# CFLAGS  = $(CFBASE) -g -DSTORAGE $(ADDINC)
LDFLAGS = 
LIBS    = -lm
# ADDINC  = -I../../misc/src
# ADDOBJ  = storage.o

UTILDIR = ./util/src
APRIDIR = ./apriori/src

HDRS    = $(UTILDIR)/memsys.h $(UTILDIR)/vecops.h \
          $(UTILDIR)/symtab.h $(UTILDIR)/tabscan.h \
          $(UTILDIR)/scan.h   $(APRIDIR)/tract.h fptree.h
OBJS    = $(UTILDIR)/memsys.o $(UTILDIR)/vecops.o \
          $(UTILDIR)/nimap.o  $(UTILDIR)/tabscan.o \
          $(UTILDIR)/scform.o $(APRIDIR)/tract.o  \
          fptree.o fpgrowth.o $(ADDOBJ)

#-----------------------------------------------------------------------
# Build Program
#-----------------------------------------------------------------------
all:        sfpgrowth

sfpgrowth:   $(OBJS) makefile
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

#-----------------------------------------------------------------------
# Main Program
#-----------------------------------------------------------------------
fpgrowth.o: $(HDRS)
fpgrowth.o: fpgrowth.c makefile
	$(CC) $(CFLAGS) -c fpgrowth.c -o $@

#-----------------------------------------------------------------------
# Frequent Pattern Tree Management
#-----------------------------------------------------------------------
fptree.o:   fptree.h
fptree.o:   fptree.c makefile
	$(CC) $(CFLAGS) -c fptree.c -o $@

#-----------------------------------------------------------------------
# External Modules
#-----------------------------------------------------------------------
$(UTILDIR)/memsys.o:
	cd $(UTILDIR); $(MAKE) memsys.o  ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/vecops.o:
	cd $(UTILDIR); $(MAKE) vecops.o  ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/nimap.o:
	cd $(UTILDIR); $(MAKE) nimap.o   ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/tabscan.o:
	cd $(UTILDIR); $(MAKE) tabscan.o ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/scform.o:
	cd $(UTILDIR); $(MAKE) scform.o  ADDFLAGS=$(ADDFLAGS)
$(APRIDIR)/tract.o:
	cd $(APRIDIR); $(MAKE) tract.o   ADDFLAGS=$(ADDFLAGS)

#-----------------------------------------------------------------------
# Storage Debugging
#-----------------------------------------------------------------------
storage.o:  ../../misc/src/storage.h
storage.o:  ../../misc/src/storage.c
	$(CC) $(CFLAGS) -c ../../misc/src/storage.c -o $@

#-----------------------------------------------------------------------
# Install
#-----------------------------------------------------------------------
install:
	cp sfpgrowth $(HOME)/bin

#-----------------------------------------------------------------------
# Clean up
#-----------------------------------------------------------------------
clean:
	rm -f *.o *~ *.flc core sfpgrowth
	cd $(UTILDIR); $(MAKE) clean
	cd $(APRIDIR); $(MAKE) localclean

localclean:
	rm -f *.o *~ *.flc core sfpgrowth
