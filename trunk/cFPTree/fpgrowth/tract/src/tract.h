/*----------------------------------------------------------------------
  File    : tract.h
  Contents: item and transaction management
  Author  : Christian Borgelt
  History : 2001.11.18 file created from file apriori.c
            2001.12.28 first version completed
            2001.01.02 function t_sort mapped to function int_qsort
            2002.02.19 transaction tree functions added (tt_...)
            2003.07.17 function tbg_filter() added (remove unused items)
            2003.08.21 parameter 'heap' added to function tbg_sort()
            2003.09.12 function tbg_wgt() added (total trans. weight)
            2003.09.20 empty transactions in input made possible
            2004.12.11 access functions for extended frequency added
            2004.12.15 function ib_trunc added (remove irrelevant items)
            2006.11.26 item set formatter and evaluator added
            2007.02.13 adapted to modified module tabread
            2008.08.12 considerable redesign, transaction weight added
            2008.08.14 function tbg_filter() extended (minimal size)
            2008.10.13 functions t_reverse and tbg_reverse() added
            2008.11.19 transaction tree and tree node separated
            2009.05.28 bug in function tbg_filter() fixed (minimal size)
            2009.08.27 fixed prototypes of trans. tree node functions
            2010.03.16 handling of extended transactions added
            2010.06.23 function tbg_extent() added (total item insts.)
            2010.07.02 transaction size comparison functions added
            2010.08.05 function tbg_itemcnt() added for convenience
            2010.08.10 function tbg_trim() added (for sequences)
            2010.08.11 parameter of ib_read() changed to general mode
            2010.08.13 function tbg_addib() added (add from item base)
            2010.08.19 function ib_readsel() added (item selectors)
            2010.08.22 adapted to modified module tabread
            2010.09.13 functions tbg_reverse() and tbg_mirror() added
            2010.10.25 parameter max added to function ib_recode()
            2010.12.15 functions tbg_read() added (read database)
            2010.12.20 functions tbg_icnts() and tbg_ifrqs() added
----------------------------------------------------------------------*/
#ifndef __TRACT__
#define __TRACT__
#ifndef NIMAPFN
#define NIMAPFN
#endif
#include "arrays.h"
#include "symtab.h"
#include "tabread.h"

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
/* --- item appearance flags --- */
#define APP_NONE    0x00        /* item should be ignored */
#define APP_BODY    0x01        /* item may appear in rule body */
#define APP_HEAD    0x02        /* item may appear in rule head */
#define APP_BOTH    (APP_HEAD|APP_BODY)

#define TA_WGTSEP   TRD_OTHER   /* item weight separator */
#define TA_MAXLEN   TRD_MAXLEN  /* maximum length of an item name */

/* --- transaction read modes --- */
#define TA_WEIGHT   0x01        /* integer weight in last field */
#define TA_DUPLICS  0x02        /* allow duplicates of items */
#define TA_DUPERR   0x04        /* consider duplicates as errors */

/* --- error codes --- */
#define E_NONE         0        /* no error */
#define E_NOMEM      (-1)       /* not enough memory */
#define E_FOPEN      (-2)       /* cannot open file */
#define E_FREAD      (-3)       /* read error on file */
#define E_FWRITE     (-4)       /* write error on file */

#define E_NOITEMS   (-15)       /* no frequent items found */
#define E_ITEMEXP   (-16)       /* item expected */
#define E_ITEMWGT   (-17)       /* invalid item weight */
#define E_DUPITEM   (-18)       /* duplicate item */
#define E_WGTEXP    (-19)       /* transaction weight expected */
#define E_TAWGT     (-20)       /* invalid transaction weight */
#define E_FLDCNT    (-21)       /* too many fields */
#define E_APPEXP    (-22)       /* appearance indicator expected */
#define E_UNKAPP    (-23)       /* unknown appearance indicator */
#define E_PENEXP    (-24)       /* insertion penalty expected */
#define E_PENALTY   (-25)       /* invalid insertion penalty */

/*----------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------*/
typedef struct {                /* --- an item --- */
  int      id;                  /* item identifier */
  int      app;                 /* appearance indicator */
  int      frq;                 /* standard frequency (trans. weight) */
  int      xfq;                 /* extended frequency (trans. sizes) */
  int      idx;                 /* index of last transaction */
  int      pad;                 /* padding to even number of ints */
  double   pen;                 /* insertion penalty */
} ITEM;                         /* (item) */

