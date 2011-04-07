/*----------------------------------------------------------------------
  File    : tract.c
  Contents: item and transaction management
  Author  : Christian Borgelt
  History : 1996.02.14 file created as apriori.c
            1996.06.24 item reading optimized (function _readitem())
            1996.07.01 adapted to modified module symtab
            1998.01.04 scan functions moved to module tabread
            1998.06.09 array enlargement modified
            1998.06.20 adapted to changed st_create function
            1998.08.07 bug in function ib_read() fixed
            1998.08.08 item appearances added
            1998.08.17 item sorting and recoding added
            1999.10.22 bug in item appearances reading fixed
            1999.11.11 adapted to name/identifier maps
            1999.12.01 check of item appearance added to sort function
            2000.03.15 removal of infrequent items added
            2001.07.14 adapted to modified module tabread
            2001.11.18 transaction functions added to this module
            2001.12.28 first version of this module completed
            2002.01.12 empty field at end of record reported as error
            2002.02.06 item sorting made flexible (different orders)
            2002.02.19 transaction tree functions added
            2003.07.17 function tbg_filter() added (remove unused items)
            2003.08.15 bug in function tt_delete() fixed
            2003.08.21 parameter 'heap' added to function tbg_sort()
            2003.09.20 empty transactions in input made possible
            2003.12.18 padding for 64 bit architecture added
            2004.02.26 item frequency counting moved to ib_read()
            2005.06.20 function _nocmp() for neutral sorting added
            2007.02.13 adapted to modified module tabread
            2008.08.11 item appearance indicator decoding improved
            2008.08.12 considerable redesign, some new functions
            2008.08.14 sentinels added to item arrays in transactions
            2008.11.13 item and transaction sorting generalized
            2008.11.19 transaction tree and tree node separated
            2009.05.28 bug in function tbg_filter() fixed (minimal size)
            2009.08.27 fixed definitions of trans. tree node functions
            2010.03.16 handling of extended transactions added
            2010.07.02 transaction size comparison functions added
            2010.07.13 function tbg_reduce() optimized (size comparison)
            2010.08.10 function tbg_trim() added (for sequences)
            2010.08.11 parameter of ib_read() changed to general mode
            2010.08.13 function tbg_addib() added (add from item base)
            2010.08.19 function ib_readsel() added (item selectors)
            2010.08.22 adapted to modified module tabread
            2010.08.30 handling of empty transactions corrected
            2010.09.13 function tbg_mirror() added (mirror transactions)
            2010.10.08 adapted to modfied module tabread
            2010.12.07 added several explicit type casts (for C++)
            2010.12.15 functions tbg_read() and tbg_recount() added
            2010.12.20 functions tbg_icnts() and tbg_ifrqs() added
            2011.03.20 item reading moved from _readitem() to ib_read()
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "tract.h"
#include "scanner.h"
#ifdef STORAGE
#include "storage.h"
#endif

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define BLKSIZE    1024         /* block size for enlarging arrays */
#define TH_INSERT     8         /* threshold for insertion sort */

#ifdef ALIGN8
#define CHOFF(n)    ((n) +1 -((n) & 1))
#else
#define CHOFF(n)    (n)
#endif
/* The item array offset to the child node array must be odd for */
/* certain 64 bit architectures, because this offset +3 must be an */
/* even number, as there are three int fields before the item array. */

/*----------------------------------------------------------------------
  Constants
----------------------------------------------------------------------*/
static XTITEM SENT = { -1, 0 }; /* sentinel for item arrays */
static int    sorted = 0;       /* flag for sorted indicators */
static const char* appmap[] = { /* item appearance indicators */
  "0:-",  "0:none",  "0:neither", "0:ignore",
  "1:i",  "1:in",    "1:a",  "1:antecedent", "1:b", "1:body",
  "2:o",  "2:out",   "2:c",  "2:consequent", "2:h", "2:head",
  "3:io", "3:inout", "3:ac", "3:a&c", "3:both", "3:bh", "3:b&h",
};                              /* (code:name) */

static const char *emsgs[] = {  /* error messages */
  /* E_NONE      0 */  "no error",
  /* E_NOMEM    -1 */  "not enough memory",
  /* E_FOPEN    -2 */  "cannot open file %s",
  /* E_FREAD    -3 */  "read error on file %s",
  /* E_FWRITE   -4 */  "write error on file %s",
  /*     -5 to  -9 */  NULL, NULL, NULL, NULL, NULL,
  /*    -10 to -14 */  NULL, NULL, NULL, NULL, NULL,
  /* E_NOITEMS -15 */  "no (frequent) items found",
  /* E_ITEMEXP -16 */  "#item expected",
  /* E_ITEMWGT -17 */  "#invalid item weight %s",
  /* E_DUPITEM -18 */  "#duplicate item '%s'",
  /* E_WGTEXP  -19 */  "#transaction weight expected",
  /* E_TAWGT   -20 */  "#invalid transaction weight %s",
  /* E_FLDCNT  -21 */  "#too many fields/columns",
  /* E_APPEXP  -22 */  "#appearance indicator expected",
  /* E_UNKAPP  -23 */  "#unknown appearance indicator '%s'",
  /* E_PENEXP  -24 */  "#insertion penalty expected",
  /* E_PENALTY -25 */  "#invalid insertion penalty %s",
  /*           -26 */  "unknown error"
};

/*----------------------------------------------------------------------
  Global Variables
----------------------------------------------------------------------*/
static char msgbuf[2*TRD_MAXLEN+64];  /* buffer for error messages */

/*----------------------------------------------------------------------
  Auxiliary Functions
----------------------------------------------------------------------*/

static int _appcmp (const void *p1, const void *p2, void *data)
{                               /* --- compare appearance indicators */
  const char *s1 = (const char*)p1 +2;  /* type the two pointers */
  const char *s2 = (const char*)p2 +2;  /* to strings (skip code) */
  int d;                        /* difference of character values */

  for ( ; 1; s1++, s2++) {      /* traverse the strings */
    d = *s1 -*s2;               /* if one string is smaller than */
    if (d   != 0) return d;     /* the other, return the result */
    if (*s1 == 0) return 0;     /* if at the end of the string, */
  }                             /* return 'strings are equal' */
}  /* _appcmp() */

/*--------------------------------------------------------------------*/

static int _appcode (const char *s)
{                               /* --- get appearance indicator code */
  int        i, n;              /* index, number of app. indicators */
  const char *t;                /* to traverse the indicator name */

  assert(s);                    /* check the function argument */
  n = (int)(sizeof(appmap)/sizeof(char*));
  if (!sorted) {                /* if app. indicators not sorted yet */
    ptr_qsort((void*)appmap, n, _appcmp, NULL);
    sorted = -1;                /* sort the appearance indicators */
  }                             /* and set the sorted flag */
  i = ptr_bsearch(s-2, (void*)appmap, n, _appcmp, NULL);
  if (i >= n) return -1;        /* try to find appearance indicator */
  if (i >= 0)                   /* if an exact match was found, */
    return appmap[i][0] -'0';   /* return the app. indicator code */
  t = appmap[i = -1-i] +2;      /* otherwise check for a prefix match */
  while (*s && (*s == *t)) s++, t++;
  if (*s) return -1;            /* if no match, abort the function */
  return appmap[i][0] -'0';     /* return the app. indicator code */
}  /* _appcode() */

/*--------------------------------------------------------------------*/

static int _nocmp (const void *p1, const void *p2, void *data)
{                               /* --- compare item frequencies */
  const ITEM *a = (const ITEM*)p1;    /* type the item pointers */
  const ITEM *b = (const ITEM*)p2;

  if (a->app == APP_NONE) return (b->app == APP_NONE) ? 0 : 1;
  if (b->app == APP_NONE) return -1;
  return a->id -b->id;          /* return sign of identifier diff. */
}  /* _nocmp() */

/*--------------------------------------------------------------------*/

static int _asccmp (const void *p1, const void *p2, void *data)
{                               /* --- compare item frequencies */
  const ITEM *a = (const ITEM*)p1;    /* type the item pointers */
  const ITEM *b = (const ITEM*)p2;

  if (a->app == APP_NONE) return (b->app == APP_NONE) ? 0 : 1;
  if (b->app == APP_NONE) return -1;
  return a->frq -b->frq;        /* return sign of frequency diff. */
}  /* _asccmp() */

/*--------------------------------------------------------------------*/

static int _descmp (const void *p1, const void *p2, void *data)
{                               /* --- compare item frequencies */
  const ITEM *a = (const ITEM*)p1;    /* type the item pointers */
  const ITEM *b = (const ITEM*)p2;

  if (a->app == APP_NONE) return (b->app == APP_NONE) ? 0 : 1;
  if (b->app == APP_NONE) return -1;
  return b->frq -a->frq;        /* return sign of frequency diff. */
}  /* _descmp() */

