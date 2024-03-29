/*----------------------------------------------------------------------
  File    : fntypes.h
  Contents: definition of some common function types
  Author  : Christian Borgelt
  History : 2008.08.11 file created
            2010.07.31 index comparison function added (ICMPFN)
----------------------------------------------------------------------*/
#ifndef __FNTYPES__
#define __FNTYPES__

typedef void   OBJFN  (void *obj);
typedef int    CMPFN  (const void *p1, const void *p2, void *data);
typedef int    ICMPFN (int i1, int i2, void *data);
typedef double RANDFN (void);

#endif
