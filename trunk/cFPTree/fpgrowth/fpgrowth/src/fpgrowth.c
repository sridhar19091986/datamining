/*----------------------------------------------------------------------
  File    : fpgrowth.c
  Contents: fpgrowth algorithm for finding frequent item sets
  Author  : Christian Borgelt
  History : 2004.11.21 file created from eclat.c
            2004.11.23 absolute/relative support output changed
            2004.12.09 filter added (binary logarithm of supp. quotient)
            2005.06.20 use of flag for "no item sorting" corrected
            2007.02.13 adapted to modified module tabread
            2008.01.22 special processing of chains added
            2008.05.02 default limit for maximal number of items removed
            2008.09.01 adapted to modified modules tract and report
            2008.09.02 pruning with perfect extensions added
            2008.09.09 more flexible information output control added
            2008.10.26 maximum item set size removed from recursion
            2008.10.31 adapted to changes in item set reporting
            2008.11.13 adapted to changes in transaction management
            2009.05.28 adapted to modified function tbg_filter()
            2009.10.15 adapted to item set counter in reporter
            2009.10.18 treatment of item chains simplified
            2009.10.19 closed and maximal item set mining added
            2010.02.08 adapted to new repository for filtering
            2010.03.03 copy pointer removed from structure FPNODE
            2010.03.04 memory system eliminated (now array of nodes)
            2010.03.17 head union tail pruning for maximal sets added
            2010.07.14 output file made optional (for benchmarking)
            2010.07.20 created projections are no longer reallocated
            2010.07.25 separate frequent pattern tree module removed
            2010.07.26 fixed frequent pattern tree for each recursion
            2010.07.27 variant with children/sibling nodes added
            2010.07.28 automatic choice of algorithm variant added
            2010.07.29 variant added that processes the tree top-down
            2010.07.30 simple node structure variant redesigned
            2010.08.03 complex node variant with item reordering added
            2010.08.05 top-down processing with single tree added
            2010.08.19 item selection file added as optional input
            2010.08.22 adapted to modified modules tabread and tract
            2010.08.30 bug in header table initialization fixed
            2010.10.15 adapted to modified interface of module report
            2010.11.24 adapted to modified error reporting (tract)
            2010.12.07 added some explicit type casts (for C++)
            2010.12.11 adapted to a generic error reporting function
            2010.12.20 adapted to function tbg_ifrqs() (filter problem)
            2011.03.20 optional integer transaction weights added
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "memsys.h"
#include "scanner.h"
#include "tract.h"
#ifndef ISR_CLOMAX
#define ISR_CLOMAX
#endif
#include "report.h"
#include "error.h"

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define PRGNAME     "fpgrowth"
#define DESCRIPTION "find frequent item sets " \
                    "with the fpgrowth algorithm"
#define VERSION     "version 4.17 (2011.03.20)        " \
                    "(c) 2004-2011   Christian Borgelt"

/* --- fpgrowth variant --- */
#define AV_AUTO        0        /* automatic choice based on density */
#define AV_SIMPLE      1        /* simple  nodes (parent/link) */
#define AV_COMPLEX     2        /* complex nodes (children/sibling) */
#define AV_REORDER     3        /* complex nodes and item reordering */
#define AV_TOPDOWN     4        /* top-down processing of the tree */
#define AV_SINGLE      5        /* top-down processing w/ single tree */

/* --- operation modes --- */
#define PR_PERFECT  0x10        /* perfect extension pruning */
#define PR_TAIL     0x20        /* head union tail pruning */
#define PR_DEFAULT  (PR_PERFECT|PR_TAIL)

/* --- additional evaluation measures --- */
#define EM_NONE        0        /* no measure */
#define EM_LOGQ        1        /* binary log. of support quotient */

/* --- error codes --- */
/* error codes   0 to  -4 defined in tract.h */
#define E_STDIN      (-5)       /* double assignment of stdin */
#define E_OPTION     (-6)       /* unknown option */
#define E_OPTARG     (-7)       /* missing option argument */
#define E_ARGCNT     (-8)       /* too few/many arguments */
#define E_TARGET     (-9)       /* invalid target type */
#define E_SIZE      (-10)       /* invalid item set size */
#define E_SUPPORT   (-11)       /* invalid item set support */
#define E_VARIANT   (-12)       /* invalid algorithm variant */
#define E_MEASURE   (-13)       /* invalid evaluation measure */
/* error codes -15 to -25 defined in tract.h */

#ifndef QUIET                   /* if not quiet version, */
#define MSG         fprintf     /* print messages */
#else                           /* if quiet version, */
#define MSG(...)                /* suppress messages */
#endif

#define SEC_SINCE(t)  ((clock()-(t)) /(double)CLOCKS_PER_SEC)
#define COPYERR       ((TDNODE*)-1)

/*----------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------*/
typedef struct _fpnode {        /* --- frequent pattern tree node --- */
  int            item;          /* associated item (last item in set) */
  int            supp;          /* support (weight of transactions) */
  struct _fpnode *parent;       /* parent node (preceding item) */
  struct _fpnode *link;         /* next node with same item */
} FPNODE;                       /* (frequent pattern tree node) */

typedef struct {                /* --- freq. pat. tree node list --- */
  int      item;                /* associated item (map to original) */
  int      supp;                /* support (weight of transactions) */
  FPNODE   *head;               /* head of the item list */
} FPLIST;                       /* (frequent pattern tree node list) */

typedef struct {                /* --- frequent pattern tree --- */
  int      cnt;                 /* number of items / node lists */
  MEMSYS   *mem;                /* memory system for the nodes */
  FPNODE   root;                /* root node connecting trees */
  FPLIST   lists[1];            /* array of one node list per item */
} FPTREE;                       /* (frequent pattern tree) */

typedef struct _csnode {        /* --- children/sibling tree node --- */
  int             item;         /* associated item (last item in set) */
  int             supp;         /* support (weight of transactions) */
  struct _csnode *children;     /* list of child nodes */
  struct _csnode *sibling;      /* successor node in sibling list */
  struct _csnode *parent;       /* parent node (preceding item) */
  struct _csnode *link;         /* next node with same item */
} CSNODE;                       /* (children/sibling tree node) */

typedef struct {                /* --- ch./sibling tree node list --- */
  int      item;                /* associated item (map to original) */
  int      supp;                /* support (weight of transactions) */
  CSNODE   *head;               /* head of the item list */
} CSLIST;                       /* (children/sibling tree node list) */

typedef struct {                /* --- children/sibling tree --- */
  int      cnt;                 /* number of items / node lists */
  MEMSYS   *mem;                /* memory system for the nodes */
  CSNODE   root;                /* root node connecting trees */
  CSLIST   lists[1];            /* array of one node list per item */
} CSTREE;                       /* (children/sibling tree) */

typedef struct _tdnode {        /* --- top-down tree node --- */
  int            item;          /* associated item (last item in set) */
  int            supp;          /* support of represented item set */
  struct _tdnode *children;     /* list of child nodes */
  struct _tdnode *sibling;      /* successor node in sibling list */
} TDNODE;                       /* (top-down tree node) */

typedef struct {                /* --- top-down freq. pat. tree --- */
  int      cnt;                 /* number of items / max. tree height */
  MEMSYS   *mem;                /* memory system for the nodes */
  TDNODE   *root;               /* root level of the tree */
  int      items[1];            /* item identifier map */
} TDTREE;                       /* (top-down tree) */

typedef struct {                /* --- recursion data --- */
  int      mode;                /* mode flags (e.g. PR_PERFECT) */
  int      supp;                /* minimum support (trans. weight) */
  ITEMBASE *base;               /* underlying item base */
  ISREPORT *isrep;              /* report function for item sets */
  int      *set;                /* item set for projection */
  int      *map;                /* item identifier map */
} RECDATA;                      /* (recursion data) */

/*----------------------------------------------------------------------
  Constants
----------------------------------------------------------------------*/
#ifndef QUIET                   /* if not quiet version */
/* --- error messages --- */
static const char *errmsgs[] = {
  /* E_NONE      0 */  "no error",
  /* E_NOMEM    -1 */  "not enough memory",
  /* E_FOPEN    -2 */  "cannot open file %s",
  /* E_FREAD    -3 */  "read error on file %s",
  /* E_FWRITE   -4 */  "write error on file %s",
  /* E_STDIN    -5 */  "double assignment of standard input",
  /* E_OPTION   -6 */  "unknown option -%c",
  /* E_OPTARG   -7 */  "missing option argument",
  /* E_ARGCNT   -8 */  "wrong number of arguments",
  /* E_TARGET   -9 */  "invalid target type '%c'",
  /* E_SIZE    -10 */  "invalid item set size %d",
  /* E_SUPPORT -11 */  "invalid minimum support %g",
  /* E_VARIANT -12 */  "invalid fpgrowth variant '%c'",
  /* E_MEASURE -13 */  "invalid evaluation measure '%c'",
  /*           -14 */  NULL,
  /* E_NOITEMS -15 */  "no (frequent) items found",
  /*           -16 */  "unknown error"
};
#endif

/*----------------------------------------------------------------------
  Global Variables
----------------------------------------------------------------------*/
#ifndef QUIET
static char     *prgname;       /* program name for error messages */
#endif
static TABREAD  *tread = NULL;  /* table/transaction reader */
static ITEMBASE *ibase = NULL;  /* item base */
static TABAG    *tabag = NULL;  /* transaction bag/multiset */
static ISREPORT *isrep = NULL;  /* item set reporter */

/*----------------------------------------------------------------------
  Auxiliary Functions for Debugging
----------------------------------------------------------------------*/
#ifndef NDEBUG

static void indent (int k)
{ while (--k >= 0) printf("   "); }

/*--------------------------------------------------------------------*/

