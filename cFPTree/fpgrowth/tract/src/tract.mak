#-----------------------------------------------------------------------
# File    : tract.mak
# Contents: build item and transaction management (on Windows systems)
# Author  : Christian Borgelt
# History : 2008.10.05 file created from apriori makefile
#-----------------------------------------------------------------------
CC       = cl.exe
LD       = link.exe
DEFS     = /D WIN32 /D NDEBUG /D _CONSOLE /D _MBCS \
           /D _CRT_SECURE_NO_DEPRECATE
CFLAGS   = /nologo /W3 /EHsc /O2 /I $(UTILDIR) $(DEFS) /FD /c
LDFLAGS  = /nologo /subsystem:console /incremental:no /machine:X86

THISDIR  = ..\..\tract\src
UTILDIR  = ..\..\util\src

#-----------------------------------------------------------------------
# Build Module
#-----------------------------------------------------------------------
all:       tract.obj report.obj clomax.obj

#-----------------------------------------------------------------------
# Item and Transaction Management
#-----------------------------------------------------------------------
tract.obj:  tract.h $(UTILDIR)\symtab.h
tract.obj:  tract.c tract.mak
	$(CC) $(CFLAGS) tract.c /Fo$@

#-----------------------------------------------------------------------
# Item Set Reporter Management
#-----------------------------------------------------------------------
report.obj: report.h tract.h $(UTILDIR)\symtab.h
report.obj: report.c tract.mak
	$(CC) $(CFLAGS) report.c /Fo$@

repcm.obj:  report.h tract.h $(UTILDIR)/symtab.h
repcm.obj:  report.c tract.mak
	$(CC) $(CFLAGS) -DISR_CLOMAX report.c /Fo$@

#-----------------------------------------------------------------------
# Prefix Tree Management for Closed/Maximal Filtering
#-----------------------------------------------------------------------
clomax.obj: clomax.h $(UTILDIR)\arrays.h $(UTILDIR)\memsys.h
clomax.obj: clomax.c tract.mak
	$(CC) $(CFLAGS) -c clomax.c /Fo$@

#-----------------------------------------------------------------------
# Clean up
#-----------------------------------------------------------------------
clean:
	-@erase /Q *~ *.obj *.idb *.pch $(PRGS)
