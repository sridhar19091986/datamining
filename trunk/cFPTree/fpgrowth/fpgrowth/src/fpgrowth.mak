#-----------------------------------------------------------------------
# File    : fpgrowth.mak
# Contents: build fpgrowth program (on Windows systems)
# Author  : Christian Borgelt
# History : 2004.11.21 file created from eclat.mak
#           2006.07.20 adapted to Visual Studio 8
#           2010.08.22 module escape added (for module tabread)
#-----------------------------------------------------------------------
CC       = cl.exe
LD       = link.exe
DEFS     = /D WIN32 /D NDEBUG /D _CONSOLE /D _MBCS \
           /D _CRT_SECURE_NO_DEPRECATE
CFLAGS   = /nologo /W3 /EHsc /O2 /I $(UTILDIR) /I $(TRACTDIR) \
           $(DEFS) /FD /c
LDFLAGS  = /nologo /subsystem:console /incremental:no /machine:X86

THISDIR  = ..\..\fpgrowth\src
UTILDIR  = ..\..\util\src
TRACTDIR = ..\..\tract\src

HDRS     = $(UTILDIR)\memsys.h    $(UTILDIR)\arrays.h \
           $(UTILDIR)\symtab.h    $(UTILDIR)\escape.h \
           $(UTILDIR)\tabread.h   $(UTILDIR)\scanner.h \
           $(TRACTDIR)\tract.h    $(TRACTDIR)\clomax.h \
           $(TRACTDIR)\report.h
OBJS     = $(UTILDIR)\memsys.obj  $(UTILDIR)\arrays.obj \
           $(UTILDIR)\nimap.obj   $(UTILDIR)\escape.obj \
           $(UTILDIR)\tabread.obj $(UTILDIR)\scform.obj \
           $(TRACTDIR)\tract.obj  $(TRACTDIR)\clomax.obj \
           $(TRACTDIR)\repcm.obj  fpgrowth.obj
PRGS     = fpgrowth.exe

#-----------------------------------------------------------------------
# Build Program
#-----------------------------------------------------------------------
all:         $(PRGS)

fpgrowth.exe:   $(OBJS) fpgrowth.mak
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) /out:$@

#-----------------------------------------------------------------------
# Main Program
#-----------------------------------------------------------------------
fpgrowth.obj:   $(HDRS) fpgrowth.mak
	$(CC) $(CFLAGS) fpgrowth.c /Fo$@

#-----------------------------------------------------------------------
# External Modules
#-----------------------------------------------------------------------
$(UTILDIR)\memsys.obj:
	cd $(UTILDIR)
	$(MAKE) /f util.mak memsys.obj
	cd $(THISDIR)
$(UTILDIR)\arrays.obj:
	cd $(UTILDIR)
	$(MAKE) /f util.mak arrays.obj
	cd $(THISDIR)
$(UTILDIR)\nimap.obj:
	cd $(UTILDIR)
	$(MAKE) /f util.mak nimap.obj
	cd $(THISDIR)
$(UTILDIR)\escape.obj:
	cd $(UTILDIR)
	$(MAKE) /f util.mak escape.obj
	cd $(THISDIR)
$(UTILDIR)\tabread.obj:
	cd $(UTILDIR)
	$(MAKE) /f util.mak tabread.obj
	cd $(THISDIR)
$(UTILDIR)\scform.obj:
	cd $(UTILDIR)
	$(MAKE) /f util.mak scform.obj
	cd $(THISDIR)
$(TRACTDIR)\tract.obj:
	cd $(TRACTDIR)
	$(MAKE) /f tract.mak tract.obj
	cd $(THISDIR)
$(TRACTDIR)\clomax.obj:
	cd $(TRACTDIR)
	$(MAKE) /f tract.mak clomax.obj
	cd $(THISDIR)
$(TRACTDIR)\repcm.obj:
	cd $(TRACTDIR)
	$(MAKE) /f tract.mak repcm.obj
	cd $(THISDIR)

#-----------------------------------------------------------------------
# Install
#-----------------------------------------------------------------------
install:
	-@copy $(PRGS) ..\..\..\bin

#-----------------------------------------------------------------------
# Clean up
#-----------------------------------------------------------------------
clean:
	$(MAKE) /f fpgrowth.mak localclean
	cd $(TRACTDIR)
	$(MAKE) /f tract.mak clean
	cd $(UTILDIR)
	$(MAKE) /f util.mak clean
	cd $(THISDIR)

localclean:
	-@erase /Q *~ *.obj *.idb *.pch $(PRGS)