static void fpt_show (FPTREE *fpt, int ind)
{                               /* --- show a freq. pattern tree */
  int    i, k;                  /* loop variable */
  FPNODE *node;                 /* to traverse the node lists */

  assert(fpt);                  /* check the function arguments */
  printf("\n"); indent(ind); printf("------\n");
  indent(ind);                  /* indent the output line */
  printf("* (%d)\n", fpt->root.supp);
  for (i = 0; i < fpt->cnt; i++) {  /* traverse the items */
    indent(ind);                /* indent the output line */
    k = fpt->lists[i].item;     /* print item and support */
    printf("%s/%d:%d", ib_name(ibase, k), k, fpt->lists[i].supp);
    for (node = fpt->lists[i].head; node; node = node->link) {
      printf(" %d", node->supp);/* print the node support */
      k = node->parent->item;   /* get the parent item */
      if (k < 0) printf("[*]"); /* print a root node indicator */
      else { k = fpt->lists[k].item;
             printf("[%s/%d]", ib_name(ibase, k), k); }
    }                           /* print the item in the parent */
    printf("\n");               /* terminate line after each node */
  }
  indent(ind); printf("------\n");
}  /* fpt_show() */

/*--------------------------------------------------------------------*/

static void cst_srec (CSNODE *node, int ind, CSLIST *lists)
{                               /* --- recursively show nodes */
  int i;                        /* mapped item identifier */

  assert(ind >= 0);             /* check the function arguments */
  while (node) {                /* traverse the node list */
    indent(ind);                /* indent the output line */
    i = lists[node->item].item; /* get the original item code */
    printf("%s/%d:", ib_name(ibase, i), i);
    printf("%d\n", node->supp); /* print the node information */
    cst_srec(node->children, ind+1, lists);
    node = node->sibling;       /* recursively show the child nodes, */
  }                             /* then go to the next node */
}  /* cst_srec() */

/*--------------------------------------------------------------------*/

static void cst_show (CSTREE *cst, int ind)
{                               /* --- show a freq. pattern tree */
  assert(cst && (ind >= 0));    /* check the function arguments */
  printf("\n"); indent(ind); printf("------\n");
  indent(ind);                  /* indent the output line */
  printf("*:%d\n", cst->root.supp);
  cst_srec(cst->root.children, ind+1, cst->lists);
  indent(ind); printf("------\n");
}  /* cst_show() */

/*--------------------------------------------------------------------*/

static void tdt_srec (TDNODE *node, int ind, const int *items)
{                               /* --- recursively show nodes */
  assert(ind >= 0);             /* check the function arguments */
  while (node) {                /* traverse the node list */
    indent(ind);                /* indent the output line */
    printf("%s/", ib_name(ibase, items[node->item]));
    printf("%d:", items[node->item]);
    printf("%d\n", node->supp); /* print the node information */
    tdt_srec(node->children, ind+1, items);
    node = node->sibling;       /* recursively show the child nodes, */
  }                             /* then go to the next node */
}  /* tdt_srec() */

/*--------------------------------------------------------------------*/

static void tdt_show (TDTREE *tdt, int ind)
{                               /* --- show a freq. pattern tree */
  assert(tdt && (ind >= 0));    /* check the function arguments */
  printf("\n"); indent(ind); printf("------\n");
  if (!tdt->root) { indent(ind); printf("(null)\n"); }
  else tdt_srec(tdt->root, ind, tdt->items);
  indent(ind); printf("------\n");  /* show the nodes recursively */
}  /* tdt_show() */

#endif  /* #ifndef NDEBUG */
/*----------------------------------------------------------------------
  Frequent Pattern Growth (simple nodes with only link/parent)
----------------------------------------------------------------------*/

static int add_simple (FPTREE *fpt, const int *items, int n, int supp)
{                               /* --- add an item set to the tree */
  int    i;                     /* buffer for an item */
  FPNODE *c, *node;             /* to create new (child) nodes */

  assert(fpt                    /* check the function arguments */
  &&    (items || (n <= 0)) && (supp >= 0));
  node = &fpt->root;            /* start at the root node and */
  while (1) {                   /* traverse the items of the set */
    node->supp += supp;         /* update the item set support */
    if (--n < 0) return 0;      /* if all items are processed, abort */
    i = *items++;               /* get the next item in the set */
    c = fpt->lists[i].head;     /* and the corresponding list head */
    if (!c || (c->parent != node)) break;
    node = c;                   /* if last set is not matched, abort, */
  }                             /* otherwise go to the existing node */
  while (1) {                   /* traverse the new items */
    c = (FPNODE*)ms_alloc(fpt->mem);
    if (!c) return -1;          /* allocate a new tree node */
    c->item   = i;              /* store the next item and */
    c->supp   = supp;           /* the support of the item set */
    c->parent = node;           /* connect to the parent node */
    c->link   = fpt->lists[i].head;   /* add the new node */
    fpt->lists[i].head = node = c;    /* to the item list */
    if (--n < 0) return 1;      /* if there are no more items, */
    i = *items++;               /* abort the insertion loop, */
  }                             /* otherwise get the next item */
}  /* add_simple() */

/*--------------------------------------------------------------------*/

static int proj_simple (FPTREE *dst, FPTREE *src, int item, RECDATA *rd)
{                               /* --- project a freq. pattern tree */
  int    i, n;                  /* loop variables, item buffer */
  int    min;                   /* minimum support for perf. exts. */
  int    *s, *d;                /* to traverse the items / support */
  FPLIST *list;                 /* to access the node lists */
  FPNODE *node, *anc;           /* to traverse the tree nodes */

  assert(dst                    /* check the function arguments */
  &&     src && (item >= 0) && rd);
  memset(s = rd->map, 0, item *sizeof(int));
  for (node = src->lists[item].head; node; node = node->link)
    for (anc = node->parent; anc->item >= 0; anc = anc->parent)
      s[anc->item] += node->supp;  /* compute the conditional support */
  /* Using a two-dimensional table that is filled when a frequent */
  /* pattern tree is created proved to be slower than the above.  */
  min = (rd->mode & PR_PERFECT) /* get min. support for perfect exts. */
      ? src->lists[item].supp : INT_MAX;
  for (i = n = 0; i < item; i++) {
    if (s[i] <  rd->supp) {     /* traverse items and their support */
      s[i] = -1; continue; }    /* eliminate infrequent items and */
    if (s[i] >= min) {          /* collect perfect extension items */
      s[i] = -1; isr_addpex(rd->isrep, src->lists[i].item); continue; }
    list = dst->lists +n;       /* init. the destination item list */
    list->item = src->lists[i].item;
    list->supp = s[i]; s[i] = n++;
    list->head = NULL;          /* build the item identifier maps */
  }                             /* and store the item support */
  if (n <= 0) return 0;         /* if the projection is empty, abort */
  dst->cnt       = n;           /* note the number of items and */
  dst->root.supp = 0;           /* init. root node and node lists */
  for (node = src->lists[item].head; node; node = node->link) {
    for (d = s, anc = node->parent; anc->item >= 0; anc = anc->parent)
      if ((i = s[anc->item]) >= 0) *--d = i;
    if (add_simple(dst, d, s-d, node->supp) < 0)
      return -1;                /* collect the non-eliminated items */
  }                             /* and add the red. trans to the tree */
  return 1;                     /* return that result is not empty */
}  /* proj_simple() */

/*--------------------------------------------------------------------*/

static int rec_simple (FPTREE *fpt, RECDATA *rd)
{                               /* --- find item sets recursively */
  int    i, r = 0;              /* loop variable, error status */
  FPLIST *l;                    /* node list for current item */
  FPNODE *node;                 /* to traverse the tree nodes */
  FPTREE *proj = NULL;          /* projected frequent pattern tree */
  int    *s;                    /* to collect the tail items */

  assert(fpt && rd);            /* check the function arguments */
  if (rd->mode & PR_TAIL) {     /* if to use head union tail pruning */
    for (s = rd->set, i = 0; i < fpt->cnt; i++)
      s[i] = fpt->lists[i].item;/* collect the tail items */
    r = isr_tail(rd->isrep, s, i);
    if (r) return r;            /* if tail needs not to be processed, */
  }                             /* abort the recursion */
  if ((fpt->cnt > 1)            /* if there is more than one item */
  &&  isr_xable(rd->isrep, 2)){ /* and another item can be added */
    proj = (FPTREE*)malloc(sizeof(FPTREE) +(fpt->cnt-2)*sizeof(FPLIST));
    if (!proj) return -1;       /* create a frequent pattern tree */
    proj->root.item = -1;       /* of the maximally possible size */
    proj->root.link = proj->root.parent = NULL;
    proj->mem = fpt->mem;       /* initialize the root node */
    if (ms_push(fpt->mem) < 0) { free(proj); return -1; }
  }                             /* note the current memory state */
  for (i = fpt->cnt; --i >= 0; ) {
    l = fpt->lists +i;          /* traverse the frequent items */
    r = isr_add(rd->isrep, l->item, l->supp);
    if (r <  0) break;          /* add current item to the reporter */
    if (r <= 0) continue;       /* check if item needs processing */
    if (!l->head->link) {       /* if projection would be a chain */
      for (node = l->head->parent; node->item >= 0; ) {
        isr_addpex(rd->isrep, fpt->lists[node->item].item);
        node = node->parent;    /* traverse the list of ancestors */
      } }                       /* and add them as perfect exts. */
    else if (proj) {            /* if another item can be added */
      r = proj_simple(proj, fpt, i, rd);
      if (r > 0) r = rec_simple(proj, rd);
      if (r < 0) break;         /* project frequent pattern tree and */
    }                           /* find freq. item sets recursively */
    isr_report(rd->isrep);      /* report the current item set */
    isr_remove(rd->isrep, 1);   /* and remove the current item */
  }                             /* from the item set reporter */
  if (proj) {                   /* delete the created projection */
    free(proj); ms_pop(fpt->mem); }
  return r;                     /* return the error status */
}  /* rec_simple() */

/*--------------------------------------------------------------------*/