typedef struct {                /* --- a transaction --- */
  int      wgt;                 /* weight (number of occurrences) */
  int      size;                /* size   (number of items) */
  int      items[1];            /* items in the transaction */
} TRACT;                        /* (transaction) */

typedef struct {                /* --- an item in a transaction --- */
  int      id;                  /* item identifier */
  float    wgt;                 /* weight of the item */
} XTITEM;                       /* (extended transaction item) */

typedef struct {                /* --- an extended transaction --- */
  int      wgt;                 /* weight (number of occurrences) */
  int      size;                /* size   (number of items) */
  XTITEM   items[1];            /* items in the transaction */
} XTRACT;                       /* (extended transaction) */

typedef struct {                /* --- an item base --- */
  NIMAP    *nimap;              /* name/identifier map */
  int      wgt;                 /* total weight of transactions */
  int      app;                 /* default appearance indicator */
  double   pen;                 /* default insertion penalty */
  int      isext;               /* flag for an extended transaction */
  int      idx;                 /* index of current transaction */
  int      size;                /* size of the transaction buffer */
  void     *tract;              /* buffer for a transaction */
  int      err;                 /* error code (file reading) */
  TABREAD  *trd;                /* table/transaction reader */
} ITEMBASE;                     /* (item base) */

typedef struct {                /* --- a transaction bag/multiset --- */
  ITEMBASE *base;               /* underlying item base */
  int      max;                 /* number of items in largest trans. */
  int      wgt;                 /* total weight of transactions */
  int      extent;              /* total number of item instances */
  int      isext;               /* flag for extended transactions */
  int      size;                /* size of the transaction array */
  int      cnt;                 /* number of transactions */
  void     **tracts;            /* array  of transactions */
  int      *icnts;              /* number of transactions per item */
  int      *ifrqs;              /* frequency of the items (weight) */
} TABAG;                        /* (transaction bag/multiset) */

typedef struct {                /* --- a transaction tree node --- */
  int      wgt;                 /* weight (number of transactions) */
  int      max;                 /* number of items in largest trans. */
  int      size;                /* node size (number of children) */
  int      items[1];            /* next items in rep. transactions */
} TTNODE;                       /* (transaction tree node) */

typedef struct {                /* --- a transaction tree --- */
  ITEMBASE *base;               /* underlying item base */
  TTNODE   *root;               /* root of the transaction tree */
} TATREE;                       /* (transaction tree) */

/*----------------------------------------------------------------------
  Item Base Functions
----------------------------------------------------------------------*/
extern ITEMBASE*  ib_create   (int ext, int size);
extern void       ib_delete   (ITEMBASE *base);

extern int        ib_isext    (ITEMBASE *base);
extern int        ib_cnt      (ITEMBASE *base);
extern int        ib_add      (ITEMBASE *base, const char *name);
extern int        ib_item     (ITEMBASE *base, const char *name);
extern CCHAR*     ib_name     (ITEMBASE *base, int item);

extern int        ib_getwgt   (ITEMBASE *base);
extern int        ib_setwgt   (ITEMBASE *base, int n);
extern int        ib_incwgt   (ITEMBASE *base, int n);

extern int        ib_getapp   (ITEMBASE *base, int item);
extern int        ib_setapp   (ITEMBASE *base, int item, int app);
extern int        ib_getfrq   (ITEMBASE *base, int item);
extern int        ib_setfrq   (ITEMBASE *base, int item, int frq);
extern int        ib_incfrq   (ITEMBASE *base, int item, int frq);
extern int        ib_getxfq   (ITEMBASE *base, int item);
extern int        ib_setxfq   (ITEMBASE *base, int item, int xfq);
extern int        ib_incxfq   (ITEMBASE *base, int item, int xfq);
extern double     ib_getpen   (ITEMBASE *base, int item);
extern double     ib_setpen   (ITEMBASE *base, int item, double pen);

extern int        ib_readsel  (ITEMBASE *base, TABREAD *trd);
extern int        ib_readapp  (ITEMBASE *base, TABREAD *trd);
extern int        ib_readpen  (ITEMBASE *base, TABREAD *trd);
extern int        ib_read     (ITEMBASE *base, TABREAD *trd, int mode);
extern CCHAR*     ib_errmsg   (ITEMBASE *base, char *buf, size_t size);

extern int        ib_recode   (ITEMBASE *base, int min, int max,
                               int dir, int *map);
extern void       ib_trunc    (ITEMBASE *base, int n);

extern TRACT*     ib_tract    (ITEMBASE *base);
extern XTRACT*    ib_xtract   (ITEMBASE *base);

