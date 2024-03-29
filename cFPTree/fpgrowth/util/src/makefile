#-----------------------------------------------------------------------
# File    : makefile
# Contents: build utility modules (on Unix systems)
# Author  : Christian Borgelt
# History : 2000.11.04 file created from makefile in table directory
#           2003.06.05 module params added
#           2003.08.12 module nstats added
#           2004.12.10 module memsys added
#           2008.08.01 adapted to name changes of arrays and lists
#           2008.08.18 adapted to main functions of arrays and lists
#           2008.08.22 module escape added, test program trdtest added
#           2010.10.07 changed c standard from -ansi to -std=c99
#           2010.10.08 module tabwrite added
#-----------------------------------------------------------------------
SHELL   = /bin/bash
CC      = gcc -std=c99
# CC      = g++
CFBASE  = -Wall -pedantic $(ADDFLAGS)
CFLAGS  = $(CFBASE) -DNDEBUG -O3
# CFLAGS  = $(CFBASE) -g
# CFLAGS  = $(CFBASE) -g -DSTORAGE $(ADDINC)
# ADDINC  = -I../../misc/src
INC      = -I. -I$(TABLEDIR)
PROGS    = sortargs listtest trdtest

#-----------------------------------------------------------------------
# Build Programs
#-----------------------------------------------------------------------
all:        $(PROGS)

sortargs:   sortargs.o makefile
	$(CC) $(LDFLAGS) $(LIBS) sortargs.o -o $@

listtest:   listtest.o makefile
	$(CC) $(LDFLAGS) $(LIBS) listtest.o -o $@

trdtest:    trdtest.o escape.o arrays.o nimap.o makefile
	$(CC) $(LDFLAGS) $(LIBS) escape.o arrays.o nimap.o trdtest.o -o $@

#-----------------------------------------------------------------------
# Programs
#-----------------------------------------------------------------------
sortargs.o: arrays.h fntypes.h
sortargs.o: arrays.c makefile
	$(CC) $(CFLAGS) -DARRAYS_MAIN -c arrays.c -o $@

listtest.o: lists.h fntypes.h
listtest.o: lists.c makefile
	$(CC) $(CFLAGS) -DLISTS_MAIN -c lists.c -o $@

trdtest.o:  tabread.h
trdtest.o:  tabread.c makefile
	$(CC) $(CFLAGS) -DTRD_MAIN -c tabread.c -o $@

#-----------------------------------------------------------------------
# Array Operations
#-----------------------------------------------------------------------
arrays.o:   arrays.h fntypes.h
arrays.o:   arrays.c makefile
	$(CC) $(CFLAGS) -c arrays.c -o $@

#-----------------------------------------------------------------------
# List Operations
#-----------------------------------------------------------------------
lists.o:    lists.h fntypes.h
lists.o:    lists.c makefile
	$(CC) $(CFLAGS) -c lists.c -o $@

#-----------------------------------------------------------------------
# Memory Management System for Objects of Equal Size
#-----------------------------------------------------------------------
memsys.o:   memsys.h
memsys.o:   memsys.c makefile
	$(CC) $(CFLAGS) -c memsys.c -o $@

#-----------------------------------------------------------------------
# Symbol Table Management
#-----------------------------------------------------------------------
symtab.o:   symtab.h fntypes.h
symtab.o:   symtab.c makefile
	$(CC) $(CFLAGS) -c symtab.c -o $@

nimap.o:    symtab.h fntypes.h arrays.h
nimap.o:    symtab.c makefile
	$(CC) $(CFLAGS) -DNIMAPFN -c symtab.c -o $@

#-----------------------------------------------------------------------
# Numerical Statistics Management
#-----------------------------------------------------------------------
nstats.o:   nstats.h
nstats.o:   nstats.c makefile
	$(CC) $(CFLAGS) -c nstats.c -o $@

nst_pars.o: nstats.h
nst_pars.o: nstats.c makefile
	$(CC) $(CFLAGS) -DNST_PARSE -c nstats.c -o $@

#-----------------------------------------------------------------------
# Escape Character Handling
#-----------------------------------------------------------------------
escape.o:   escape.h
escape.o:   escape.c makefile
	$(CC) $(CFLAGS) -c escape.c -o $@

#-----------------------------------------------------------------------
# Table Reader and Writer Management
#-----------------------------------------------------------------------
tabread.o:  tabread.h escape.h
tabread.o:  tabread.c makefile
	$(CC) $(CFLAGS) -c tabread.c -o $@

tabwrite.o: tabwrite.h escape.h
tabwrite.o: tabwrite.c makefile
	$(CC) $(CFLAGS) -c tabwrite.c -o $@

#-----------------------------------------------------------------------
# Scanner
#-----------------------------------------------------------------------
scform.o:   scanner.h
scform.o:   scanner.c makefile
	$(CC) $(CFLAGS) -c scanner.c -o $@

scanner.o:   scanner.h
scanner.o:   scanner.c makefile
	$(CC) $(CFLAGS) -DSCN_SCAN -c scanner.c -o $@

#-----------------------------------------------------------------------
# Parser Utilities
#-----------------------------------------------------------------------
parser.o:   parser.h
parser.o:   parser.c makefile
	$(CC) $(CFLAGS) -c parser.c -o $@

#-----------------------------------------------------------------------
# Command Line Parameter Retrieval
#-----------------------------------------------------------------------
params.o:   params.h
params.o:   params.c makefile
	$(CC) $(CFLAGS) -c params.c -o $@

#-----------------------------------------------------------------------
# Source Distribution Packages
#-----------------------------------------------------------------------
dist:
	cd ../..; rm -f util.zip util.tar.gz; \
        zip -rq util.zip    util/{src,doc}; \
        tar cfz util.tar.gz util/{src,doc}

#-----------------------------------------------------------------------
# Clean up
#-----------------------------------------------------------------------
clean:
	rm -f $(PROGS) *.o *~ *.flc core