/*--------------------------------------------------------------------*/

static int _asccmpx (const void *p1, const void *p2, void *data)
{                               /* --- compare item frequencies */
  const ITEM *a = (const ITEM*)p1;    /* type the item pointers */
  const ITEM *b = (const ITEM*)p2;

  if (a->app == APP_NONE) return (b->app == APP_NONE) ? 0 : 1;
  if (b->app == APP_NONE) return -1;
  return a->xfq -b->xfq;        /* return sign of frequency diff. */
}  /* _asccmpx() */

/*--------------------------------------------------------------------*/

static int _descmpx (const void *p1, const void *p2, void *data)
{                               /* --- compare item frequencies */
  const ITEM *a = (const ITEM*)p1;    /* type the item pointers */
  const ITEM *b = (const ITEM*)p2;

  if (a->app == APP_NONE) return (b->app == APP_NONE) ? 0 : 1;
  if (b->app == APP_NONE) return -1;
  return b->xfq -a->xfq;        /* return sign of frequency diff. */
}  /* _descmpx() */

/*----------------------------------------------------------------------
  Item Base Functions
----------------------------------------------------------------------*/

ITEMBASE* ib_create (int ext, int size)
{                               /* --- create an item base */
  ITEMBASE *base;               /* created item base */
  TRACT    *t;                  /* a  standard transaction */
  XTRACT   *x;                  /* an extended transaction */

  if (size <= 0) size = BLKSIZE;/* check and adapt number of items */
  base = (ITEMBASE*)malloc(sizeof(ITEMBASE));
  if (!base) return NULL;       /* create item base and components */
  base->nimap = nim_create(0, 0, (HASHFN*)0, (OBJFN*)0);
  if (!base->nimap) { free(base); return NULL; }
  base->wgt   = 0;              /* initialize the fields */
  base->app   = APP_BOTH;       /* default: appearance in body & head */
  base->pen   = 0.0;            /* default: no item insertion allowed */
  base->isext = ext;            /* flag for extended transactions */
  base->idx   = 0;              /* index of current transaction */
  base->size  = size;           /* size of transaction buffer */
  if (ext) {                    /* if extended transaction */
    base->tract = x = (XTRACT*)malloc(sizeof(XTRACT)
                               +size *sizeof(XTITEM));
    if (x) { x->size = x->wgt = 0; x->items[0] = SENT; } }
  else {                        /* if standard transaction */
    base->tract = t = (TRACT*) malloc(sizeof(TRACT)
                               +size *sizeof(int));
    if (t) { t->size = t->wgt = 0; t->items[0] = -1; }
  }                             /* clear the transaction buffer */
  if (!base->tract) { ib_delete(base); return NULL; }
  base->err = 0;                /* clear the error code and */
  base->trd = NULL;             /* the table/transaction reader */
  return base;                  /* return the created item set */
}  /* ib_create() */

/*--------------------------------------------------------------------*/

void ib_delete (ITEMBASE *base)
{                               /* --- delete an item set */
  assert(base);                 /* check the function argument */
  if (base->tract) free(base->tract);
  if (base->nimap) nim_delete(base->nimap);
  free(base);                   /* delete the components */
}  /* ib_delete() */            /* and the item base body */

/*--------------------------------------------------------------------*/

int ib_add (ITEMBASE *base, const char *name)
{                               /* --- add an item to the set */
  ITEM *item;                   /* to access the item data */

  assert(base                   /* check the function arguments */
  &&     name && (strlen(name) < TA_MAXLEN));
  item = (ITEM*)nim_add(base->nimap, name, sizeof(ITEM));
  if (item == NULL)   return -1;/* add the new item */
  if (item == EXISTS) return -2;/* to the name/id map */
  item->app = base->app;        /* init. the appearance indicator */
  item->idx = item->xfq = item->frq = 0;
  item->pen = base->pen;        /* clear counters and trans. index, */
  return item->id;              /* init. the insertion penalty and */
}  /* ib_add() */               /* return the item identifier */

/*--------------------------------------------------------------------*/

int ib_readsel (ITEMBASE *base, TABREAD *trd)
{                               /* --- read item selection */
  int  d;                       /* delimiter type */
  char *b;                      /* buffer for a field */
  ITEM *item;                   /* to access the item data */

  assert(base && trd);          /* check the function arguments */
  base->trd = trd;              /* note the table reader and set */
  base->app = APP_NONE;         /* the default appearance indicator */
  while (1) {                   /* read item/indicator pairs */
    d = trd_read(trd);          /* read the next item */
    if (d <= TRD_ERR)   return base->err = E_FREAD;
    if (d <= TRD_EOF)   return base->err = nim_cnt(base->nimap);
    b = trd_field(trd);         /* check whether an item was read */
    if (!*b)            return base->err = E_ITEMEXP;
    item = (ITEM*)nim_add(base->nimap, b, sizeof(ITEM));
    if (item == NULL)   return base->err = E_NOMEM;
    if (item == EXISTS) continue;     /* add the new item */
    item->app = APP_BOTH;       /* init. the appearance indicator */
    item->idx = item->xfq = item->frq = 0;
    item->pen = base->pen;      /* clear counters and trans. index */
  }                             /* and init. the insertion penalty */
}  /* ib_readsel() */

/*--------------------------------------------------------------------*/

int ib_readapp (ITEMBASE *base, TABREAD *trd)
{                               /* --- read appearance indicators */
  int  d, i;                    /* delimiter type, app. indicator */
  char *b;                      /* buffer for a field */
  ITEM *item;                   /* to access the item data */

  assert(base && trd);          /* check the function arguments */
  base->trd = trd;              /* note the table reader */
  d = trd_read(trd);            /* read the first record */
  if (d <= TRD_ERR)     return base->err = E_FREAD;
  if (d != TRD_REC)     return base->err = E_FLDCNT;
  i = _appcode(trd_field(trd)); /* get and check appearance code */
  if (i < 0)            return base->err = E_UNKAPP;
  base->app = i;                /* store default appearance indicator */
  while (1) {                   /* read item/indicator pairs */
    d = trd_read(trd);          /* read the next item */
    if (d <= TRD_ERR)   return base->err = E_FREAD;
    if (d <= TRD_EOF)   return base->err = nim_cnt(base->nimap);
    b = trd_field(trd);         /* check whether an item was read */
    if (!*b)            return base->err = E_ITEMEXP;
    item = (ITEM*)nim_add(base->nimap, b, sizeof(ITEM));
    if (item == NULL)   return base->err = E_NOMEM;
    if (item == EXISTS) return base->err = E_DUPITEM;
    item->app = base->app;      /* init. the appearance indicator */
    item->idx = item->xfq = item->frq = 0;
    item->pen = base->pen;      /* clear counters and trans. index */
    if (d != TRD_FLD)   return base->err = E_APPEXP;
    d = trd_read(trd);          /* read the item appearance code */
    if (d <= TRD_ERR)   return base->err = E_FREAD;
    if (d == TRD_FLD)   return base->err = E_FLDCNT;
    i = _appcode(trd_field(trd));
    if (i < 0)          return base->err = E_UNKAPP;
    item->app = i;              /* get, check and store */
  }                             /* the appearance indicator */
}  /* ib_readapp() */

/*--------------------------------------------------------------------*/

int ib_readpen (ITEMBASE *base, TABREAD *trd)
{                               /* --- read insertion penalties */
  int    d;                     /* delimiter type */
  double p;                     /* item insertion penalty */
  char   *b;                    /* buffer for a field, end pointer */
  ITEM   *item;                 /* to access the item data */

  assert(base && trd);          /* check the function arguments */
  base->trd = trd;              /* note the table reader */
  d = trd_read(trd);            /* read the first record */
  if (d <= TRD_ERR)     return base->err = E_FREAD;
  if (d != TRD_REC)     return base->err = E_FLDCNT;
  p = strtod(trd_field(trd),&b);/* get and check default penalty */
  if (*b || (p > 1))    return base->err = E_PENALTY;
  if (p < 0) { base->app = APP_NONE; p = 0; }
  else       { base->app = APP_BOTH; }
  base->pen = p;                /* store default insertion penalty */
  while (1) {                   /* read item/indicator pairs */
    d = trd_read(trd);          /* read the next item */
    if (d <= TRD_ERR)   return base->err = E_FREAD;
    if (d <= TRD_EOF)   return base->err = nim_cnt(base->nimap);
    b = trd_field(trd);         /* check whether an item was read */
    if (!*b)            return base->err = E_ITEMEXP;
    item = (ITEM*)nim_add(base->nimap, b, sizeof(ITEM));
    if (item == NULL)   return base->err = E_NOMEM;
    if (item == EXISTS) return base->err = E_DUPITEM;
    item->app = base->app;      /* init. the appearance indicator */
    item->idx = item->xfq = item->frq = 0;
    item->pen = base->pen;      /* clear counters and trans. index */
    if (d != TRD_FLD)   return base->err = E_PENEXP;
    d = trd_read(trd);          /* read the insertion penalty */
    if (d <= TRD_ERR)   return base->err = E_FREAD;
    if (d == TRD_FLD)   return base->err = E_FLDCNT;
    p = strtod(trd_field(trd), &b);
    if (*b || (p > 1))  return base->err = E_PENALTY;
    if (p < 0) { item->app = APP_NONE; p = 0; }
    else       { item->app = APP_BOTH; }
    item->pen = p;              /* store item appearence indicator */
  }                             /* and the insertion penalty */
}  /* ib_readpen() */