int fpg_simple (TABAG *tabag, int mode, int supp, ISREPORT *isrep)
{                               /* --- search for frequent item sets */
  int       i, m;               /* loop variable, buffer */
  int       n, k;               /* number of items / transactions */
  int       min;                /* minimum support for perf. exts. */
  int       *s, *d;             /* to traverse flags / counters */
  const int *c;                 /* to traverse transaction items */
  TRACT     *t;                 /* to traverse the transactions */
  FPTREE    *fpt;               /* created frequent pattern tree */
  FPLIST    *l;                 /* to traverse the item lists */
  RECDATA   rd;                 /* structure for recursive search */

  assert(tabag && isrep);       /* check the function arguments */
  if (!(mode & ISR_MAXIMAL)) mode &= ~PR_TAIL;
  rd.mode  = mode;              /* make mode consistent and store it */
  rd.supp  = (supp > 0) ? supp : 1;   /* check and adapt the support */
  min      = tbg_wgt(tabag);    /* check against the minimum support */
  if (min < rd.supp) return 0;  /* and get minimum for perfect exts. */
  if (!(mode & PR_PERFECT)) min = INT_MAX;
  rd.isrep = isrep;             /* initialize the recursion data */
  n = tbg_itemcnt(tabag);       /* get the number of items */
  if (n <= 0) { isr_report(isrep); return 0; }
  c = tbg_ifrqs(tabag);         /* get the item frequencies */
  if (!c) return -1;            /* in the transaction bag and */
  k = tbg_cnt(tabag);           /* the number of transactions */
  s = rd.set = (int*)malloc((n+n)*sizeof(int));
  if (!s) return -1;            /* create item and support arrays */
  rd.map  = d = s+n;            /* note item map and set buffer */
  rd.base = tbg_base(tabag);    /* get the underlying item base */
  for (i = m = 0; i < n; i++) { /* build the item identifier map */
    if (c[i] <  rd.supp) { d[i] = -1;                       continue; }
    if (c[i] >= min)     { d[i] = -1; isr_addpex(isrep, i); continue; }
    d[i] = m; s[m++] = i;       /* eliminate infrequent items and */
  }                             /* collect perfect extension items */
  if (m <= 0) {                 /* check whether there are items left */
    isr_report(isrep); free(rd.set); return 0; }
  fpt = (FPTREE*)malloc(sizeof(FPTREE) +(m-1)*sizeof(FPLIST));
  if (!fpt) { free(rd.set); return -1; }
  fpt->cnt = n = m;             /* allocate the base tree structure */
  fpt->mem = ms_create(sizeof(FPNODE), 65535);    /* block size 1MB */
  if (!fpt->mem) { free(fpt); free(rd.set); return -1; }
  fpt->root.item = -1;          /* create memory system for the nodes */
  fpt->root.supp =  0;          /* and initialize the root node */
  fpt->root.link = fpt->root.parent = NULL;
  for (i = 0; i < n; i++) {     /* initialize the item lists (header) */
    l = fpt->lists+i; l->supp = c[l->item = s[i]]; l->head = NULL; }
  for (i = k; --i >= 0; ) {     /* traverse the transactions and */
    t = tbg_tract(tabag, i);    /* collect the non-eliminated items */
    for (n = 0, c = t_items(t); *c >= 0; c++)
      if ((m = d[*c]) >= 0) s[n++] = m;
    k = add_simple(fpt, s, n, t_wgt(t));
    if (k < 0) break;           /* add the reduced transaction */
  }                             /* to the frequent pattern tree */
  if (k >= 0) {                 /* if a frequent pattern tree */
    k = rec_simple(fpt, &rd);   /* has successfully been built, */
    isr_report(isrep);          /* find freq. item sets recursively */
  }                             /* and report the empty item set */
  ms_delete(fpt->mem);          /* delete the memory mgmt. system */
  free(fpt); free(rd.set);      /* and the frequent pattern tree */
  return k;                     /* return the error status */
}  /* fpg_simple() */

/*----------------------------------------------------------------------
  Frequent Pattern Growth (complex nodes with children/sibling)
----------------------------------------------------------------------*/

static int add_cmplx (CSTREE *cst, const int *items, int n, int supp)
{                               /* --- add an item set to the tree */
  int    i;                     /* buffer for an item */
  CSNODE *node;                 /* to traverse the nodes */
  CSNODE **p, *c;               /* to create new nodes */
  CSLIST *list;                 /* to insert into an item list */

  assert(cst                    /* check the function arguments */
  &&    (items || (n <= 0)) && (supp >= 0));
  node = &cst->root;            /* start at the root node and */
  while (1) {                   /* traverse the items of the set */
    node->supp += supp;         /* update the item set support */
    if (--n < 0) return 0;      /* if all items are processed, abort */
    i = *items++;               /* get the next item in the set and */
    p = &node->children;        /* traverse the list of children */
    while (*p && ((*p)->item < i)) p = &(*p)->sibling;
    if (!(c = *p) || (c->item != i)) break;
    node = c;                   /* find the item/insertion position */
  }                             /* and if item does not exist, abort */
  c = (CSNODE*)ms_alloc(cst->mem);
  if (!c) return -1;            /* create a new prefix tree node */
  c->item    = i;               /* store the current item and */
  c->supp    = supp;            /* the support of the item set */
  c->parent  = node;            /* connect to the parent node */
  c->sibling = *p;              /* insert the created node */
  list       = cst->lists +i;   /* into the sibling list */
  c->link    = list->head;      /* and into the item list */
  list->head = node = *p = c;
  while (--n >= 0) {            /* traverse the rest of the items */
    node->children = c = (CSNODE*)ms_alloc(cst->mem);
    if (!c) return -1;          /* create a new prefix tree node */
    c->item    = i = *items++;  /* store the next item and */
    c->supp    = supp;          /* the support of the item set */
    c->parent  = node;          /* connect to the parent node */
    c->sibling = NULL;          /* there are no siblings yet */
    list       = cst->lists +i; /* insert the created node */
    c->link    = list->head;    /* into the node list */
    list->head = node = c;      /* for the current item */
  }
  node->children = NULL;        /* last created node is a leaf */
  return 1;                     /* return that nodes were added */
}  /* add_cmplx() */

/*--------------------------------------------------------------------*/

static int proj_cmplx (CSTREE *dst, CSTREE *src, int item, RECDATA *rd)
{                               /* --- project a freq. pattern tree */
  int    i, n;                  /* loop variables, item buffer */
  int    min;                   /* minimum support for perf. exts. */
  int    *s, *d;                /* to traverse the items / support */
  CSLIST *list;                 /* to traverse the item lists */
  CSNODE *node, *anc;           /* to traverse the tree nodes */

  assert(dst                    /* check the function arguments */
  &&     src && (item >= 0) && rd);
  memset(s = rd->map, 0, item *sizeof(int));
  for (node = src->lists[item].head; node; node = node->link)
    for (anc = node->parent; anc->item >= 0; anc = anc->parent)
      s[anc->item] += node->supp;  /* compute the conditional support */
  /* Using a two-dimensional table that is filled when a frequent */
  /* pattern tree is created proved to be slower than the above.  */
  min = (rd->mode & PR_PERFECT) /* get min. support for perfect exts. */
      ? src->lists[item].supp : INT_MAX;
  for (i = n = 0; i < item; i++) {
    if (s[i] <  rd->supp) {     /* traverse items and their support */
      s[i] = -1; continue; }    /* eliminate infrequent items and */
    if (s[i] >= min) {          /* collect perfect extension items */
      s[i] = -1; isr_addpex(rd->isrep, src->lists[i].item); continue; }
    list = dst->lists +n;       /* init. the destination item list */
    list->item = src->lists[i].item;
    list->supp = s[i]; s[i] = n++;
    list->head = NULL;          /* build the item identifier maps */
  }                             /* and store the item support */
  if (n <= 0) return 0;         /* if the projection is empty, abort */
  dst->cnt           = n;       /* note the number of items and */
  dst->root.supp     = 0;       /* init. the root node support */
  dst->root.children = NULL;    /* and clear the child list */
  for (node = src->lists[item].head; node; node = node->link) {
    for (d = s, anc = node->parent; anc->item >= 0; anc = anc->parent)
      if ((i = s[anc->item]) >= 0) *--d = i;
    if (add_cmplx(dst, d, s-d, node->supp) < 0)
      return -1;                /* collect the non-eliminated items */
  }                             /* add the red. trans. to the tree */
  return 1;                     /* return that result is not empty */
}  /* proj_cmplx() */

/*--------------------------------------------------------------------*/

static int rec_cmplx (CSTREE *cst, RECDATA *rd)
{                               /* --- find item sets recursively */
  int     i, r = 0;             /* loop variable, error status */
  CSLIST *l;                    /* node list for current item */
  CSNODE *node;                 /* to traverse the tree nodes */
  CSTREE *proj = NULL;          /* projected frequent pattern tree */
  int    *s;                    /* to collect the tail items */

  assert(cst && rd);            /* check the function arguments */
  if (rd->mode & PR_TAIL) {     /* if to use head union tail pruning */
    for (s = rd->set, i = 0; i < cst->cnt; i++)
      s[i] = cst->lists[i].item;/* collect the tail items */
    r = isr_tail(rd->isrep, s, i);
    if (r) return r;            /* if tail needs not to be processed, */
  }                             /* abort the recursion */
  if ((cst->cnt > 1)            /* if there is more than one item */
  &&  isr_xable(rd->isrep, 2)){ /* and another item can be added */
    proj = (CSTREE*)malloc(sizeof(CSTREE) +(cst->cnt-2)*sizeof(CSLIST));
    if (!proj) return -1;       /* create a frequent pattern tree */
    proj->root.item = -1;       /* of the maximally possible size */
    proj->root.link = proj->root.parent = proj->root.sibling = NULL;
    proj->mem = cst->mem;       /* initialize the root node */
    if (ms_push(cst->mem) < 0) { free(proj); return -1; }
  }                             /* note the current memory state */
  for (i = cst->cnt; --i >= 0; ) {
    l = cst->lists +i;          /* traverse the frequent items */
    r = isr_add(rd->isrep, l->item, l->supp);
    if (r <  0) break;          /* add current item to the reporter */
    if (r <= 0) continue;       /* check if item needs processing */
    if (!l->head->link) {       /* if projection would be a chain */
      for (node = l->head->parent; node->item >= 0; ) {
        isr_addpex(rd->isrep, cst->lists[node->item].item);
        node = node->parent;    /* traverse the list of ancestors */
      } }                       /* and add them as perfect exts. */
    else if (proj) {            /* if another item can be added */
      r = proj_cmplx(proj, cst, i, rd);
      if (r > 0) r = rec_cmplx(proj, rd);
      if (r < 0) break;         /* project frequent pattern tree and */
    }                           /* find freq. item sets recursively */
    isr_report(rd->isrep);      /* report the current item set */
    isr_remove(rd->isrep, 1);   /* and remove the current item */
  }                             /* from the item set reporter */
  if (proj) {                   /* delete the created projection */
    free(proj); ms_pop(cst->mem); }
  return r;                     /* return the error status */
}  /* rec_cmplx() */

