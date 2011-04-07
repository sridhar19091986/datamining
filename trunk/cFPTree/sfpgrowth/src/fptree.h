/*----------------------------------------------------------------------
  File    : fptree.h
  Contents: frequent pattern tree management
  Author  : Christian Borgelt
  History : 2004.11.21 file created
            2004.11.22 second projection method added
            2004.12.09 definition of report function changed
            2004.12.10 adapted to general memory management system
            2006.11.26 prefix added to parameters of report function
            2008.01.23 root node added (for corrected bonsai pruning)

	    Changes by Li Ma
	    2008.06.02 FPTNODE, FPTLIST modified
----------------------------------------------------------------------*/
#ifndef __FPTREE__
#define __FPTREE__
#include "memsys.h"
#include "tract.h"

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define FPT_ALTPROJ   0x01      /* use alternative projection */
#define FPT_BONSAI    0x02      /* prune projection to bonsai */





/*----------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------*/
typedef struct _fptnode {       /* --- frequent pattern tree node --- */
  struct _fptnode *succ;        /* pointer to successor (same item) */
  struct _fptnode *parent;      /* pointer to parent node */
  struct _fptnode *copy;        /* auxiliary pointer to copy */
  int             item;         /* corresponding item */
  int             cnt;          /* number of transactions */
  int             cnt_cases;      /** number of transactions among cases **/
} FPTNODE;                      /* (frequent pattern tree node) */

typedef struct {                /* --- freq. pat. tree node list --- */
  int     cnt;                  /* number of transactions */
  int     cnt_among_cases;      /** count among cases **/
  FPTNODE *node;                /* pointer to first node in list */
} FPTLIST;                      /* (frequent pattern tree node list) */

typedef struct {                /* --- frequent pattern tree --- */
  ITEMSET *itemset;             /* underlying item set */
  int     cnt;                  /* number of items / node lists */
  int     tra;                  /* number of transactions */
  MEMSYS  *mem;                 /* memory management system */
  FPTNODE root;                 /* root node connecting trees */
  FPTLIST lists[1];             /* vector of one node list per item */
} FPTREE;                       /* (frequent pattern tree) */

typedef int FPTREPFN (int *ids,int cnt,int prefix,int supp,int supp_among_cases, void *data);
                                /** report function **/

/*----------------------------------------------------------------------
  Main Functions
----------------------------------------------------------------------*/
extern FPTREE* fpt_create  (TASET *taset);
extern void    fpt_delete  (FPTREE *fpt);
extern void    fpt_itemset (FPTREE *fpt);

extern int     fpt_search  (FPTREE *fpt, int supp, int min, int max,
                            int mode, FPTREPFN report, void *data);
#ifndef NDEBUG
extern void    fpt_show    (FPTREE *fpt, const char *title);
#endif

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define fpt_itemset(t)     ((t)->itemset)

#endif