/*--------------------------------------------------------------------*/

int ib_read (ITEMBASE *base, TABREAD *trd, int mode)
{                               /* --- read a transaction */
  int    i, w;                  /* loop variable/buffer, item weight */
  int    d, n;                  /* delimiter type, array size */
  char   *b, *e;                /* read buffer and end pointer */
  ITEM   *item;                 /* to access the item data */
  XTITEM *xitm;                 /* to access the transaction item */
  TRACT  *t;                    /* to access the standard transaction */
  XTRACT *x;                    /* to access the extended transaction */

  assert(base && trd);          /* check the function arguments */
  base->trd = trd;              /* note the table reader */
  ++base->idx;                  /* update the transaction index and */
  x = (XTRACT*)base->tract;     /* get the transaction buffers */
  t = (TRACT*) base->tract;     /* set the default transaction weight */
  t->wgt = 1; t->size = 0;      /* andd init. the transaction size */
  do {                          /* read the transaction items */
    d = trd_read(trd);          /* read the next field (item name) */
    if (d <= TRD_ERR) return base->err = E_FREAD;
    if (d <= TRD_EOF) return base->err = 1;
    b = trd_field(trd);         /* check whether field is empty */
    if ((d == TRD_REC)          /* if at the last field of a record */
    &&  (mode & TA_WEIGHT)) {   /* and to read transaction weights */
      if (!*b) return base->err = E_WGTEXP;
      t->wgt = strtol(b, &e,0); /* check for an empty field */
      if ((t->wgt < 0) || *e) return base->err = E_TAWGT;
      break;                    /* get the transaction weight and */
    }                           /* check for a non-negative value */
    if (!*b) return base->err = E_ITEMEXP;
    item = (ITEM*)nim_byname(base->nimap, b);
    if (!item) {                /* look up the name in name/id map */
      if (base->app == APP_NONE) { /* if new items are to be ignored */
        if (base->isext && trd_istype(trd, trd_last(trd), TA_WGTSEP))
          d = trd_read(trd);    /* consume a possible item weight */
        return (d <= TRD_ERR) ? E_FREAD : (d == TRD_FLD) ? 0 : 1;
      }                         /* do not register the item */
      item = (ITEM*)nim_add(base->nimap, b, sizeof(ITEM));
      if (!item) return E_NOMEM;/* add the new item to the map and */
      item->app = base->app;    /* init. the appearance indicator */
      item->idx = item->xfq = item->frq = 0;
      item->pen = base->pen;    /* clear counters and trans. index */
    }                           /* and init. the insertion penalty */
    if (item->idx >= base->idx){/* if the item is already contained */
      if   (mode & TA_DUPERR)   return base->err = E_DUPITEM;
      if (!(mode & TA_DUPLICS)) continue;
    }                           /* check what to do with duplicates */
    item->idx = base->idx;      /* update the transaction index */
    n = base->size;             /* get the current buffer size */
    if (base->isext) {          /* if extended transaction */
      if (x->size >= n) {       /* if the transaction buffer is full */
        n += (n > BLKSIZE) ? (n >> 1) : BLKSIZE;
        x  = (XTRACT*)realloc(x, sizeof(XTRACT) +n *sizeof(XTITEM));
        if (!x) return base->err = E_NOMEM;
        base->tract = x; base->size = n;
      }                         /* enlarge the transaction buffer */
      xitm = x->items +x->size++; /* get the next transaction item */
      xitm->id = item->id;        /* and store the item identifier */
      if (!trd_istype(trd, trd_last(trd), TA_WGTSEP))
        xitm->wgt = 1;          /* if no weight separator follows, */
      else {                    /* set default weight, otherwise */
        d = trd_read(trd);      /* read the given item weight */
        if (d <= TRD_ERR) return base->err = E_FREAD;
        xitm->wgt = (float)strtod(trd_field(trd), &b);
        if (*b || (xitm->wgt < 0)) return base->err = E_ITEMWGT;
      } }                       /* decode and set the item weight */
    else {                      /* if standard transaction */
      if (t->size >= n) {       /* if the transaction buffer is full */
        n += (n > BLKSIZE) ? (n >> 1) : BLKSIZE;
        t  = (TRACT*)realloc(t, sizeof(TRACT) +n *sizeof(int));
        if (!t) return base->err = E_NOMEM;
        base->tract = t; base->size = n;
      }                         /* enlarge the transaction buffer */
      t->items[t->size++] = item->id;
    }                           /* add the item to the transaction */
  } while (d == TRD_FLD);       /* while not at end of record */
  if (base->isext) {            /* if extended transaction */
    x = (XTRACT*)base->tract;   /* get the transaction buffer and */
    x->items[x->size] = SENT;   /* store a sentinel after the items */
    base->wgt += x->wgt;        /* sum the transaction weight */
    w = x->size *x->wgt;        /* compute extended frequency weight */
    for (i = 0; i < x->size; i++) {
      item = (ITEM*)nim_byid(base->nimap, x->items[i].id);
      item->frq += x->wgt;      /* traverse the items and */
      item->xfq += w;           /* sum the transaction weights */
    } }                         /* and the transaction sizes */
  else {                        /* if standard transaction */
    t = (TRACT*)base->tract;    /* get the transaction buffer and */
    t->items[t->size] = -1;     /* store a sentinel after the items */
    base->wgt += t->wgt;        /* sum the transaction weight and */
    w = t->size *t->wgt;        /* compute extended frequency weight */
    for (i = 0; i < t->size; i++) {
      item = (ITEM*)nim_byid(base->nimap, t->items[i]);
      item->frq += t->wgt;      /* traverse the items and */
      item->xfq += w;           /* sum the transaction weights */
    }                           /* and the transaction sizes */
  }                             /* as importance indicators */
  return base->err = 0;         /* return 'ok' */
}  /* ib_read() */

/*--------------------------------------------------------------------*/

const char* ib_errmsg (ITEMBASE *base, char *buf, size_t size)
{                               /* --- get last (read) error message */
  int        i;                 /* index of error message */
  size_t     k = 0;             /* buffer for header length */
  const char *msg;              /* error message (format) */

  assert(base                   /* check the function arguments */
  &&    (!buf || (size > 0)));  /* if none given, get internal buffer */
  if (!buf) { buf = msgbuf; size = sizeof(msgbuf); }
  i = (base->err < 0) ? -base->err : 0;
  assert(i < (int)(sizeof(emsgs)/sizeof(char*)));
  msg = emsgs[i];               /* get the error message (format) */
  assert(msg);                  /* check for a proper message */
  if (*msg == '#') { msg++;     /* if message needs a header */
    k = snprintf(buf, size, "%s:%d(%d): ", TRD_FPOS(base->trd));
    if (k >= size) k = size-1;  /* print the input file name and */
  }                             /* the record and field number */
  snprintf(buf+k, size-k, msg, trd_field(base->trd));
  return buf;                   /* format the error message */
}  /* ib_errmsg() */

/*--------------------------------------------------------------------*/