/*--------------------------------------------------------------------*/

int fpg_cmplx (TABAG *tabag, int mode, int supp, ISREPORT *isrep)
{                               /* --- search for frequent item sets */
  int       i, m;               /* loop variables */
  int       n, k;               /* number of items / nodes */
  int       min;                /* minimum support for perf. exts. */
  int       *s, *d;             /* to traverse flags / counters */
  const int *c;                 /* to traverse transaction items */
  TRACT     *t;                 /* to traverse the transactions */
  CSTREE    *cst;               /* created frequent pattern tree */
  CSLIST    *l;                 /* to traverse the item lists */
  RECDATA   rd;                 /* structure for recursive search */

  assert(tabag && isrep);       /* check the function arguments */
  if (!(mode & ISR_MAXIMAL)) mode &= ~PR_TAIL;
  rd.mode  = mode;              /* make mode consistent and store it */
  rd.supp  = (supp > 0) ? supp : 1;   /* check and adapt the support */
  min      = tbg_wgt(tabag);    /* check against the minimum support */
  if (min < rd.supp) return 0;  /* and get minimum for perfect exts. */
  if (!(mode & PR_PERFECT)) min = INT_MAX;
  rd.isrep = isrep;             /* initialize the recursion data */
  n = tbg_itemcnt(tabag);       /* get the number of items */
  if (n <= 0) { isr_report(isrep); return 0; }
  c = tbg_ifrqs(tabag);         /* get the item frequencies */
  if (!c) return -1;            /* in the transaction bag and */
  k = tbg_cnt(tabag);           /* the number of transactions */
  s = rd.set = (int*)malloc((n+n)*sizeof(int));
  if (!s) return -1;            /* create item and support arrays */
  rd.map  = d = s+n;            /* note item map and set buffer */
  rd.base = tbg_base(tabag);    /* get the underlying item base */
  for (i = m = 0; i < n; i++) { /* build the item identifier map */
    if (c[i] <  rd.supp) { d[i] = -1;                       continue; }
    if (c[i] >= min)     { d[i] = -1; isr_addpex(isrep, i); continue; }
    d[i] = m; s[m++] = i;       /* eliminate infrequent items and */
  }                             /* collect perfect extension items */
  if (m <= 0) {                 /* check whether there are items left */
    isr_report(isrep); free(rd.set); return 0; }
  cst = (CSTREE*)malloc(sizeof(CSTREE) +(m-1)*sizeof(CSLIST));
  if (!cst) { free(rd.set); return -1; }
  cst->cnt = n = m;             /* allocate the base tree structure */
  cst->mem = ms_create(sizeof(CSNODE), 43690);    /* block size 1MB */
  if (!cst->mem) { free(cst); free(rd.set); return -1; }
  cst->root.item    = -1;       /* create memory system for the nodes */
  cst->root.supp    =  0;       /* and initialize the root node */
  cst->root.sibling = cst->root.children = NULL;
  cst->root.link    = cst->root.parent   = NULL;
  for (i = 0; i < n; i++) {     /* initialize the item lists (header) */
    l = cst->lists+i; l->supp = c[l->item = s[i]]; l->head = NULL; }
  for (i = k; --i >= 0; ) {     /* traverse the transactions and */
    t = tbg_tract(tabag, i);    /* collect the non-eliminated items */
    for (n = 0, c = t_items(t); *c >= 0; c++)
      if ((m = d[*c]) >= 0) s[n++] = m;
    k = add_cmplx(cst, s, n, t_wgt(t));
    if (k < 0) break;           /* add the reduced transaction */
  }                             /* to the frequent pattern tree */
  if (k >= 0) {                 /* if a frequent pattern tree */
    k = rec_cmplx(cst, &rd);    /* has successfully been built, */
    isr_report(isrep);          /* find freq. item sets recursively */
  }                             /* and report the empty item set */
  ms_delete(cst->mem);          /* delete the memory mgmt. system */
  free(cst); free(rd.set);      /* and the frequent pattern tree */
  return k;                     /* return the error status */
}  /* fpg_cmplx() */

/*----------------------------------------------------------------------
  Frequent Pattern Growth (with item reordering)
----------------------------------------------------------------------*/

static int add_reord (CSTREE *cst, int *flags, int n, int supp)
{                               /* --- add an item set to the tree */
  int    i;                     /* buffer for an item */
  CSNODE *node;                 /* to traverse the nodes */
  CSNODE **p, *c;               /* to create new nodes */
  CSLIST *list;                 /* to insert into an item list */

  assert(cst                    /* check the function arguments */
  &&    (n >= 0) && (supp >= 0));
  node = &cst->root;            /* start at the root node and */
  node->supp += supp;           /* update the empty set support */
  if (n <= 0) return 0;         /* if transaction is empty, abort */
  for (i = 0; 1; i++) {         /* traverse the items in the tree */
    if (!flags[i]) continue;    /* if item is not in set, skip it */
    flags[i] = 0;               /* clear the containment flag */
    p = &node->children;        /* traverse the list of children */
    while (*p && ((*p)->item < i)) p = &(*p)->sibling;
    if (!(c = *p) || (c->item != i)) break;
    node = c;                   /* find the item/insertion position */
    node->supp += supp;         /* if the item does not exist, abort */
    if (--n <= 0) return 0;     /* otherwise update the support */
  }                             /* and check for last item */
  c = (CSNODE*)ms_alloc(cst->mem);
  if (!c) return -1;            /* create a new prefix tree node */
  c->item    = i;               /* store the current item and */
  c->supp    = supp;            /* the support of the item set */
  c->parent  = node;            /* connect to the parent node */
  c->sibling = *p;              /* insert the created node */
  list       = cst->lists +i;   /* into the sibling list */
  c->link    = list->head;      /* and into the item list */
  list->head = node = *p = c;   /* then check for last item */
  if (--n <= 0) { node->children = NULL; return 1; }
  while (1) {                   /* traverse the remaining items */
    if (!flags[++i]) continue;  /* if item is not in set, skip it */
    flags[i] = 0;               /* clear the containment flag */
    node->children = c = (CSNODE*)ms_alloc(cst->mem);
    if (!c) return -1;          /* create a new prefix tree node */
    c->item    = i;             /* store the next item and */
    c->supp    = supp;          /* the support of the item set */
    c->parent  = node;          /* connect to the parent node */
    c->sibling = NULL;          /* there are no siblings yet */
    list       = cst->lists +i; /* insert the created node */
    c->link    = list->head;    /* into the node list */
    list->head = node = c;      /* for the current item */
    if (--n <= 0) break;        /* check for last item */
  }
  node->children = NULL;        /* last created node is a leaf */
  return 1;                     /* return that nodes were added */
}  /* add_reord() */

/*--------------------------------------------------------------------*/

static int proj_reord (CSTREE *dst, CSTREE *src, int item, RECDATA *rd)
{                               /* --- project a freq. pattern tree */
  int    i, k, n;               /* loop variables, item buffer */
  int    min;                   /* minimum support for perf. exts. */
  int    *s, *d;                /* to traverse the items / support */
  CSLIST *list;                 /* to traverse the item lists */
  CSNODE *node, *anc;           /* to traverse the tree nodes */

  assert(dst                    /* check the function arguments */
  &&     src && (item >= 0) && rd);
  memset(s = rd->map, 0, item *sizeof(int));
  for (node = src->lists[item].head; node; node = node->link)
    for (anc = node->parent; anc->item >= 0; anc = anc->parent)
      s[anc->item] += node->supp;  /* compute the conditional support */
  /* Using a two-dimensional table that is filled when a frequent */
  /* pattern tree is created proved to be slower than the above.  */
  min = (rd->mode & PR_PERFECT) /* get min. support for perfect exts. */
      ? src->lists[item].supp : INT_MAX;
  for (d = rd->set, i = n = 0; i < item; i++) {
    if (s[i] <  rd->supp) {     /* traverse items and their support */
      s[i] = -1; continue; }    /* eliminate infrequent items and */
    if (s[i] >= min) {          /* collect perfect extension items */
      s[i] = -1; isr_addpex(rd->isrep, src->lists[i].item); continue; }
    d[n++] = i;                 /* collect the remaining items */
  }                             /* in a temporary buffer */
  if (n <= 0) return 0;         /* if the projection is empty, abort */
  i2i_qsort(d, n, s);           /* sort the rem. items descendingly */
  int_reverse(d, n);            /* w.r.t to their conditional support */
  for (i = 0; i < n; i++) {     /* traverse the sorted items */
    list = dst->lists +i;       /* init. the destination item list */
    list->item = src->lists[k = d[i]].item;
    list->supp = s[k]; s[k] = i;
    list->head = NULL;          /* build the item identifier maps */
  }                             /* and store the item support */
  dst->cnt           = n;       /* note the number of items and */
  dst->root.supp     = 0;       /* init. the root node support */
  dst->root.children = NULL;    /* and clear the child list */
  memset(d, 0, item *sizeof(int));
  for (node = src->lists[item].head; node; node = node->link) {
    for (n = 0, anc = node->parent; anc->item >= 0; anc = anc->parent)
      if ((i = s[anc->item]) >= 0) n += d[i] = 1;
    if (add_reord(dst, d, n, node->supp) < 0)
      return -1;                /* collect the non-eliminated items */
  }                             /* and add red. trans. to the tree */
  return 1;                     /* return 'projection created' */
}  /* proj_reord() */

/*--------------------------------------------------------------------*/