/*----------------------------------------------------------------------
  Transaction Functions
----------------------------------------------------------------------*/
extern TRACT*     t_create    (const int *items, int n, int wgt);
extern void       t_delete    (TRACT *t);
extern TRACT*     t_clone     (const TRACT *t);

extern const int* t_items     (const TRACT *t);
extern int        t_size      (const TRACT *t);
extern int        t_wgt       (const TRACT *t);

extern void       t_sort      (TRACT *t);
extern void       t_reverse   (TRACT *t);
extern int        t_unique    (TRACT *t);

extern int        t_cmp       (const void *p1,
                               const void *p2, void *data);
extern int        t_cmpx      (const TRACT *t, const int *items, int n);
extern int        t_cmpsz     (const void *p1,
                               const void *p2, void *data);
#ifndef NDEBUG
extern void       t_show      (TRACT *t, ITEMBASE *base);
#endif

/*----------------------------------------------------------------------
  Extended Transaction Item Functions
----------------------------------------------------------------------*/
extern int        xi_cmp      (const XTITEM *a, const XTITEM *b);
extern void       xi_sort     (XTITEM *xia, int n);
extern void       xi_reverse  (XTITEM *xia, int n);
extern int        xi_unique   (XTITEM *xia, int n);

/*----------------------------------------------------------------------
  Extended Transaction Functions
----------------------------------------------------------------------*/
extern XTRACT*    xt_create   (int size, int wgt);
extern void       xt_delete   (XTRACT *t);
extern XTRACT*    xt_clone    (const XTRACT *t);

extern void       xt_add      (XTRACT *t, int item, float wgt);
extern XTITEM*    xt_items    (const XTRACT *t);
extern int        xt_size     (const XTRACT *t);
extern int        xt_wgt      (const XTRACT *t);

extern void       xt_sort     (XTRACT *t);
extern void       xt_reverse  (XTRACT *t);
extern int        xt_unique   (XTRACT *t);

extern int        xt_cmp      (const void *p1,
                               const void *p2, void *data);

/*----------------------------------------------------------------------
  Transaction Bag/Multiset Functions
----------------------------------------------------------------------*/
extern TABAG*     tbg_create  (ITEMBASE *base);
extern void       tbg_delete  (TABAG *bag, int delis);
extern ITEMBASE*  tbg_base    (TABAG *bag);

extern int        tbg_isext   (const TABAG *bag);
extern int        tbg_itemcnt (const TABAG *bag);
extern int        tbg_cnt     (const TABAG *bag);
extern int        tbg_wgt     (const TABAG *bag);
extern int        tbg_max     (const TABAG *bag);
extern int        tbg_extent  (const TABAG *bag);
extern const int* tbg_icnts   (TABAG *bag);
extern const int* tbg_ifrqs   (TABAG *bag);

extern int        tbg_add     (TABAG *bag,  TRACT *t);
extern int        tbg_addx    (TABAG *bag, XTRACT *t);
extern int        tbg_addib   (TABAG *bag);
extern TRACT*     tbg_tract   (TABAG *bag, int index);
extern XTRACT*    tbg_xtract  (TABAG *bag, int index);
extern int        tbg_read    (TABAG *bag, TABREAD *trd, int mode);
extern CCHAR*     tbg_errmsg  (TABAG *bag, char *buf, size_t size);

extern void       tbg_recode  (TABAG *bag, int *map);
extern void       tbg_filter  (TABAG *bag, int min, const int *marks);
extern void       tbg_trim    (TABAG *bag, int min, const int *marks);
extern void       tbg_itsort  (TABAG *bag, int dir, int heap);
extern void       tbg_mirror  (TABAG *bag);
extern void       tbg_sort    (TABAG *bag, int dir, int heap);
extern void       tbg_sortsz  (TABAG *bag, int dir, int heap);
extern void       tbg_reverse (TABAG *bag);
extern int        tbg_reduce  (TABAG *bag);
extern int        tbg_occur   (TABAG *bag, const int *items, int n);

#ifndef NDEBUG
extern void       tbg_show    (TABAG *bag);
#endif

/*----------------------------------------------------------------------
  Transaction Tree Functions
----------------------------------------------------------------------*/
extern TATREE*    tt_create   (TABAG *bag);
extern void       tt_delete   (TATREE *tree, int delis);
extern ITEMBASE*  tt_base     (TATREE *tree);

extern TTNODE*    tt_root     (TATREE *tree);
extern int        tt_nodecnt  (const TATREE *tree);
extern int        tt_extcnt   (const TATREE *tree);
extern int        tt_wgt      (const TATREE *tree);
extern int        tt_max      (const TATREE *tree);