int ib_recode (ITEMBASE *base, int min, int max, int dir, int *map)
{                               /* --- recode items w.r.t. frequency */
  int    i, n;                  /* loop variables, item buffer */
  ITEM   *item;                 /* to traverse the items */
  TRACT  *t;                    /* to access the standard transaction */
  int    *s, *d;                /* to traverse the items */
  XTRACT *x;                    /* to access the extended transaction */
  XTITEM *a, *b;                /* to traverse the items */
  CMPFN  *cmp;                  /* comparison function */

  assert(base);                 /* check the function arguments */
  for (i = n = nim_cnt(base->nimap); --n >= 0; ) {
    item = (ITEM*)nim_byid(base->nimap, n);
    if ((item->frq < min) || (item->frq > max))
      item->app = APP_NONE;     /* set all items to 'ignore' */
  }                             /* that have an invalid frequency */
  if      (dir >  1) cmp = _asccmpx;  /* get the appropriate */
  else if (dir >  0) cmp = _asccmp;   /* comparison function */
  else if (dir >= 0) cmp = _nocmp;    /* (ascending/descending) */
  else if (dir > -2) cmp = _descmp;   /* and sort the items */
  else               cmp = _descmpx;  /* w.r.t. their frequency */
  nim_sort(base->nimap, cmp, NULL, map, 1);
  for (i = n = nim_cnt(base->nimap); --n >= 0; )
    if (((ITEM*)nim_byid(base->nimap, n))->app != APP_NONE)
      break;                    /* find last non-ignorable item */
  nim_trunc(base->nimap, ++n);  /* remove all items to ignore */
  if (!map) return n;           /* if no map is provided, abort */
  while (--i >= 0)              /* mark all removed items */
    if (map[i] >= n) map[i] = -1;
  if (base->isext) {            /* if extended transaction */
    x = (XTRACT*)base->tract;   /* traverse the buffered transaction */
    for (a = b = x->items; a->id >= 0; a++) {
      i = map[a->id];           /* recode all items and */
      if (i >= 0) (b++)->id = i;/* remove all items to ignore */
    }                           /* from the buffered transaction */
    x->size = b -x->items;      /* compute the new number of items */
    x->items[x->size] = SENT; } /* store a sentinel after the items */
  else {                        /* if standard transaction */
    t = (TRACT*)base->tract;    /* traverse the buffered transaction */
    for (s = d = t->items; *s >= 0; s++) {
      i = map[*s];              /* recode all items and */
      if (i >= 0) *d++ = i;     /* remove all items to ignore */
    }                           /* from the buffered transaction */
    t->size = d -t->items;      /* compute the new number of items */
    t->items[t->size] = -1;     /* store a sentinel after the items */
  }
  return n;                     /* return number of frequent items */
}  /* ib_recode() */

/*--------------------------------------------------------------------*/

void ib_trunc (ITEMBASE *base, int cnt)
{                               /* --- truncate an item base */
  TRACT  *t;                    /* to access the standard transaction */
  int    *s, *d;                /* to traverse the items */
  XTRACT *x;                    /* to access the extended transaction */
  XTITEM *a, *b;                /* to traverse the items */

  assert(base && (cnt >= 0));   /* check the function arguments */
  nim_trunc(base->nimap, cnt);  /* truncate the item base */
  if (base->isext) {            /* if extended transaction */
    x = (XTRACT*)base->tract;   /* traverse the buffered transaction */
    for (a = b = x->items; a->id >= 0; a++)
      if (a->id < cnt) *b++ = *a;      /* remove all deleted items */
    x->size = b -x->items;      /* compute the new number of items */
    x->items[x->size] = SENT; } /* store a sentinel after the items */
  else {                        /* if standard transaction */
    t = (TRACT*)base->tract;    /* traverse the buffered transaction */
    for (s = d = t->items; *s >= 0; s++)
      if (*s < cnt) *d++ = *s;  /* remove all deleted items */
    t->size = d -t->items;      /* compute the new number of items */
    t->items[t->size] = -1;     /* store a sentinel after the items */
  }                             /* (adapt the buffered transaction) */
}  /* ib_trunc() */

/*----------------------------------------------------------------------
  Transaction Functions
----------------------------------------------------------------------*/

TRACT* t_create (const int *items, int n, int wgt)
{                               /* --- create a transaction */
  TRACT *t;                     /* created transaction */

  assert(items || (n <= 0));    /* check the function arguments */
  t = (TRACT*)malloc(sizeof(TRACT) +n *sizeof(int));
  if (!t) return NULL;          /* allocate a new transaction */
  t->wgt = wgt;                 /* set weight and copy the items */
  memcpy(t->items, items, n *sizeof(int));
  t->items[t->size = n] = -1;   /* store a sentinel after the items */
  return t;                     /* return the created transaction */
}  /* t_create() */

/*--------------------------------------------------------------------*/

TRACT* t_clone (const TRACT *t)
{ return t_create(t->items, t->size, t->wgt); }

/*--------------------------------------------------------------------*/

int t_cmp (const void *p1, const void *p2, void *data)
{                               /* --- compare transactions */
  const int *a, *b;             /* to traverse the items */
  int d;                        /* difference of item identifiers */

  assert(p1 && p2);             /* check the function arguments */
  a = ((const TRACT*)p1)->items;
  b = ((const TRACT*)p2)->items;
  for ( ; 1; a++, b++) {        /* traverse the item arrays */
    d = *a -*b;                 /* compare corresponding items */
    if (d != 0) return d;       /* and if one is greater, abort */
    if (*a < 0) return 0;       /* otherwise check for the sentinel */
  }                             /* and abort if it is reached */
}  /* t_cmp() */

/*--------------------------------------------------------------------*/

int t_cmpx (const TRACT *t, const int *items, int n)
{                               /* --- compare transactions */
  const int *i;                 /* to traverse the items */
  int k, d;                     /* loop variable, difference */

  assert(t && items);           /* check the function arguments */
  k = (n < t->size) ? n : t->size;
  for (i = t->items; --k >= 0;){/* traverse the item array */
    d = *i++ -*items++;         /* compare corresponding items */
    if (d != 0) return d;       /* and abort the comparison */
  }                             /* if one of them is greater */
  return t->size -n;            /* return the size difference */
}  /* t_cmpx() */

/*--------------------------------------------------------------------*/

int t_cmpsz (const void *p1, const void *p2, void *data)
{                               /* --- compare transactions */
  int a, b;                     /* transaction sizes */

  assert(p1 && p2);             /* check the function arguments */
  a = ((const TRACT*)p1)->size; /* return the sign */
  b = ((const TRACT*)p2)->size; /* of the size difference */
  return (a > b) ? 1 : (a < b) ? -1 : t_cmp(p1, p2, data);
}  /* t_cmpsz() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

void t_show (TRACT *t, ITEMBASE *base)
{                               /* --- show a transaction */
  int k;                        /* loop variable */

  for (k = 0; k < t->size; k++){/* traverse the items */
    if (k > 0) fputc(' ', stdout);
    if (base) printf("%s/", ib_name(base, t->items[k]));
    printf("%d", t->items[k]);  /* print item name and identifier */
  }
  printf(" [%d]\n", t->wgt);    /* finally print the weight */
}  /* t_show() */

#endif
/*----------------------------------------------------------------------
  Extended Transaction Item Functions
----------------------------------------------------------------------*/

int xi_cmp (const XTITEM *a, const XTITEM *b)
{                               /* --- compare two transactions */
  int    i, d;                  /* loop variable, id. difference */
  double w;                     /* weight difference */

  assert(a && b);               /* check the function arguments */
  for (i = 0; 1; i++) {         /* lexicographic comparison loop */
    d = a[i].id - b[i].id;      /* compare corresponding items */
    if (d != 0) return d;       /* and if one is greater, abort */
    if (a[i].id < 0) break;     /* check for the sentinel and */
  }                             /* abort if it is reached */
  for (i = 0; 1; i++) {         /* weight comparison loop */
    w = a[i].wgt - b[i].wgt;    /* compare the item weights and */
    if (w != 0) return (w > 0) ? 1 : -1;/* abort if they differ */
    if (a[i].id < 0) return 0;  /* check for the sentinel and */
  }                             /* abort if it is reached */
}  /* xi_cmp() */

/*--------------------------------------------------------------------*/

static void _xirec (XTITEM *xia, int n)
{                               /* --- recursive part of item sorting */
  XTITEM *l, *r, t;             /* exchange positions and buffer*/
  int    x, m;                  /* pivot element, number of elements */

  do {                          /* sections sort loop */
    l = xia; r = l +n -1;       /* start at left and right boundary */
    if (l->id > r->id) {        /* bring the first and last element */
      t = *l; *l = *r; *r = t;} /* into the proper order */
    x = xia[n >> 1].id;         /* get the middle element as pivot */
    if      (x < l->id) x = l->id;  /* try to find a */
    else if (x > r->id) x = r->id;  /* better pivot element */
    while (1) {                 /* split and exchange loop */
      while ((++l)->id < x);    /* skip smaller elements on the left */
      while ((--r)->id > x);    /* skip greater elements on the right */
      if (l >= r) {             /* if less than two elements left, */
        if (l <= r) { l++; r--; } break; }       /* abort the loop */
      t = *l; *l = *r; *r = t;  /* otherwise exchange elements */
    }
    m = xia +n -l;              /* compute the number of elements */
    n = r -xia +1;              /* right and left of the split */
    if (n > m) {                /* if right section is smaller, */
      if (m >= TH_INSERT)       /* but larger than the threshold, */
        _xirec(l, m); }         /* sort it by a recursive call, */
    else {                      /* if the left section is smaller, */
      if (n >= TH_INSERT)       /* but larger than the threshold, */
        _xirec(xia, n);         /* sort it by a recursive call, */
      xia = l; n = m;           /* then switch to the right section */
    }                           /* keeping its size m in variable n */
  } while (n >= TH_INSERT);     /* while greater than threshold */
}  /* _xirec() */