static int rec_reord (CSTREE *cst, RECDATA *rd)
{                               /* --- find item sets recursively */
  int     i, r = 0;             /* loop variable, error status */
  CSLIST *l;                    /* node list for current item */
  CSNODE *node;                 /* to traverse the tree nodes */
  CSTREE *proj = NULL;          /* projected frequent pattern tree */
  int    *s;                    /* to collect the tail items */

  assert(cst && rd);            /* check the function arguments */
  if (rd->mode & PR_TAIL) {     /* if to use head union tail pruning */
    for (s = rd->set, i = 0; i < cst->cnt; i++)
      s[i] = cst->lists[i].item;/* collect the tail items */
    r = isr_tail(rd->isrep, s, i);
    if (r) return r;            /* if tail needs not to be processed, */
  }                             /* abort the recursion */
  if ((cst->cnt > 1)            /* if there is more than one item */
  &&  isr_xable(rd->isrep, 2)){ /* and another item can be added */
    proj = (CSTREE*)malloc(sizeof(CSTREE) +(cst->cnt-2)*sizeof(CSLIST));
    if (!proj) return -1;       /* create a frequent pattern tree */
    proj->root.item = -1;       /* of the maximally possible size */
    proj->root.link = proj->root.parent = proj->root.sibling = NULL;
    proj->mem = cst->mem;       /* initialize the root node */
    if (ms_push(cst->mem) < 0) { free(proj); return -1; }
  }                             /* note the current memory state */
  for (i = cst->cnt; --i >= 0; ) {
    l = cst->lists +i;          /* traverse the frequent items */
    r = isr_add(rd->isrep, l->item, l->supp);
    if (r <  0) break;          /* add current item to the reporter */
    if (r <= 0) continue;       /* check if item needs processing */
    if (!l->head->link) {       /* if projection would be a chain */
      for (node = l->head->parent; node->item >= 0; ) {
        isr_addpex(rd->isrep, cst->lists[node->item].item);
        node = node->parent;    /* traverse the list of ancestors */
      } }                       /* and add them as perfect exts. */
    else if (proj) {            /* if another item can be added */
      r = proj_reord(proj, cst, i, rd);
      if (r > 0) r = rec_reord(proj, rd);
      if (r < 0) break;         /* project frequent pattern tree and */
    }                           /* find freq. item sets recursively */
    isr_report(rd->isrep);      /* report the current item set */
    isr_remove(rd->isrep, 1);   /* and remove the current item */
  }                             /* from the item set reporter */
  if (proj) {                   /* delete the created projection */
    free(proj); ms_pop(cst->mem); }
  return r;                     /* return the error status */
}  /* rec_reord() */

/*--------------------------------------------------------------------*/

int fpg_reord (TABAG *tabag, int mode, int supp, ISREPORT *isrep)
{                               /* --- search for frequent item sets */
  int       i, m;               /* loop variables */
  int       n, k;               /* number of items / nodes */
  int       min;                /* minimum support for perf. exts. */
  int       *s, *d;             /* to traverse flags / counters */
  const int *c;                 /* to traverse transaction items */
  TRACT     *t;                 /* to traverse the transactions */
  CSTREE    *cst;               /* created frequent pattern tree */
  CSLIST    *l;                 /* to traverse the item lists */
  RECDATA   rd;                 /* structure for recursive search */

  assert(tabag && isrep);       /* check the function arguments */
  if (!(mode & ISR_MAXIMAL)) mode &= ~PR_TAIL;
  rd.mode  = mode;              /* make mode consistent and store it */
  rd.supp  = (supp > 0) ? supp : 1;   /* check and adapt the support */
  min      = tbg_wgt(tabag);    /* check against the minimum support */
  if (min < rd.supp) return 0;  /* and get minimum for perfect exts. */
  if (!(mode & PR_PERFECT)) min = INT_MAX;
  rd.isrep = isrep;             /* initialize the recursion data */
  n = tbg_itemcnt(tabag);       /* get the number of items */
  if (n <= 0) { isr_report(isrep); return 0; }
  c = tbg_ifrqs(tabag);         /* get the item frequencies */
  if (!c) return -1;            /* in the transaction bag and */
  k = tbg_cnt(tabag);           /* the number of transactions */
  s = rd.set = (int*)malloc((n+n)*sizeof(int));
  if (!s) return -1;            /* create item and support arrays */
  rd.map  = d = s+n;            /* note item map and set buffer */
  rd.base = tbg_base(tabag);    /* get the underlying item base */
  for (i = m = 0; i < n; i++) { /* build the item identifier map */
    if (c[i] <  rd.supp) { d[i] = -1;                       continue; }
    if (c[i] >= min)     { d[i] = -1; isr_addpex(isrep, i); continue; }
    d[i] = m; s[m++] = i;       /* eliminate infrequent items and */
  }                             /* collect perfect extension items */
  if (m <= 0) {                 /* check whether there are items left */
    isr_report(isrep); free(rd.set); return 0; }
  cst = (CSTREE*)malloc(sizeof(CSTREE) +(m-1)*sizeof(CSLIST));
  if (!cst) { free(rd.set); return -1; }
  cst->cnt = n = m;             /* allocate the base tree structure */
  cst->mem = ms_create(sizeof(CSNODE), 43690);    /* block size 1MB */
  if (!cst->mem) { free(cst); free(rd.set); return -1; }
  cst->root.item    = -1;       /* create memory system for the nodes */
  cst->root.supp    =  0;       /* and initialize the root node */
  cst->root.sibling = cst->root.children = NULL;
  cst->root.link    = cst->root.parent   = NULL;
  for (i = 0; i < n; i++) {     /* initialize the item lists (header) */
    l = cst->lists+i; l->supp = c[l->item = s[i]]; l->head = NULL; }
  for (i = k; --i >= 0; ) {     /* traverse the transactions and */
    t = tbg_tract(tabag, i);    /* collect the non-eliminated items */
    for (n = 0, c = t_items(t); *c >= 0; c++)
      if ((m = d[*c]) >= 0) s[n++] = m;
    k = add_cmplx(cst, s, n, t_wgt(t));
    if (k < 0) break;           /* add the reduced transaction */
  }                             /* to the frequent pattern tree */
  if (k >= 0) {                 /* if a frequent pattern tree */
    k = rec_reord(cst, &rd);    /* has successfully been built, */
    isr_report(isrep);          /* find freq. item sets recursively */
  }                             /* and report the empty item set */
  ms_delete(cst->mem);          /* delete the memory mgmt. system */
  free(cst); free(rd.set);      /* and the frequent pattern tree */
  return k;                     /* return the error status */
}  /* fpg_reord() */

/*----------------------------------------------------------------------
  Frequent Pattern Growth (top-down processing)
----------------------------------------------------------------------*/

static int add_topdn (TDTREE *tdt, const int *items, int n, int supp)
{                               /* --- add an item set to the tree */
  int    i;                     /* buffer for an item */
  TDNODE **p;                   /* insertion position */
  TDNODE *node;                 /* to insert new nodes */

  assert(tdt                    /* check the function arguments */
  &&    (items || (n <= 0)) && (supp >= 0));
  p = &tdt->root;               /* start the search at the root node */
  while (1) {                   /* traverse the items of the set */
    if (--n < 0) return 0;      /* if all items are processed, abort */
    i = items[n];               /* get the next item in the set */
    while (*p && ((*p)->item > i)) p = &(*p)->sibling;
    node = *p;                  /* find the item/insertion position */
    if (!node || (node->item != i)) break;                    
    node->supp += supp;         /* if the item does not exist, abort */
    p = &node->children;        /* else update the item set support */
  }                             /* and get the list of children */
  node = (TDNODE*)ms_alloc(tdt->mem);
  if (!node) return -1;         /* create a new prefix tree node */
  node->item    = i;            /* store the current item and */
  node->supp    = supp;         /* the support of the item set */
  node->sibling = *p;           /* insert the created node */
  *p = node;                    /* into the sibling list */
  while (--n >= 0) {            /* traverse the rest of the items */
    node = node->children = (TDNODE*)ms_alloc(tdt->mem);
    if (!node) return -1;       /* create a new prefix tree node */
    node->item    = items[n];   /* create a new prefix tree node */
    node->supp    = supp;       /* store item and its support */
    node->sibling = NULL;       /* there are no siblings yet */
  }                             
  node->children = NULL;        /* last created node is a leaf */
  return 1;                     /* return that nodes were added */
}  /* add_topdn() */

/*--------------------------------------------------------------------*/

static void getsupp (TDNODE *node, int *supp)
{                               /* --- determine conditional support */
  for ( ; node; node = node->sibling) {
    supp[node->item] += node->supp;
    getsupp(node->children, supp);
  }                             /* recursively sum support per item */
}  /* getsupp() */

/*--------------------------------------------------------------------*/

static TDNODE* merge (TDNODE *s1, TDNODE *s2)
{                               /* --- merge two node lists */
  TDNODE *out, **end = &out;    /* output node list and end pointer */

  if (!s1) return s2;           /* if there is only one node list, */
  if (!s2) return s1;           /* simply return the other list */
  end = &out;                   /* start the output list */
  while (1) {                   /* node list merge loop */
    if      (s1->item > s2->item) {
      *end = s1; end = &s1->sibling; s1 = *end; if (!s1) break; }
    else if (s2->item > s1->item) {
      *end = s2; end = &s2->sibling; s2 = *end; if (!s2) break; }
    else {                      /* copy nodes with singular items */
      s1->children = merge(s1->children, s2->children);
      s1->supp += s2->supp;     /* merge the children recursively */
      *end = s1; end = &s1->sibling; s1 = *end; s2 = s2->sibling;
      if (!s1 || !s2) break;    /* move node from the first source */
    }                           /* to the output and delete the node */
  }                             /* from the second source */
  *end = (s1) ? s1 : s2;        /* append the remaining nodes */
  return out;                   /* return the merged top-down tree */
}  /* merge() */

/*--------------------------------------------------------------------*/

static TDNODE* copy (TDNODE *src, int *map, MEMSYS *mem)
{                               /* --- copy a top-down tree */
  int    i;                     /* new item identifier */
  TDNODE *node, *dst;           /* created copy of the node list */
  TDNODE **end = &dst;          /* end of the created copy */
  TDNODE *c, *b = NULL;         /* buffer for copied children */

  assert(src && map && mem);    /* check the function arguments */
  do {                          /* sibling list copying loop */
    c = src->children;          /* if there are children, copy them */
    if (c && ((c = copy(c, map, mem)) == COPYERR)) return COPYERR;
    i = map[src->item];         /* get the new item identifier */
    if (i >= 0) {               /* if to copy the current node */
      *end = node = (TDNODE*)ms_alloc(mem);
      if (!node) return COPYERR;/* create a copy of the current node */
      node->item  = i;          /* store the item and the support and */
      node->supp  = src->supp;  /* update the conditional support */
      node->children = c;       /* set the (copied) children */
      end = &node->sibling; }   /* get the new end of the output */
    else if (c)                 /* merge copied children to a buffer */
      b = (!b) ? c : merge(b, c);
    src = src->sibling;         /* get the next sibling */
  } while (src);                /* while there is another node */
  *end = NULL;                  /* terminate the copied list */
  return (!b) ? dst : (!dst) ? b : merge(dst, b);
}  /* copy() */                 /* merge with buffered copies */

