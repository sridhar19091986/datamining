#-----------------------------------------------------------------------
# File    : makefile
# Contents: build fpgrowth program (on Unix systems)
# Author  : Christian Borgelt
# History : 2004.11.21 file created from eclat makefile
#           2004.12.10 general memory management system added
#           2008.08.11 adapted to name change from vecops to arrays
#           2009.10.19 external module pfxtree added
#           2010.02.08 external module pfxtree replaced by module clomax
#           2010.08.22 module escape added (for module tabread)
#           2010.10.08 changed standard from -ansi to -std=c99
#-----------------------------------------------------------------------
SHELL    = /bin/bash
CC       = gcc -std=c99
# CC       = g++
CFBASE   = -Wall -pedantic -I$(UTILDIR) -I$(TRACTDIR) $(ADDFLAGS)
CFLAGS   = $(CFBASE) -DNDEBUG -O3
# CFLAGS   = $(CFBASE) -DNDEBUG -O3 -DBENCH
# CFLAGS   = $(CFBASE) -g
# CFLAGS   = $(CFBASE) -g -DSTORAGE $(ADDINC)
LDFLAGS  = 
LIBS     = -lm
# ADDINC   = -I../../misc/src
# ADDOBJ   = storage.o

UTILDIR  = ../../util/src
TRACTDIR = ../../tract/src

HDRS     = $(UTILDIR)/memsys.h  $(UTILDIR)/arrays.h \
           $(UTILDIR)/symtab.h  $(UTILDIR)/escape.h \
           $(UTILDIR)/tabread.h $(UTILDIR)/scanner.h \
           $(TRACTDIR)/tract.h  $(TRACTDIR)/clomax.h \
           $(TRACTDIR)/report.h
OBJS     = $(UTILDIR)/memsys.o  $(UTILDIR)/arrays.o \
           $(UTILDIR)/nimap.o   $(UTILDIR)/escape.o \
           $(UTILDIR)/tabread.o $(UTILDIR)/scform.o \
           $(TRACTDIR)/tract.o  $(TRACTDIR)/clomax.o \
           $(TRACTDIR)/repcm.o  fpgrowth.o $(ADDOBJ)
PRGS     = fpgrowth

#-----------------------------------------------------------------------
# Build Program
#-----------------------------------------------------------------------
all:        $(PRGS)

fpgrowth:   $(OBJS) makefile
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

#-----------------------------------------------------------------------
# Main Program
#-----------------------------------------------------------------------
fpgrowth.o: $(HDRS)
fpgrowth.o: fpgrowth.c makefile
	$(CC) $(CFLAGS) -c fpgrowth.c -o $@

#-----------------------------------------------------------------------
# External Modules
#-----------------------------------------------------------------------
$(UTILDIR)/arrays.o:
	cd $(UTILDIR);  $(MAKE) arrays.o  ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/memsys.o:
	cd $(UTILDIR);  $(MAKE) memsys.o  ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/nimap.o:
	cd $(UTILDIR);  $(MAKE) nimap.o   ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/escape.o:
	cd $(UTILDIR);  $(MAKE) escape.o  ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/tabread.o:
	cd $(UTILDIR);  $(MAKE) tabread.o ADDFLAGS=$(ADDFLAGS)
$(UTILDIR)/scform.o:
	cd $(UTILDIR);  $(MAKE) scform.o  ADDFLAGS=$(ADDFLAGS)
$(TRACTDIR)/tract.o:
	cd $(TRACTDIR); $(MAKE) tract.o   ADDFLAGS=$(ADDFLAGS)
$(TRACTDIR)/clomax.o:
	cd $(TRACTDIR); $(MAKE) clomax.o  ADDFLAGS=$(ADDFLAGS)
$(TRACTDIR)/repcm.o:
	cd $(TRACTDIR); $(MAKE) repcm.o   ADDFLAGS=$(ADDFLAGS)

#-----------------------------------------------------------------------
# Storage Debugging
#-----------------------------------------------------------------------
storage.o:  ../../misc/src/storage.h
storage.o:  ../../misc/src/storage.c
	$(CC) $(CFLAGS) -c ../../misc/src/storage.c -o $@

#-----------------------------------------------------------------------
# Source Distribution Packages
#-----------------------------------------------------------------------
dist:
	cd ../..; rm -f fpgrowth.zip fpgrowth.tar.gz; \
        zip -rq fpgrowth.zip fpgrowth/{src,ex,doc} \
          tract/src/{tract.[ch],report.[ch],clomax.[ch]} \
          tract/src/{makefile,tract.mak} tract/doc \
          util/src/{fntypes.h,arrays.[ch],memsys.[ch],symtab.[ch]} \
          util/src/{escape.[ch],tabread.[ch],scanner.[ch],error.h} \
          util/src/{makefile,util.mak} util/doc; \
        tar cfz fpgrowth.tar.gz fpgrowth/{src,ex,doc} \
          tract/src/{tract.[ch],report.[ch],clomax.[ch]} \
          tract/src/{makefile,tract.mak} tract/doc \
          util/src/{fntypes.h,arrays.[ch],memsys.[ch],symtab.[ch]} \
          util/src/{escape.[ch],tabread.[ch],scanner.[ch],error.h} \
          util/src/{makefile,util.mak} util/doc

#-----------------------------------------------------------------------
# Installation
#-----------------------------------------------------------------------
install:
	cp $(PRGS) $(HOME)/bin

#-----------------------------------------------------------------------
# Clean up
#-----------------------------------------------------------------------
localclean:
	rm -f *.o *~ *.flc core $(PRGS)

clean:
	$(MAKE) localclean
	cd $(UTILDIR);  $(MAKE) clean
	cd $(TRACTDIR); $(MAKE) clean