/*--------------------------------------------------------------------*/

void xi_sort (XTITEM *xia, int n)
{                               /* --- sort an transaction item array */
  int    k;                     /* size of first section */
  XTITEM *l, *r;                /* to traverse the array */
  XTITEM t;                     /* exchange buffer */

  assert(xia && (n >= 0));      /* check the function arguments */
  if (n <= 1) return;           /* do not sort less than two elements */
  if (n < TH_INSERT)            /* if fewer elements than threshold */
    k = n;                      /* for insertion sort, note the */
  else {                        /* number of elements, otherwise */
    _xirec(xia, n);             /* call the recursive function */
    k = TH_INSERT -1;           /* and get the number of elements */
  }                             /* in the first array section */
  for (l = r = xia; --k > 0; )  /* find the smallest element */
    if ((++r)->id < l->id) l = r;  /* among the first k elements */
  r = xia;                      /* swap the smallest element */
  t = *l; *l = *r; *r = t;      /* to the front as a sentinel */
  while (--n > 0) {             /* insertion sort loop */
    t = *++r;                   /* note the element to insert */
    for (l = r; (--l)->id > t.id; )   /* shift right elements */
      l[1] = *l;                /* that are greater than the one to */
    l[1] = t;                   /* insert and store the element to */
  }                             /* insert in the place thus found */
}  /* xi_sort() */

/*--------------------------------------------------------------------*/

void xi_reverse (XTITEM *xia, int n)
{                               /* --- reverse an item array */
  XTITEM t;                     /* exchange buffer */

  assert(xia && (n >= 0));      /* check the function arguments */
  while (--n > 0) {             /* reverse the order of the items */
    t = xia[n]; xia[n--] = xia[0]; *xia++ = t; }
}  /* xi_reverse() */

/*--------------------------------------------------------------------*/

int xi_unique (XTITEM *xia, int n)
{                               /* --- make item array unique */
  XTITEM *s, *d;                /* to traverse the item array */

  assert(xia && (n >= 0));      /* check the function arguments */
  if (n <= 1) return n;         /* check for 0 or 1 element */
  for (d = s = xia; --n > 0; ) {
    if      ((++s)->id != d->id) *++d = *s;
    else if (   s->wgt > d->wgt) d->wgt = s->wgt;
  }                             /* collect the unique elements */
  *++d = SENT;                  /* store a sentinel at the end */
  return d -xia;                /* return the new number of elements */
}  /* xi_unique() */

/*----------------------------------------------------------------------
  Extended Transaction Functions
----------------------------------------------------------------------*/

XTRACT* xt_create (int size, int wgt)
{                               /* --- create a transaction */
  XTRACT *t;                    /* created transaction */

  assert(size >= 0);            /* check the function arguments */
  t = (XTRACT*)malloc(sizeof(XTRACT) +size *sizeof(XTITEM));
  if (!t) return NULL;          /* allocate a new transaction */
  t->wgt  = wgt;                /* store the transaction weight */
  t->size = 0;                  /* and init. the transaction size */
  t->items[size] = SENT;        /* store a sentinel after the items */
  return t;                     /* return the created transaction */
}  /* xt_create() */

/*--------------------------------------------------------------------*/

XTRACT* xt_clone (const XTRACT *t)
{                               /* --- clone a transaction */
  XTRACT *c;                    /* created transaction clone */
  XTITEM *d; const XTITEM *s;   /* to traverse the items */

  c = (XTRACT*)malloc(sizeof(XTRACT) +t->size *sizeof(XTITEM));
  if (!c) return NULL;          /* allocate a new transaction */
  c->size = t->size;            /* copy the transaction size */
  c->wgt  = t->wgt;             /* and  the transaction weight */
  for (s = t->items, d = c->items; s->id >= 0; )
    *d++ = *s++;                /* copy the transaction items */
  *d = *s;                      /* copy the sentinel */
  return c;                     /* return the created copy */
}  /* xt_clone() */

/*--------------------------------------------------------------------*/

void xt_add (XTRACT *t, int item, float wgt)
{                               /* --- add an item to a transaction */
  XTITEM *d;                    /* destination for the item */

  assert(t && (item >= 0));     /* check the function arguments */
  d = t->items +t->size++;      /* get the next extended item and */
  d->id = item; d->wgt = wgt;   /* store item ientifier and weight */
}  /* xt_add() */

/*--------------------------------------------------------------------*/

void xt_sort (XTRACT *t)
{ xi_sort(t->items, t->size); }

/*--------------------------------------------------------------------*/

void xt_reverse (XTRACT *t)
{ xi_reverse(t->items, t->size); }

/*--------------------------------------------------------------------*/

int xt_unique (XTRACT *t)
{ return t->size = xi_unique(t->items, t->size); }

/*--------------------------------------------------------------------*/

int xt_cmp (const void *p1, const void *p2, void *data)
{                               /* --- compare transactions */
  return xi_cmp(((const XTRACT*)p1)->items,
                ((const XTRACT*)p2)->items);
}  /* xt_cmp() */

/*--------------------------------------------------------------------*/

int xt_cmpsz (const void *p1, const void *p2, void *data)
{                               /* --- compare transactions */
  int a, b;                     /* transaction sizes */

  assert(p1 && p2);             /* check the function arguments */
  a = ((const XTRACT*)p1)->size;/* return the sign */
  b = ((const XTRACT*)p2)->size;/* of the size difference */
  return (a > b) ? 1 : (a < b) ? -1 : xt_cmp(p1, p2, data);
}  /* xt_cmpsz() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

void xt_show (XTRACT *t, ITEMBASE *base)
{                               /* --- show an extended transaction */
  int k;                        /* to traverse the items */

  for (k = 0; k < t->size; k++){/* traverse the items */
    if (k > 0) fputc(' ', stdout);
    if (base) printf("%s/", ib_name(base, t->items[k].id));
    printf("%d:%g", t->items[k].id, t->items[k].wgt);
  }                             /* print item name, id, weight */
  printf(" [%d]\n", t->wgt);    /* finally print the trans. weight */
}  /* xt_show() */

#endif
/*----------------------------------------------------------------------
  Transaction Bag/Multiset Functions
----------------------------------------------------------------------*/

TABAG* tbg_create (ITEMBASE *base)
{                               /* --- create a transaction bag */
  TABAG *bag;                   /* created transaction bag */

  assert(base);                 /* check the function argument */
  bag = (TABAG*)malloc(sizeof(TABAG));
  if (!bag) return NULL;        /* create a transaction bag/multiset */
  bag->base   = base;           /* store the underlying item base */
  bag->extent = bag->wgt = bag->max = 0;
  bag->isext  = base->isext;    /* initialize the other fields */
  bag->cnt    = bag->size = 0;
  bag->tracts = NULL;           /* there are no transactions yet */
  bag->ifrqs  = bag->icnts = NULL;
  return bag;                   /* return the created t.a. bag */
}  /* tbg_create() */

/*--------------------------------------------------------------------*/

void tbg_delete (TABAG *bag, int delis)
{                               /* --- delete a transaction bag */
  assert(bag);                  /* check the function argument */
  if (bag->tracts) {            /* if there are loaded transactions */
    while (--bag->cnt >= 0)     /* traverse the transaction array */
      free(bag->tracts[bag->cnt]);
    free(bag->tracts);          /* delete all transactions */
  }                             /* and the transaction array */
  if (bag->icnts) free (bag->icnts);
  if (delis) ib_delete(bag->base);
  free(bag);                    /* delete the item base and */
}  /* tbg_delete() */           /* the transaction bag body */

/*--------------------------------------------------------------------*/

static void tbg_count (TABAG *bag)
{                               /* --- count item occurrences */
  int   i;                      /* loop variable, number of items */
  TRACT *t;                     /* to traverse the transactions */
  int   *s;                     /* to traverse the transaction items */

  i = ib_cnt(bag->base);        /* get the number of items */
  bag->icnts = (int*)calloc(i+i, sizeof(int));
  if (!bag->icnts) return;      /* allocate the counter arrays */
  bag->ifrqs = bag->icnts +i;   /* and organize them */
  for (i = bag->cnt; --i >= 0; ) {
    t = bag->tracts[i];         /* traverse the transactions */
    for (s = t->items; *s >= 0; s++) {
      bag->icnts[*s] += 1;      /* traverse the transaction items */
      bag->ifrqs[*s] += t->wgt;
    }                           /* count the occurrences and */
  }                             /* sum the transaction weights */
}  /* tbg_count() */