/*--------------------------------------------------------------------*/

static int rec_topdn (TDTREE *tdt, RECDATA *rd)
{                               /* --- find item sets recursively */
  int    i, n, r = 0;           /* loop variables, error status */
  int    min;                   /* minimum for perfect extensions */
  TDTREE *proj = NULL;          /* created projection */
  TDNODE *node;                 /* to traverse the nodes */
  int    *s;                    /* to collect the tail items */

  assert(tdt && rd);            /* check the function arguments */
  if (rd->mode & PR_TAIL) {     /* if to use head union tail pruning */
    r = isr_tail(rd->isrep, tdt->items, tdt->cnt);
    if (r) return r;            /* if tail needs not be processed, */
  }                             /* abort the recursion */
  if ((tdt->cnt > 1)            /* if there is more than one item */
  &&  isr_xable(rd->isrep, 2)){ /* and another item can be added */
    proj = (TDTREE*)malloc(sizeof(TDTREE) +(tdt->cnt-2)*sizeof(TDNODE));
    if (!proj) return -1;       /* create a frequent pattern tree */
    proj->mem = tdt->mem;       /* of the maximally possible size */
    if (ms_push(tdt->mem) < 0) { free(proj); return -1; }
  }                             /* note the current memory state */
  for (node = tdt->root; node; node = tdt->root) {
    r = isr_add(rd->isrep, tdt->items[node->item], node->supp);
    if (r <  0) break;          /* add current item to the reporter */
    if (r <= 0) continue;       /* check if item needs processing */
    if (proj && node->children){/* if current node has children */
      memset(s = rd->map, 0, node->item *sizeof(int));
      getsupp(node->children,s);/* determine the conditional support */
      min = (rd->mode & PR_PERFECT) ? node->supp : INT_MAX;
      for (i = n = 0; i < node->item; i++) {
        if (s[i] <  rd->supp) { /* traverse items and their support */
          s[i] = -1; continue; }/* eliminate infrequent items and */
        if (s[i] >= min) {      /* collect perfect extension items */
          s[i] = -1; isr_addpex(rd->isrep, tdt->items[i]); continue; }
        s[i] = n; proj->items[n++] = tdt->items[i];
      }                         /* build item identifier maps */
      if (n > 0) {              /* if the projection is not empty, */
        proj->cnt  = n;         /* note the number of items */
        proj->root = copy(node->children, s, proj->mem);
        if (proj->root == COPYERR) { r = -1; break; }
        r = rec_topdn(proj, rd);
        if (r < 0) break;       /* copy the subtree for the item */
      }                         /* and process it recursively */
    }
    isr_report(rd->isrep);      /* report the current item set */
    isr_remove(rd->isrep, 1);   /* and remove the current item */
    tdt->root = merge(node->sibling, node->children);
  }                             /* prune the processed item */
  if (proj) {                   /* delete the created projection */
    free(proj); ms_pop(tdt->mem); }
  return r;                     /* return the error status */
}  /* rec_topdn() */

/*--------------------------------------------------------------------*/

int fpg_topdn (TABAG *tabag, int mode, int supp, ISREPORT *isrep)
{                               /* --- search for frequent item sets */
  int       i, m;               /* loop variable, buffer */
  int       n, k;               /* number of items / nodes */
  int       min;                /* minimum support for perf. exts. */
  int       *s, *d;             /* to traverse flags / counters */
  const int *c;                 /* to traverse transaction items */
  TRACT     *t;                 /* to traverse the transactions */
  TDTREE    *tdt;               /* top-down prefix tree */
  RECDATA   rd;                 /* structure for recursive search */

  assert(tabag && isrep);       /* check the function arguments */
  if (!(mode & ISR_MAXIMAL)) mode &= ~PR_TAIL;
  rd.mode  = mode;              /* make mode consistent and store it */
  rd.supp  = (supp > 0) ? supp : 1;   /* check and adapt the support */
  min      = tbg_wgt(tabag);    /* check against the minimum support */
  if (min < rd.supp) return 0;  /* and get minimum for perfect exts. */
  if (!(mode & PR_PERFECT)) min = INT_MAX;
  rd.isrep = isrep;             /* initialize the recursion data */
  n = tbg_itemcnt(tabag);       /* get the number of items */
  if (n <= 0) { isr_report(isrep); return 0; }
  c = tbg_ifrqs(tabag);         /* get the item frequencies */
  if (!c) return -1;            /* in the transaction bag and */
  k = tbg_cnt(tabag);           /* the number of transactions */
  s = rd.set = (int*)malloc((n+n)*sizeof(int));
  if (!s) return -1;            /* create item and support arrays */
  rd.map  = d = s+n;            /* note item set buffer and item map */
  rd.base = tbg_base(tabag);    /* get the underlying item base */
  for (i = m = 0; i < n; i++) { /* build the item identifier map */
    if (c[i] <  rd.supp) { d[i] = -1;                       continue; }
    if (c[i] >= min)     { d[i] = -1; isr_addpex(isrep, i); continue; }
    d[i] = m; s[m++] = i;       /* eliminate infrequent items and */
  }                             /* collect perfect extension items */
  if (m <= 0) {                 /* check whether there are items left */
    isr_report(isrep); free(rd.set); return 0; }
  tdt = (TDTREE*)malloc(sizeof(TDTREE) +(m-1)*sizeof(int));
  if (!tdt) { free(rd.set); return -1; }
  tdt->cnt  = n = m;            /* create a top-down prefix tree and */
  tdt->root = NULL;             /* the memory system for the nodes */
  tdt->mem  = ms_create(sizeof(TDNODE), 65536);  /* block size 1MB */
  if (!tdt->mem) { free(tdt); free(rd.set); return -1; }
  memcpy(tdt->items, d, n *sizeof(int));
  for (i = k; --i >= 0; ) {     /* traverse the transactions and */
    t = tbg_tract(tabag, i);    /* collect the non-eliminated items */
    for (n = 0, c = t_items(t); *c >= 0; c++)
      if ((m = d[*c]) >= 0) s[n++] = m;
    k = add_topdn(tdt, s, n, t_wgt(t));
    if (k < 0) break;           /* add the reduced transaction */
  }                             /* to the frequent pattern tree */
  if (k >= 0) {                 /* if a frequent pattern tree */
    k = rec_topdn(tdt, &rd);    /* has successfully been built, */
    isr_report(isrep);          /* find freq. item sets recursively */
  }                             /* and report the empty item set */
  ms_delete(tdt->mem);          /* delete the memory mgmt. system */
  free(tdt); free(rd.set);      /* delete the frequent pattern tree */
  return k;                     /* return the error status */
}  /* fpg_topdn() */

/*----------------------------------------------------------------------
  Frequent Pattern Growth (on single tree)
----------------------------------------------------------------------*/

int rec_single (FPTREE *fpt, int n, RECDATA *rd)
{                               /* --- search for frequent item sets */
  int    i, k, r;               /* loop variables, error status */
  int    min;                   /* minimum support for perf. exts. */
  FPLIST *l;                    /* node list for current item */
  FPNODE *node, *anc;           /* to traverse the tree nodes */

  assert(fpt && rd);            /* check the function arguments */
  for (i = r = 0; i < n; i++) { /* traverse the items, */
    l = fpt->lists +i;          /* but skip infrequent ones */
    if (l->supp < rd->supp) continue;
    r = isr_add(rd->isrep, l->item, l->supp);
    if (r <  0) break;          /* add current item to the reporter */
    if (r <= 0) continue;       /* check if item needs processing */
    if (!l->head->link) {       /* if projection would be a chain */
      for (anc = l->head->parent; anc->item >= 0; ) {
        isr_addpex(rd->isrep, fpt->lists[anc->item].item);
        anc = anc->parent;      /* traverse the list of ancestors */
      } }                       /* and add them as perfect exts. */
    else if (i > 0) {           /* if another item can be added */
      for (k = 0; k < i; k++) { /* clear item lists and support */
        l = fpt->lists +k; l->supp = 0; l->head = NULL; }
      for (node = fpt->lists[i].head; node; node = node->link) {
        for (anc = node->parent; anc->item >= 0; anc = anc->parent) {
          l = fpt->lists +anc->item;  /* traverse the item list */
          if (l->head == anc) break;  /* and the paths to the root */
          l->supp  += anc->supp = node->supp;
          anc->link = l->head;  /* store and sum the node support */
          l->head   = anc;      /* and insert the current ancestor */
        }                       /* into the corresp. item list */
        for ( ; anc->item >= 0; anc = anc->parent) {
          fpt->lists[anc->item].supp += node->supp;
          anc->supp += node->supp;
        }                       /* update the support values */
      }                         /* on the rest of the path */
      min = (rd->mode & PR_PERFECT) ? fpt->lists[i].supp : INT_MAX;
      for (k = r = 0; k < i; k++) {
        l = fpt->lists +k;      /* traverse the items again */
        if (l->supp < rd->supp)   continue;
        if (l->supp < min) { r++; continue; }
        l->supp = 0;            /* count the frequent items */
        isr_addpex(rd->isrep, l->item);
      }                         /* collect the perfect extensions */
      if (r > 0) r = rec_single(fpt, i, rd);
      if (r < 0) break;         /* if the projection is not empty, */
    }                           /* process it recursively */
    isr_report(rd->isrep);      /* report the current item set */
    isr_remove(rd->isrep, 1);   /* and remove the current item */
  }                             /* from the item set reporter */
  return r;                     /* return the error status */
}  /* rec_single() */

/*--------------------------------------------------------------------*/

