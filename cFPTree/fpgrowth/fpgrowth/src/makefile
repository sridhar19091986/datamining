#-----------------------------------------------------------------------
# File    : makefile
# Contents: build item and transaction management (on Unix systems)
# Author  : Christian Borgelt
# History : 2008.10.05 file created from apriori makefile
#           2009.10.09 module pfxtree added
#           2010.02.10 module pfxtree replaced by module clomax
#           2010.10.08 changed standard from -ansi to -std=c99
#-----------------------------------------------------------------------
SHELL   = /bin/bash
CC      = gcc -std=c99
# CC      = g++
CFBASE  = -Wall -pedantic -I$(UTILDIR) $(ADDFLAGS)
CFLAGS  = $(CFBASE) -DNDEBUG -O3
# CFLAGS  = $(CFBASE) -DNDEBUG -O3 -DALIGN8
# CFLAGS  = $(CFBASE) -g
# CFLAGS  = $(CFBASE) -g -DALIGN8
# CFLAGS  = $(CFBASE) -g -DSTORAGE $(ADDINC)
LDFLAGS =
LIBS    = -lm
# ADDINC  = -I../../misc/src
# ADDOBJ  = storage.o

UTILDIR = ../../util/src

#-----------------------------------------------------------------------
# Build Module
#-----------------------------------------------------------------------
all:       tract.o report.o repcm.o clomax.o

#-----------------------------------------------------------------------
# Item and Transaction Management
#-----------------------------------------------------------------------
tract.o:   tract.h $(UTILDIR)/symtab.h
tract.o:   tract.c makefile
	$(CC) $(CFLAGS) -c tract.c -o $@

#-----------------------------------------------------------------------
# Item Set Reporter Management
#-----------------------------------------------------------------------
report.o:  report.h tract.h $(UTILDIR)/symtab.h
report.o:  report.c makefile
	$(CC) $(CFLAGS) -c report.c -o $@

repcm.o:   report.h tract.h $(UTILDIR)/symtab.h
repcm.o:   report.c makefile
	$(CC) $(CFLAGS) -DISR_CLOMAX -c report.c -o $@

#-----------------------------------------------------------------------
# Closed and Maximal Frequent Item Set Tree Management
#-----------------------------------------------------------------------
clomax.o:  clomax.h $(UTILDIR)/arrays.h $(UTILDIR)/memsys.h
clomax.o:  clomax.c makefile
	$(CC) $(CFLAGS) -c clomax.c -o $@

#-----------------------------------------------------------------------
# Storage Debugging
#-----------------------------------------------------------------------
storage.o:  ../../misc/src/storage.h
storage.o:  ../../misc/src/storage.c
	$(CC) $(CFLAGS) -c ../../misc/src/storage.c -o $@

#-----------------------------------------------------------------------
# Clean up
#-----------------------------------------------------------------------
clean:
	rm -f *.o *~ *.flc core