/*--------------------------------------------------------------------*/

const int* tbg_icnts (TABAG *bag)
{                               /* --- number of trans. per item */
  if (!bag->icnts) tbg_count(bag);
  return bag->icnts;            /* count the item occurrences */
}  /* tbg_icnts() */

/*--------------------------------------------------------------------*/

const int* tbg_ifrqs (TABAG *bag)
{                               /* --- item frequencies (weight) */
  if (!bag->ifrqs) tbg_count(bag);
  return bag->ifrqs;            /* determine the item frequencies */
}  /* tbg_ifrqs() */

/*--------------------------------------------------------------------*/

int tbg_add (TABAG *bag, TRACT *t)
{                               /* --- add a standard transaction */
  void **p;                     /* new transaction array */
  int  n;                       /* new transaction array size */

  assert(bag && !bag->isext);   /* check the function arguments */
  n = bag->size;                /* get the transaction array size */
  if (bag->cnt >= n) {          /* if the transaction array is full */
    n += (n > BLKSIZE) ? (n >> 1) : BLKSIZE;
    p  = (void**)realloc(bag->tracts, n *sizeof(TRACT*));
    if (!p) return -1;          /* enlarge the transaction array */
    bag->tracts = p; bag->size = n;
  }                             /* set the new array and its size */
  if (!t && !(t = t_clone(ib_tract(bag->base))))
    return -1;                  /* get trans. from item base if nec. */
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  bag->tracts[bag->cnt++] = t;  /* store the transaction and */
  bag->wgt += t->wgt;           /* sum the transaction weight */
  if (t->size > bag->max) bag->max = t->size;
  bag->extent += t->size;       /* update maximal transaction size */
  return 0;                     /* and return 'ok' */
}  /* tbg_add() */

/*--------------------------------------------------------------------*/

int tbg_addx (TABAG *bag, XTRACT *t)
{                               /* --- add an extended transaction */
  void **p;                     /* new transaction array */
  int  n;                       /* new transaction array size */

  assert(bag && bag->isext);    /* check the function arguments */
  n = bag->size;                /* get the transaction array size */
  if (bag->cnt >= n) {          /* if the transaction array is full */
    n += (n > BLKSIZE) ? (n >> 1) : BLKSIZE;
    p  = (void**)realloc(bag->tracts, n *sizeof(XTRACT*));
    if (!p) return -1;          /* enlarge the transaction array */
    bag->tracts = p; bag->size = n;
  }                             /* set the new array and its size */
  if (!t && !(t = xt_clone(ib_xtract(bag->base))))
    return -1;                  /* get trans. from item base if nec. */
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  bag->tracts[bag->cnt++] = t;  /* store the transaction and */
  bag->wgt += t->wgt;           /* sum the transaction weight */
  if (t->size > bag->max) bag->max = t->size;
  bag->extent += t->size;       /* update maximal transaction size */
  return 0;                     /* and return 'ok' */
}  /* tbg_addx() */

/*--------------------------------------------------------------------*/

int tbg_addib (TABAG *bag)
{                               /* --- add transaction from item base */
  assert(bag);                  /* check the function argument */
  return (bag->isext) ? tbg_addx(bag, NULL) : tbg_add(bag, NULL);
}  /* tbg_addib() */

/*--------------------------------------------------------------------*/

int tbg_read (TABAG *bag, TABREAD *tread, int mode)
{                               /* --- read transactions from a file */
  int r;                        /* result of ib_read()/tbg_add() */

  assert(bag && tread);         /* check the function arguments */
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  while (1) {                   /* transaction read loop */
    r = ib_read(bag->base, tread, mode);
    if (r < 0) return r;        /* read the next transaction and */
    if (r > 0) return 0;        /* check for error and end of file */
    r = (bag->isext) ? tbg_addx(bag, NULL) : tbg_add(bag, NULL);
    if (r) return bag->base->err = E_NOMEM;
  }                             /* add transaction to bag/multiset */
}  /* tbg_read() */

/*--------------------------------------------------------------------*/

void tbg_recode (TABAG *bag, int *map)
{                               /* --- recode items in transactions */
  int    i, n;                  /* item buffer, loop variable */
  TRACT  *t;                    /* to traverse the transactions */
  int    *s, *d;                /* to traverse the items */
  XTRACT *x;                    /* to traverse the transactions */
  XTITEM *a, *b;                /* to traverse the items */

  assert(bag && map);           /* check the function arguments */
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  bag->max = bag->extent = 0;   /* clear maximal transaction size */
  if (bag->isext) {             /* if extended transactions */
    for (n = bag->cnt; --n >= 0; ) {
      x = (XTRACT*)bag->tracts[n];  /* traverse the transactions */
      for (a = b = x->items; a->id >= 0; a++) {
        i = map[a->id];         /* traverse and recode the items */
        if (i >= 0) (b++)->id = i;  /* remove all items that are */
      }                         /* not mapped (mapped to id < 0) */
      x->size = b -x->items;    /* compute the new number of items */
      x->items[x->size] = SENT; /* and store a sentinel at the end */
      if (x->size > bag->max)   /* update the maximal trans. size */
        bag->max = x->size;     /* (may differ from the old size */
      bag->extent += x->size;   /* as items may have been removed) */
    } }                         /* and sum the item instances */
  else {                        /* if standard transactions */
    for (n = bag->cnt; --n >= 0; ) {
      t = (TRACT*)bag->tracts[n];   /* traverse the transactions */
      for (s = d = t->items; *s >= 0; s++) {
        i = map[*s];            /* traverse and recode the items */
        if (i >= 0) *d++ = i;   /* remove all items that are */
      }                         /* not mapped (mapped to id < 0) */
      t->size = d -t->items;    /* compute the new number of items */
      t->items[t->size] = -1;   /* and store a sentinel at the end */
      if (t->size > bag->max)   /* update the maximal trans. size */
        bag->max = t->size;     /* (may differ from the old size */
      bag->extent += t->size;   /* as items may have been removed) */
    }                           /* and sum the item instances */
  }
}  /* tbg_recode() */

/*--------------------------------------------------------------------*/

void tbg_filter (TABAG *bag, int min, const int *marks)
{                               /* --- filter (items in) transactions */
  int    n;                     /* loop variable */
  TRACT  *t;                    /* to traverse the transactions */
  int    *s, *d;                /* to traverse the items */
  XTRACT *x;                    /* to traverse the transactions */
  XTITEM *a, *b;                /* to traverse the items */

  assert(bag);                  /* check the function arguments */
  if (!marks && (min <= 1)) return;
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  bag->max = bag->extent = 0;   /* clear maximal transaction size */
  if (bag->isext) {             /* if extended transactions */
    for (n = bag->cnt; --n >= 0; ) {
      x = (XTRACT*)bag->tracts[n];  /* traverse the transactions */
      if (marks) {              /* if item markers are given */
        for (a = b = x->items; a->id >= 0; a++)
          if (marks[a->id]) *b++ = *a;
        x->size = b -x->items;  /* remove unmarked items and */
      }                         /* store the new number of items */
      if (x->size < min)        /* if the transaction is too short, */
        x->size = 0;            /* delete all items (clear size) */
      x->items[x->size] = SENT; /* store a sentinel after the items */
      if (x->size > bag->max)   /* update the maximal trans. size */
        bag->max = x->size;     /* (may differ from the old size */
      bag->extent += x->size;   /* as items may have been removed) */
    } }                         /* and sum the item instances */
  else {                        /* if standard transactions */
    for (n = bag->cnt; --n >= 0; ) {
      t = (TRACT*)bag->tracts[n];   /* traverse the transactions */
      if (marks) {              /* if item markers are given */
        for (s = d = t->items; *s >= 0; s++)
          if (marks[*s]) *d++ = *s;
        t->size = d -t->items;  /* remove unmarked items and */
      }                         /* store the new number of items */
      if (t->size < min)        /* if the transaction is too short, */
        t->size = 0;            /* delete all items (clear size) */
      t->items[t->size] = -1;   /* store a sentinel after the items */
      if (t->size > bag->max)   /* update the maximal trans. size */
        bag->max = t->size;     /* (may differ from the old size */
      bag->extent += t->size;   /* as items may have been removed) */
    }                           /* and sum the item instances */
  }
}  /* tbg_filter() */

/*--------------------------------------------------------------------*/