int fpg_single (TABAG *tabag, int mode, int supp, ISREPORT *isrep)
{                               /* --- search for frequent item sets */
  int       i, m;               /* loop variables */
  int       n, k;               /* number of items / transactions */
  int       min;                /* minimum support for perf. exts. */
  int       *s, *d;             /* to traverse items / counters */
  const int *c;                 /* to traverse transaction items */
  TRACT     *t;                 /* to traverse the transactions */
  FPTREE    *fpt;               /* created frequent pattern tree */
  FPLIST    *l;                 /* to traverse the item lists */
  RECDATA   rd;                 /* structure for recursive search */

  assert(tabag && isrep);       /* check the function arguments */
  if (!(mode & ISR_MAXIMAL)) mode &= ~PR_TAIL;
  rd.mode  = mode;              /* make mode consistent and store it */
  rd.supp  = (supp > 0) ? supp : 1;   /* check and adapt the support */
  min      = tbg_wgt(tabag);    /* check against the minimum support */
  if (min < rd.supp) return 0;  /* and get minimum for perfect exts. */
  if (!(mode & PR_PERFECT)) min = INT_MAX;
  rd.isrep = isrep;             /* initialize the recursion data */
  n = tbg_itemcnt(tabag);       /* get the number of items */
  if (n <= 0) { isr_report(isrep); return 0; }
  c = tbg_ifrqs(tabag);         /* get the item frequencies */
  if (!c) return -1;            /* in the transaction bag and */
  k = tbg_cnt(tabag);           /* the number of transactions */
  s = rd.set = (int*)malloc((n+n)*sizeof(int));
  if (!s) return -1;            /* create item and support arrays */
  rd.map  = d = s+n;            /* note item map and set buffer */
  rd.base = tbg_base(tabag);    /* get the underlying item base */
  for (i = m = 0; i < n; i++) { /* build the item identifier map */
    if (c[i] <  rd.supp) { d[i] = -1;                       continue; }
    if (c[i] >= min)     { d[i] = -1; isr_addpex(isrep, i); continue; }
    d[i] = m; s[m++] = i;       /* eliminate infrequent items and */
  }                             /* collect perfect extension items */
  if (m <= 0) {                 /* check whether there are items left */
    isr_report(isrep); free(rd.set); return 0; }
  fpt = (FPTREE*)malloc(sizeof(FPTREE) +(m-1)*sizeof(FPLIST));
  if (!fpt) { free(rd.set); return -1; }
  fpt->cnt = n = m;             /* allocate the base tree structure */
  fpt->mem = ms_create(sizeof(FPNODE), 65535);    /* block size 1MB */
  if (!fpt->mem) { free(fpt); free(rd.set); return -1; }
  fpt->root.item = -1;          /* create memory system for the nodes */
  fpt->root.supp =  0;          /* and initialize the root node */
  fpt->root.link = fpt->root.parent = NULL;
  for (i = 0; i < n; i++) {     /* initialize the item lists (header) */
    l = fpt->lists+i; l->supp = c[l->item = s[i]]; l->head = NULL; }
  for (i = k; --i >= 0; ) {     /* traverse the transactions and */
    t = tbg_tract(tabag, i);    /* collect the non-eliminated items */
    for (n = 0, c = t_items(t); *c >= 0; c++)
      if ((m = d[*c]) >= 0) s[n++] = m;
    k = add_simple(fpt, s, n, t_wgt(t));
    if (k < 0) break;           /* add the reduced transaction */
  }                             /* to the frequent pattern tree */
  if (k >= 0) {                 /* if a frequent pattern tree has */
    k = rec_single(fpt, fpt->cnt, &rd); /* successfully been built, */
    isr_report(isrep);          /* find freq. item sets recursively */
  }                             /* and report the empty item set */
  ms_delete(fpt->mem);          /* delete the memory mgmt. system */
  free(fpt); free(rd.set);      /* and the frequent pattern tree */
  return k;                     /* return the error status */
}  /* fpg_single() */

/*----------------------------------------------------------------------
  Main Functions
----------------------------------------------------------------------*/

static void help (void)
{                               /* --- print add. option information */
  #ifndef QUIET
  fprintf(stderr, "\n");        /* terminate startup message */
  printf("fpgrowth algorithm variants (option -a#)\n");
  printf("  a   automatic choice based on data density (default)\n");
  printf("  s   simple  tree nodes with only link and parent\n");
  printf("  c   complex tree nodes with children and siblings\n");
  printf("  r   complex tree nodes and local item reordering\n");
  printf("  t   top-down processing of the prefix trees\n");
  printf("  d   top-down processing on a single prefix tree\n");
  printf("variants r and d do not support "
         "closed/maximal item set mining\n");
  printf("additional evaluation measures (option -e#)\n");
  printf("  x   no measure\n");
  printf("  b   binary logarithm of support quotient\n");
  printf("information output format characters (option -v#)\n");
  printf("  %%%%  a percent sign\n");
  printf("  %%i  number of items (item set size)\n");
  printf("  %%a  absolute item set support\n");
  printf("  %%s  relative item set support as a fraction\n");
  printf("  %%S  relative item set support as a percentage\n");
  printf("  %%e  additional evaluation measure\n");
  printf("  %%E  additional evaluation measure as a percentage\n");
  printf("s,S,e,E can be preceded by the number of decimal places\n");
  printf("to be printed (at most 32 places).\n");
  #endif                        /* print help information */
  exit(0);                      /* abort the program */
}  /* help() */

/*--------------------------------------------------------------------*/

#ifndef NDEBUG                  /* if debug version */
  #undef  CLEANUP               /* clean up memory and close files */
  #define CLEANUP \
  if (isrep) isr_delete(isrep, 0); \
  if (tabag) tbg_delete(tabag, 0); \
  if (tread) trd_delete(tread, 1); \
  if (ibase) ib_delete (ibase);
#endif

GENERROR(error, exit)           /* generic error reporting function */

/*--------------------------------------------------------------------*/