#ifndef NDEBUG
extern void       tt_show     (TATREE *tree);
#endif

/*----------------------------------------------------------------------
  Transaction Tree Node Functions
----------------------------------------------------------------------*/
extern int        ttn_wgt     (const TTNODE *node);
extern int        ttn_max     (const TTNODE *node);
extern int        ttn_size    (const TTNODE *node);
extern int*       ttn_items   (TTNODE *node);
extern int        ttn_item    (const TTNODE *node, int index);
extern TTNODE*    ttn_child   (const TTNODE *node, int index);

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define ib_isext(s)       ((s)->isext)
#define ib_cnt(s)         nim_cnt((s)->nimap)
#define ib_item(s,n)      nim_getid((s)->nimap, n)
#define ib_name(s,i)      nim_name(nim_byid((s)->nimap, i))

#define ib_getwgt(s)      ((s)->wgt)
#define ib_setwgt(s,n)    ((s)->wgt  = (n))
#define ib_incwgt(s,n)    ((s)->wgt += (n))

#define ib_getapp(s,i)    (((ITEM*)nim_byid((s)->nimap, i))->app)
#define ib_setapp(s,i,a)  (((ITEM*)nim_byid((s)->nimap, i))->app  = (a))
#define ib_getfrq(s,i)    (((ITEM*)nim_byid((s)->nimap, i))->frq)
#define ib_setfrq(s,i,n)  (((ITEM*)nim_byid((s)->nimap, i))->frq  = (n))
#define ib_incfrq(s,i,n)  (((ITEM*)nim_byid((s)->nimap, i))->frq += (n))
#define ib_getxfq(s,i)    (((ITEM*)nim_byid((s)->nimap, i))->xfq)
#define ib_setxfq(s,i,n)  (((ITEM*)nim_byid((s)->nimap, i))->xfq  = (n))
#define ib_incxfq(s,i,n)  (((ITEM*)nim_byid((s)->nimap, i))->xfq += (n))
#define ib_getpen(s,i)    (((ITEM*)nim_byid((s)->nimap, i))->pen)
#define ib_setpen(s,i,p)  (((ITEM*)nim_byid((s)->nimap, i))->pen  = (p))

#define ib_tract(s)       ((TRACT*) (s)->tract)
#define ib_xtract(s)      ((XTRACT*)(s)->tract)

/*--------------------------------------------------------------------*/
#define t_delete(t)       free(t)
#define t_sort(t)         int_qsort  ((t)->items, (t)->size)
#define t_reverse(t)      int_reverse((t)->items, (t)->size)
#define t_unique(t)       int_unique ((t)->items, (t)->size)
#define t_items(t)        ((const int*)(t)->items)
#define t_size(t)         ((t)->size)
#define t_wgt(t)          ((t)->wgt)

/*--------------------------------------------------------------------*/
#define xt_delete(t)      free(t)
#define xt_items(t)       ((t)->items)
#define xt_size(t)        ((t)->size)
#define xt_wgt(t)         ((t)->wgt)

/*--------------------------------------------------------------------*/
#define tbg_base(b)       ((b)->base)

#define tbg_isext(b)      ((b)->isext)
#define tbg_itemcnt(b)    ib_cnt((b)->base)
#define tbg_cnt(b)        ((b)->cnt)
#define tbg_wgt(b)        ((b)->wgt)
#define tbg_max(b)        ((b)->max)
#define tbg_extent(b)     ((b)->extent)

#define tbg_tract(b,i)    ((TRACT*) (b)->tracts[i])
#define tbg_xtract(b,i)   ((XTRACT*)(b)->tracts[i])
#define tbg_errmsg(b,s,n) ib_errmsg((b)->base, s, n)
#define tbg_reverse(b)    ptr_reverse((b)->tracts, (b)->cnt)

/*--------------------------------------------------------------------*/
#define tt_base(t)        ((t)->base)
#define tt_root(t)        ((t)->root)
#define tt_wgt(t)         ((t)->root->wgt)
#define tt_max(t)         ((t)->root->max)

/*--------------------------------------------------------------------*/
#define ttn_wgt(n)        ((n)->wgt)
#define ttn_max(n)        ((n)->max)
#define ttn_size(n)       ((n)->size)
#define ttn_item(n,i)     ((n)->items[i])
#define ttn_items(n)      ((n)->items)
#ifndef ALIGN8
#define ttn_child(n,i)    (((TTNODE**)((n)->items +(n)->size))[i])
#endif

#endif