void tbg_trim (TABAG *bag, int min, const int *marks)
{                               /* --- trim transactions (sequences) */
  int    k, n;                  /* loop variables */
  TRACT  *t;                    /* to traverse the transactions */
  int    *s, *d;                /* to traverse the items */
  XTRACT *x;                    /* to traverse the transactions */
  XTITEM *a, *b;                /* to traverse the items */

  assert(bag);                  /* check the function arguments */
  bag->max = bag->extent = 0;   /* clear maximal transaction size */
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  if (bag->isext) {             /* if extended transactions */
    for (n = bag->cnt; --n >= 0; ) {
      x = (XTRACT*)bag->tracts[n];  /* traverse the transactions */
      if (marks) {              /* if item markers are given */
        for (a = x->items, k = x->size; --k >= 0; )
          if (marks[a[k].id]) break;
        x->size     = ++k;      /* trim infrequent items at the end */
        x->items[k] = SENT;     /* and set a (new) sentinel */
        for (a = b = x->items; a->id >= 0; a++)
          if (marks[a->id]) break;  /* trim items at the front */
        if (a > b) {            /* if front items were trimmed */
          while (a->id >= 0) *b++ = *a++;
          x->size = b -x->items;/* move remaining items to the front */
        }                       /* and store the new number of items */  
      }
      if (x->size < min)        /* if the transaction is too short, */
        x->size = 0;            /* delete all items (clear size) */
      x->items[x->size] = SENT; /* store a sentinel after the items */
      if (x->size > bag->max)   /* update the maximal trans. size */
        bag->max = x->size;     /* (may differ from the old size */
      bag->extent += x->size;   /* as items may have been removed) */
    } }                         /* and sum the item instances */
  else {                        /* if standard transactions */
    for (n = bag->cnt; --n >= 0; ) {
      t = (TRACT*)bag->tracts[n];  /* traverse the transactions */
      if (marks) {              /* if item markers are given */
        for (s = t->items, k = t->size; --k >= 0; )
          if (marks[s[k]]) break;
        t->size     = ++k;      /* trim infrequent items at the end */
        t->items[k] = -1;       /* and set a (new) sentinel */
        for (s = d = t->items; *s >= 0; s++)
          if (marks[*s]) break; /* trim infrequent items at the front */
        if (s > d) {            /* if front items were trimmed */
          while (*s >= 0) *d++ = *s++;
          t->size = d -t->items;/* move remaining items to the front */
        }                       /* and store the new number of items */
      }
      if (t->size < min)        /* if the transaction is too short, */
        t->size = 0;            /* delete all items (clear size) */
      t->items[t->size] = -1;   /* store a sentinel after the items */
      if (t->size > bag->max)   /* update the maximal trans. size */
        bag->max = t->size;     /* (may differ from the old size */
      bag->extent += t->size;   /* as items may have been removed) */
    }                           /* and sum the item instances */
  }
}  /* tbg_trim() */

/*--------------------------------------------------------------------*/

void tbg_itsort (TABAG *bag, int dir, int heap)
{                               /* --- sort items in transactions */
  int    i;                     /* loop variable */
  TRACT  *t;                    /* to traverse the transactions */
  XTRACT *x;                    /* to traverse the transactions */
  void   (*sortfn)(int*, int);  /* transaction sort function */

  assert(bag);                  /* check the function arguments */
  if (bag->isext) {             /* if extended transactions */
    for (i = bag->cnt; --i >= 0; ) {
      x = (XTRACT*)bag->tracts[i]; /* traverse the transactions */
      xi_sort(x->items, x->size);
      if (dir < 0) xi_reverse(x->items, x->size);
    } }                         /* sort the items in each transaction */
  else {                        /* if standard transactions */
    sortfn = (heap) ? int_heapsort : int_qsort;
    for (i = bag->cnt; --i >= 0; ) {
      t = (TRACT*)bag->tracts[i];  /* traverse the transactions */
      sortfn(t->items, t->size);
      if (dir < 0) int_reverse(t->items, t->size);
    }                         /* sort the items in each transaction */
  }                           /* according to the given direction */
}  /* tbg_itsort() */

/*--------------------------------------------------------------------*/

void tbg_mirror (TABAG *bag)
{                             /* --- mirror all transactions */
  int    i;                   /* loop variable */
  TRACT  *t;                  /* to traverse the transactions */
  XTRACT *x;                  /* to traverse the transactions */

  assert(bag);                /* check the function argument */
  if (bag->isext) {           /* if extended transactions */
    for (i = 0; i < bag->cnt; i++) {
      x = (XTRACT*)bag->tracts[i]; xt_reverse(x); } }
  else {                      /* if standard transactions */
    for (i = 0; i < bag->cnt; i++) {
      t = (TRACT*)bag->tracts[i];   t_reverse(t); } }
}  /* tbg_mirror() */

/*--------------------------------------------------------------------*/

void tbg_sort (TABAG *bag, int dir, int heap)
{                               /* --- sort a transaction bag */
  assert(bag);                  /* check the function arguments */
  if (bag->isext) {             /* if extended transactions */
    if (heap) ptr_heapsort(bag->tracts, bag->cnt, xt_cmp, NULL);
    else      ptr_qsort   (bag->tracts, bag->cnt, xt_cmp, NULL); }
  else {                        /* if standard transactions */
    if (heap) ptr_heapsort(bag->tracts, bag->cnt,  t_cmp, NULL);
    else      ptr_qsort   (bag->tracts, bag->cnt,  t_cmp, NULL);
  }                             /* sort the transactions */
  if (dir < 0) ptr_reverse(bag->tracts, bag->cnt);
}  /* tbg_sort() */

/*--------------------------------------------------------------------*/

void tbg_sortsz (TABAG *bag, int dir, int heap)
{                               /* --- sort a transaction bag */
  assert(bag);                  /* check the function arguments */
  if (bag->isext) {             /* if extended transactions */
    if (heap) ptr_heapsort(bag->tracts, bag->cnt, xt_cmpsz, NULL);
    else      ptr_qsort   (bag->tracts, bag->cnt, xt_cmpsz, NULL); }
  else {                        /* if standard transactions */
    if (heap) ptr_heapsort(bag->tracts, bag->cnt,  t_cmpsz, NULL);
    else      ptr_qsort   (bag->tracts, bag->cnt,  t_cmpsz, NULL);
  }                             /* sort the transactions */
  if (dir < 0) ptr_reverse(bag->tracts, bag->cnt);
}  /* tbg_sortsz() */

/*--------------------------------------------------------------------*/

int tbg_reduce (TABAG *bag)
{                               /* --- reduce a transaction bag */
  /* This function presupposes that the transaction bag has been */
  /* sorted with the above function tbg_sort() beforehand. */
  int   i, c;                   /* loop variable, comparison result */
  TRACT **s, **d;               /* to traverse the transactions */

  assert(bag);                  /* check the function argument */
  if (bag->cnt <= 1) return 1;  /* deal only with two or more trans. */
  if (bag->icnts) {             /* delete the item-specific counters */
    free(bag->icnts); bag->ifrqs = bag->icnts = NULL; }
  bag->extent = 0;              /* reinit. number of item occurrences */
  s = d = (TRACT**)bag->tracts; /* traverse the sorted transactions */
  for (i = bag->cnt; --i > 0; ) {
    c = (*++s)->size -(*d)->size;     /* compare the size first and */
    if (c == 0)                 /* compare items only for same size */
      c = (bag->isext) ? xt_cmp(*s, *d, NULL) : t_cmp(*s, *d, NULL);
    if (c == 0) {               /* if the transactions are equal */
      (*d)->wgt += (*s)->wgt;   /* combine the transactions */
      free(*s); }               /* by summing their weights */
    else {                      /* if transactions are not equal, */
      if ((*d)->wgt > 0) bag->extent += (*d++)->size;
      else free(*d);            /* check weight of old transaction */
      *d = *s;                  /* copy the new transaction */
    }                           /* to close a possible gap */
  }                             /* (collect unique transactions) */
  if ((*d)->wgt > 0) bag->extent += (*d++)->size;
  else free(*d);                /* check weight of last transaction */
  return bag->cnt = d -(TRACT**)bag->tracts;
}  /* tbg_reduce() */           /* return new number of transactions */

/*--------------------------------------------------------------------*/