int main (int argc, char *argv[])
{                               /* --- main function */
  int     i, k = 0, n, w;       /* loop variables, counters */
  char    *s;                   /* to traverse the options */
  CCHAR   **optarg = NULL;      /* option argument */
  CCHAR   *fn_inp  = NULL;      /* name of the input  file */
  CCHAR   *fn_out  = NULL;      /* name of the output file */
  CCHAR   *fn_sel  = NULL;      /* name of item selection file */
  CCHAR   *recseps = NULL;      /* record  separators */
  CCHAR   *fldseps = NULL;      /* field   separators */
  CCHAR   *blanks  = NULL;      /* blank   characters */
  CCHAR   *comment = NULL;      /* comment characters */
  CCHAR   *hdr     = "";        /* record header  for output */
  CCHAR   *sep     = " ";       /* item separator for output */
  CCHAR   *dflt    = "  (%1S)"; /* default format for check */
  CCHAR   *format  = dflt;      /* format for information output */
  int     target   = 's';       /* target type (closed/maximal) */
  double  supp     = 10;        /* minimum support (in percent) */
  int     min      =  1;        /* minimum size of an item set */
  int     max      = INT_MAX;   /* maximum size of an item set */
  int     eval     =  0;        /* additional evaluation measure */
  double  minval   = 10;        /* minimum evaluation measure value */
  int     tarm     =  0;        /* mode for transaction reading */
  int     sort     =  2;        /* flag for item sorting and recoding */
  int     heap     =  1;        /* flag for heap sort vs. quick sort */
  int     report   =  0;        /* flags for item set reporting */
  int     mode     = PR_DEFAULT;/* search mode (e.g. pruning) */
  int     algo     = 'a';       /* variant of fpgrowth algorithm */
  int     *map;                 /* identifier map for recoding */
  clock_t t;                    /* timer for measurements */

  #ifndef QUIET                 /* if not quiet version */
  prgname = argv[0];            /* get program name for error msgs. */

  /* --- print usage message --- */
  if (argc > 1) {               /* if arguments are given */
    fprintf(stderr, "%s - %s\n", argv[0], DESCRIPTION);
    fprintf(stderr, VERSION); } /* print a startup message */
  else {                        /* if no arguments are given */
    printf("usage: %s [options] infile [outfile [selfile]]\n", argv[0]);
    printf("%s\n", DESCRIPTION);
    printf("%s\n", VERSION);
    printf("-t#      target type                              "
                    "(default: %c)\n", target);
    printf("         (s: frequent, c: closed, m: maximal item sets)\n");
    printf("-m#      minimum number of items per item set     "
                    "(default: %d)\n", min);
    printf("-n#      maximum number of items per item set     "
                    "(default: no limit)\n");
    printf("-s#      minimum support of an item set           "
                    "(default: %g%%)\n", supp);
    printf("         (positive: percentage, "
                     "negative: absolute number)\n");
    printf("-e#      additional evaluation measure            "
                    "(default: none)\n");
    printf("-d#      minimum value of add. evaluation measure "
                    "(default: %g%%)\n", minval);
    printf("-g       write output in scanable form "
                    "(quote certain characters)\n");
    printf("-h#      record header  for output                "
                    "(default: \"%s\")\n", hdr);
    printf("-k#      item separator for output                "
                    "(default: \"%s\")\n", sep);
    printf("-v#      output format for item set information   "
                    "(default: \"%s\")\n", format);
    printf("-q#      sort items w.r.t. their frequency        "
                    "(default: %d)\n", sort);
    printf("         (1: ascending, -1: descending, 0: do not sort,\n"
           "          2: ascending, -2: descending w.r.t. "
                    "transaction size sum)\n");
    printf("-j       use quicksort to sort the transactions   "
                    "(default: heapsort)\n");
    printf("-a#      variant of the fpgrowth algorithm to use "
                    "(default: simple)\n");
    printf("-x       do not prune with perfect extensions     "
                    "(default: prune)\n");
    printf("-z       do not use head union tail (hut) pruning\n");
    printf("         (only for maximal item sets, option -tm)\n");
    printf("-w       integer transaction weight in last field "
                    "(default: only items)\n");
    printf("-r#      record/transaction separators            "
                    "(default: \"\\n\")\n");
    printf("-f#      field /item        separators            "
                    "(default: \" \\t,\")\n");
    printf("-b#      blank   characters                       "
                    "(default: \" \\t\\r\")\n");
    printf("-C#      comment characters                       "
                    "(default: \"#\")\n");
    printf("-!       print additional option information\n");
    printf("infile   file to read transactions from           "
                    "[required]\n");
    printf("outfile  file to write frequent item sets to      "
                    "[optional]\n");
    printf("selfile  file stating a selection of items        "
                    "[optional]\n");
    return 0;                   /* print a usage message */
  }                             /* and abort the program */
  #endif  /* #ifndef QUIET */
  /* free option characters: cilopuwy [A-Z]\[C] */

  /* --- evaluate arguments --- */
  for (i = 1; i < argc; i++) {  /* traverse the arguments */
    s = argv[i];                /* get an option argument */
    if (optarg) { *optarg = s; optarg = NULL; continue; }
    if ((*s == '-') && *++s) {  /* -- if argument is an option */
      while (*s) {              /* traverse the options */
        switch (*s++) {         /* evaluate the options */
          case '!': help();                         break;
          case 't': target = (*s) ? *s++ : 's';     break;
          case 'm': min    = (int)strtol(s, &s, 0); break;
          case 'n': max    = (int)strtol(s, &s, 0); break;
          case 's': supp   =      strtod(s, &s);    break;
          case 'e': eval   = (*s) ? *s++ : 0;       break;
          case 'd': minval =      strtod(s, &s);    break;
          case 'g': report = ISR_SCAN;              break;
          case 'h': optarg = &hdr;                  break;
          case 'k': optarg = &sep;                  break;
          case 'v': optarg = &format;               break;
          case 'q': sort   = (int)strtol(s, &s, 0); break;
          case 'j': heap   = 0;                     break;
          case 'a': algo   = (*s) ? *s++ : 0;       break;
          case 'x': mode  &= ~PR_PERFECT;           break;
          case 'z': mode  &= ~PR_TAIL;              break;
          case 'w': tarm  |= TA_WEIGHT;             break;
          case 'r': optarg = &recseps;              break;
          case 'f': optarg = &fldseps;              break;
          case 'b': optarg = &blanks;               break;
          case 'C': optarg = &comment;              break;
          default : error(E_OPTION, *--s);          break;
        }                       /* set the option variables */
        if (optarg && *s) { *optarg = s; optarg = NULL; break; }
      } }                       /* get an option argument */
    else {                      /* -- if argument is no option */
      switch (k++) {            /* evaluate non-options */
        case  0: fn_inp = s;      break;
        case  1: fn_out = s;      break;
        case  2: fn_sel = s;      break;
        default: error(E_ARGCNT); break;
      }                         /* note filenames */
    }
  }
  if (optarg)     error(E_OPTARG);    /* check option arguments */
  if (k    < 1)   error(E_ARGCNT);    /* and number of arguments */
  if (min  < 0)   error(E_SIZE, min); /* check the size limits */
  if (max  < 0)   error(E_SIZE, max); /* and the minimum support */
  if (supp > 100) error(E_SUPPORT, supp);
  if ((!fn_inp || !*fn_inp) && (fn_sel && !*fn_sel))
    error(E_STDIN);             /* stdin must not be used twice */
  switch (algo) {               /* check and translate alg. variant */
    case 'a': case 0: algo = AV_AUTO;        break;
    case 's':         algo = AV_SIMPLE;      break;
    case 'c':         algo = AV_COMPLEX;     break;
    case 'r':         algo = AV_REORDER;     break;
    case 't':         algo = AV_TOPDOWN;     break;
    case 'd':         algo = AV_SINGLE;      break;
    default : error(E_VARIANT, (char)algo);  break;
  }                             /* (get eclat algorithm code) */
  switch (target) {             /* check and translate target type */
    case 's': target = ISR_ALL;              break;
    case 'c': target = ISR_CLOSED;           break;
    case 'm': target = ISR_MAXIMAL;          break;
    default : error(E_TARGET, (char)target); break;
  }                             /* (get target type code) */
  switch (eval) {               /* check and translate measure */
    case 'x': case 0: eval = EM_NONE;        break;
    case 'b':         eval = EM_LOGQ;        break;
    default : error(E_MEASURE, (char)eval);  break;
  }                             /* (get evaluation measure code) */
  if ((format == dflt) && (supp < 0))
    format = "  (%a)";          /* adapt the default info. format */
  MSG(stderr, "\n");            /* terminate the startup message */

  /* --- read item selection --- */
  ibase = ib_create(0, 0);      /* create an item base */
  if (!ibase) error(E_NOMEM);   /* to manage the items */
  tread = trd_create();         /* create a transaction reader */
  if (!tread) error(E_NOMEM);   /* and configure the characters */
  trd_allchs(tread, recseps, fldseps, blanks, "", comment);
  if (fn_sel) {                 /* if item appearances are given */
    t = clock();                /* start timer, open input file */
    if (trd_open(tread, NULL, fn_sel) != 0)
      error(E_FOPEN, trd_name(tread));
    MSG(stderr, "reading %s ... ", trd_name(tread));
    k = ib_readsel(ibase,tread);/* read the given item selection */
    if (k < 0) error(-k, ib_errmsg(ibase, NULL, 0));
    trd_close(tread);           /* close the input file */
    MSG(stderr, "[%d item(s)] done [%.2fs].\n", k, SEC_SINCE(t));
  }                             /* print a log message */

  /* --- read transaction database --- */
  tabag = tbg_create(ibase);    /* create a transaction bag */
  if (!tabag) error(E_NOMEM);   /* to store the transactions */
  t = clock();                  /* start timer, open input file */
  if (trd_open(tread, NULL, fn_inp) != 0)
    error(E_FOPEN, trd_name(tread));
  MSG(stderr, "reading %s ... ", trd_name(tread));
  k = tbg_read(tabag, tread, tarm);
  if (k < 0)                    /* read the transaction database */
    error(-k, tbg_errmsg(tabag, NULL, 0));
  trd_delete(tread, 1);         /* close the input file and */
  tread = NULL;                 /* delete the table reader */
  n = ib_cnt(ibase);            /* get the number of items, */
  k = tbg_cnt(tabag);           /* the number of transactions, */
  w = tbg_wgt(tabag);           /* the total transaction weight */
  MSG(stderr, "[%d item(s), %d", n, k);
  if (w != k) MSG(stderr, "/%d", w);
  MSG(stderr, " transaction(s)] done [%.2fs].", SEC_SINCE(t));
  if ((n <= 0) || (k <= 0))     /* check for at least one item */
    error(E_NOITEMS);           /* and at least one transaction */
  MSG(stderr, "\n");            /* compute absolute support value */
  supp = ceil((supp >= 0) ? 0.01 *supp *w : -supp);

  /* --- sort and recode items --- */
  t = clock();                  /* start timer, print log message */
  MSG(stderr, "filtering, sorting and recoding items ... ");
  map = (int*)malloc(n *sizeof(int));
  if (!map) error(E_NOMEM);     /* create an item identifier map */
  n = ib_recode(ibase, (int)supp, INT_MAX, -sort, map);
  tbg_recode(tabag, map);       /* recode the items and transactions */
  free(map);                    /* delete the item identifier map */
  MSG(stderr, "[%d item(s)] done [%.2fs].", n, SEC_SINCE(t));
  if (n <= 0) error(E_NOITEMS); /* print a log message and */
  MSG(stderr, "\n");            /* check the number of items */
  i = tbg_max(tabag);           /* restrict the item set size to */
  if (max > i)   max = i;       /* the maximum transaction size */
  if (max < min) max = min;     /* but at least the minimum size */

  /* --- sort and reduce transactions --- */
  t = clock();                  /* start timer, print log message */
  MSG(stderr, "sorting and reducing transactions ... ");
  tbg_filter(tabag, min, NULL); /* remove items of short transactions */
  tbg_itsort(tabag, +1, heap);  /* sort items in transactions and */
  tbg_sort  (tabag, +1, heap);  /* sort the trans. lexicographically */
  k = tbg_reduce(tabag);        /* reduce transactions to unique ones */
  MSG(stderr, "[%d", k); if (w != k) MSG(stderr, "/%d", w);
  MSG(stderr, " transaction(s)] done [%.2fs].\n", SEC_SINCE(t));

  /* --- find frequent item sets --- */
  t = clock();                  /* start the timer */
  report |= (target & (ISR_CLOSED|ISR_MAXIMAL));
  if (eval == EM_LOGQ) report |= ISR_LOGS;
  isrep = isr_create(ibase, report, -1, hdr, sep, NULL);
  if (!isrep) error(E_NOMEM);   /* create an item set reporter */
  isr_setfmt (isrep, format);   /* and configure it: set flags, */
  isr_setsize(isrep, min, max); /* info. format and size range, */
  if (eval == EM_LOGQ)          /* and the evaluation function */
    isr_seteval(isrep, isr_logq, NULL, 0.01*minval);
  if (isr_open(isrep, NULL, fn_out) != 0)
    error(E_FOPEN, isr_name(isrep));  /* open the output file */
  MSG(stderr, "writing %s ... ", isr_name(isrep));
  if (algo == AV_AUTO)          /* choose variant automatically */
    algo = ((k*(double)n > 10.0*tbg_extent(tabag)) || (n > k/4.0))
         ? AV_REORDER : AV_SINGLE;
  if (target & (ISR_CLOSED|ISR_MAXIMAL)) {
    if      (algo == AV_REORDER) algo = AV_COMPLEX;
    else if (algo == AV_SINGLE)  algo = AV_SIMPLE;
  }                             /* not all variants work for c/m yet */
  if      (algo == AV_SINGLE)   /* fpgrowth with single fp-tree */
    k = fpg_single(tabag, mode, (int)supp, isrep);
  else if (algo == AV_TOPDOWN)  /* fpgrowth with top-down processing */
    k = fpg_topdn (tabag, mode, (int)supp, isrep);
  else if (algo == AV_REORDER)  /* fpgrowth with item reordering */
    k = fpg_reord (tabag, mode, (int)supp, isrep);
  else if (algo == AV_COMPLEX)  /* fpgrowth with complex nodes */
    k = fpg_cmplx (tabag, mode, (int)supp, isrep);
  else                          /* fpgrowth with simple nodes */
    k = fpg_simple(tabag, mode, (int)supp, isrep);
  if (k < 0) error(E_NOMEM);    /* (search for frequent item sets) */
  if (isr_close(isrep) != 0)    /* close the output file */
    error(E_FWRITE, isr_name(isrep));
  MSG(stderr, "[%d set(s)]", isr_repcnt(isrep));
  MSG(stderr, " done [%.2fs].\n", SEC_SINCE(t));

  /* --- clean up --- */
  CLEANUP;                      /* clean up memory and close files */
  SHOWMEM;                      /* show (final) memory usage */
  return 0;                     /* return 'ok' */
}  /* main() */