int tbg_occur (TABAG *bag, const int *items, int n)
{                               /* --- count transaction occurrences */
  int l, r, m, k;               /* index and loop variables */

  assert(bag && !bag->isext && items);  /* check function arguments */
  k = bag->cnt;                 /* get the number of transactions */
  for (r = m = 0; r < k; ) {    /* find right boundary */
    m = (r+k) >> 1;             /* by a binary search */
    if (t_cmpx((TRACT*)bag->tracts[m], items, n) > 0) k = m;
    else                                      r = m+1;
  }
  for (l = m = 0; l < k; ) {    /* find left boundary */
    m = (l+k) >> 1;             /* by a binary search */
    if (t_cmpx((TRACT*)bag->tracts[m], items, n) < 0) l = m+1;
    else                                      k = m;
  }
  for (k = 0; l < r; l++)       /* traverse the found section */
    k += tbg_tract(bag,l)->wgt; /* sum the transaction weights */
  return k;                     /* return the number of occurrences */
}  /* tbg_occur() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

void tbg_show (TABAG *bag)
{                               /* --- show a transaction bag */
  int i;                        /* loop variable */

  assert(bag);                  /* check the function argument */
  for (i = 0; i < bag->cnt; i++) { /* traverse the transactions */
    if (bag->isext) xt_show((XTRACT*)bag->tracts[i], bag->base);
    else             t_show( (TRACT*)bag->tracts[i], bag->base);
  }                             /* print the transactions */
  printf("%d/%d transaction(s)\n", bag->cnt, bag->wgt);
}  /* tbg_show() */

#endif
/*----------------------------------------------------------------------
  Transaction Tree Functions
----------------------------------------------------------------------*/

void _delete (TTNODE *root)
{                               /* --- delete a transaction (sub)tree */
  int    i;                     /* loop variable */
  TTNODE **cnds;                /* array of child nodes */

  assert(root);                 /* check the function argument */
  cnds = (TTNODE**)(root->items +CHOFF(root->size));
  for (i = root->size; --i >= 0; )
    _delete(cnds[i]);           /* recursively delete the subtrees */
  free(root);                   /* and the tree node itself */
}  /* _delete() */

/*--------------------------------------------------------------------*/

TTNODE* _create (TRACT **tracts, int cnt, int index)
{                               /* --- recursive part of tt_create() */
  int    i, k, t, w;            /* loop variables, buffers */
  int    item, n;               /* item and item counter */
  TTNODE *root;                 /* root of created transaction tree */
  TTNODE **cnds;                /* array of child nodes */
  int    *s, *d;                /* to traverse the items */

  assert(tracts                 /* check the function arguments */
     && (cnt >= 0) && (index >= 0));
  for (w = 0, k = cnt; --k >= 0; )
    w += tracts[k]->wgt;        /* determine the total trans. weight */
  if (w <= 0) cnt = 0;          /* check for weightless transactions */

  if (cnt <= 1) {               /* if only one transaction left */
    n    = (cnt > 0) ? (*tracts)->size -index : 0;
    root = (TTNODE*)malloc(sizeof(TTNODE) +n *sizeof(int));
    if (!root) return NULL;     /* create a transaction tree node */
    root->wgt  =  w;            /* and initialize the fields */
    root->max  =  n;
    root->size = -n;
    d = root->items; d[n] = -1; /* place a sentinel at the end */
    s = (*tracts)->items +index;
    while (--n >= 0) d[n] = s[n];
    return root;                /* copy the remaining items and */
  }                             /* return the created leaf node */

  for (k = cnt; (--k >= 0) && ((*tracts)->size <= index); )
    tracts++;                   /* skip t.a. that are too short */
  for (n = 0, item = -1, i = ++k; --i >= 0; ) {   
    t = tracts[i]->items[index];/* traverse the transactions */
    if (t != item) { item = t; n++; }
  }                             /* count the different items */
  i    = CHOFF(n);              /* get offset to the child pointers */
  root = (TTNODE*)malloc(sizeof(TTNODE) +(i-1) *sizeof(int)
                                        + n    *sizeof(TTNODE*));
  if (!root) return NULL;       /* create a transaction tree node */
  root->wgt  = w;               /* and initialize its fields */
  root->max  = 0;
  root->size = n;               /* if transactions are captured, */
  if (n <= 0) return root;      /* return the created tree */

  cnds = (TTNODE**)(root->items +i);
  for (--k; --n >= 0; k = i) {  /* traverse the different items */
    root->items[n] = item = tracts[k]->items[index];
    for (i = k; --i >= 0; )     /* find trans. with the current item */
      if (tracts[i]->items[index] != item) break;
    cnds[n] = _create(tracts +i+1, k-i, index+1);
    if (!cnds[n]) break;        /* recursively create a subtree */
    t = cnds[n]->max +1;        /* adapt the maximal remaining size */
    if (t > root->max) root->max = t;
  }                             /* (create all subtrees) */
  if (n < 0) return root;       /* if successful, return created tree */

  for (i = root->size; --i > n; )
    _delete(cnds[i]);           /* on error delete created subtrees */
  free(root);                   /* and the transaction tree node */
  return NULL;                  /* return 'failure' */
}  /* _create() */

/*--------------------------------------------------------------------*/

TATREE* tt_create (TABAG *bag)
{                               /* --- create a transactions tree */
  TATREE *tree;                 /* created transaction tree */

  assert(bag);                  /* check the function argument */
  tree = (TATREE*)malloc(sizeof(TATREE));
  if (!tree) return NULL;       /* create the transaction tree body */
  tree->base = bag->base;       /* note the underlying item set */
  tree->root = _create((TRACT**)bag->tracts, bag->cnt, 0);
  if (!tree->root) { free(tree); return NULL; }
  return tree;                  /* recursively build the tree */
}  /* tt_create() */

/*--------------------------------------------------------------------*/

void tt_delete (TATREE *tree, int delis)
{                               /* --- delete a transaction tree */
  assert(tree);                 /* check the function argument */
  _delete(tree->root);          /* delete the nodes of the tree */
  if (tree->base && delis) ib_delete(tree->base);
  free(tree);                   /* delete the item base and */
}  /* tt_delete() */            /* the transaction tree body */

/*--------------------------------------------------------------------*/

static int _nodecnt (const TTNODE *root)
{                               /* --- count the nodes */
  int    i, n;                  /* loop variable, number of nodes */
  TTNODE **cnds;                /* array of child nodes */

  assert(root);                 /* check the function argument */
  if (root->size <= 0)          /* if this is a leaf node, */
    return 1;                   /* there is only one node */
  cnds = (TTNODE**)(root->items +CHOFF(root->size));
  for (i = n = 0; i < root->size; i++)
    n += _nodecnt(cnds[i]);     /* recursively count the nodes */
  return n+1;                   /* return number of nodes in tree */
}  /* _nodecnt() */

/*--------------------------------------------------------------------*/

int tt_nodecnt (const TATREE *tree)
{ return _nodecnt(tree->root); }

/*--------------------------------------------------------------------*/

static int _extcnt (const TTNODE *root)
{                               /* --- extended node counting */
  int    i, n;                  /* loop variable, number of nodes */
  TTNODE **cnds;                /* array of child nodes */

  assert(root);                 /* check the function argument */
  if (root->size <= 0)          /* if this is a leaf node, */
    return -root->size;         /* return the number of items */
  cnds = (TTNODE**)(root->items +CHOFF(root->size));
  for (i = n = 0; i < root->size; i++)
    n += _extcnt(cnds[i]);      /* recursively count the nodes */
  return n+1;                   /* return number of nodes in tree */
}  /* _extcnt() */

/*--------------------------------------------------------------------*/

int tt_extcnt (const TATREE *tree)
{ return _extcnt(tree->root); }

/*--------------------------------------------------------------------*/
#ifdef ALIGN8

TTNODE* ttn_child (const TTNODE *node, int index)
{                               /* --- go to a child node */
  TTNODE **cnds;                /* array of child nodes */

  assert(node                   /* check the function arguments */
     && (index >= 0) && (index < node->size));
  cnds = (TTNODE**)(node->items +CHOFF(node->size));
  return cnds[index];           /* return the child node/subtree */
}  /* ttn_child */

#endif
/*--------------------------------------------------------------------*/
#ifndef NDEBUG

static void _show (TTNODE *node, ITEMBASE *base, int ind)
{                               /* --- rekursive part of tt_show() */
  int    i, k;                  /* loop variables */
  TTNODE **cnds;                /* array of child nodes */

  assert(node && (ind >= 0));   /* check the function arguments */
  if (node->size <= 0) {        /* if this is a leaf node */
    for (i = 0; i < node->max; i++)
      printf("%s ", ib_name(base, node->items[i]));
    printf("[%d]\n", node->wgt);
    return;                     /* print the items in the */
  }                             /* (rest of) the transaction */
  cnds = (TTNODE**)(node->items +node->size);
  for (i = 0; i < node->size; i++) {
    if (i > 0) for (k = ind; --k >= 0; ) printf("  ");
    printf("%s ", ib_name(base, node->items[i]));
    _show(cnds[i], base, ind+1);/* traverse the items, print them, */
  }                             /* and show the children recursively */
}  /* _show() */

/*--------------------------------------------------------------------*/

void tt_show (TATREE *tree)
{                               /* --- show a transaction tree */
  assert(tree);                 /* check the function argument */
  _show(tree->root, tree->base, 0);
}  /* tt_show() */              /* call the recursive function */

#endif
