/*----------------------------------------------------------------------
  File    : clomax.h
  Contents: prefix tree management for closed and maximal item sets
  Author  : Christian Borgelt
  History : 2009.10.08 file created as pfxtree.c
            2009.10.26 function cmt_dir() added (get item order dir.)
            2010.02.10 function cmt_proj() added (project a c/m tree)
            2010.03.12 function cmt_xproj() added (project a c/m tree)
            2010.06.21 generalized by introducing definition of SUPP_T
            2010.07.09 function cmt_check() added (with given support)
            2010.07.22 closed/maximal item set filter functions added
----------------------------------------------------------------------*/
#ifndef __CLOMAX__
#define __CLOMAX__
#include "memsys.h"
#ifndef NDEBUG
#include "tract.h"
#endif
#ifndef SUPP_T
#define SUPP_T   int
#endif

/*----------------------------------------------------------------------
  Type Definitions
----------------------------------------------------------------------*/
typedef struct _cmnode {        /* --- c/m prefix tree node --- */
  SUPP_T         supp;          /* support of represented item set */
  int            item;          /* associated item (last item in set) */
  struct _cmnode *sibling;      /* successor node in sibling list */
  struct _cmnode *children;     /* list of child nodes */
} CMNODE;                       /* (c/m prefix tree node) */

typedef struct {                /* --- c/m prefix tree --- */
  MEMSYS *mem;                  /* memory management system */
  int    dir;                   /* direction of item order */
  int    size;                  /* (maximum) number of items */
  int    item;                  /* associated prefix item */
  int    max;                   /* maximum support for prefix */
  CMNODE root;                  /* root node of the tree */
  int    keep[1];               /* flags for cmt_xproj() calls */
} CMTREE;                       /* (c/m prefix tree) */

typedef struct {                /* --- closed/maximal filter --- */
  MEMSYS *mem;                  /* memory management system */
  int    dir;                   /* direction of item order */
  int    size;                  /* maximum number of prefix trees */
  int    cnt;                   /* current number of prefix trees */
  CMTREE *trees[1];             /* conditional prefix trees */
} CLOMAX;                       /* (closed/maximal filter) */

/*----------------------------------------------------------------------
  Prefix Tree Functions
----------------------------------------------------------------------*/
CMTREE* cmt_create (MEMSYS *mem, int dir, int size);
void    cmt_clear  (CMTREE *cmt);
void    cmt_delete (CMTREE *cmt, int delms);
MEMSYS* cmt_memsys (CMTREE *cmt);
int     cmt_cnt    (CMTREE *cmt);
int     cmt_dir    (CMTREE *cmt);
SUPP_T  cmt_supp   (CMTREE *cmt);
SUPP_T  cmt_max    (CMTREE *cmt);
int     cmt_valid  (CMTREE *cmt);

int     cmt_add    (CMTREE *cmt, const int *items, int n, SUPP_T supp);
SUPP_T  cmt_get    (CMTREE *cmt, const int *items, int n);
int     cmt_check  (CMTREE *cmt, const int *items, int n, SUPP_T supp);
void    cmt_prune  (CMTREE *cmt, int item);
CMTREE* cmt_proj   (CMTREE *dst, CMTREE *src, int item);
CMTREE* cmt_xproj  (CMTREE *dst, CMTREE *src, int item,
                    const int *keep, int n);

#ifndef NDEBUG
void    cmt_show   (CMTREE *cmt, ITEMBASE *base, int ind);
#endif

/*----------------------------------------------------------------------
  Closed/Maximal Filter Functions
----------------------------------------------------------------------*/
CLOMAX* cm_create  (int dir, int size);
void    cm_delete  (CLOMAX *cm);
int     cm_cnt     (CLOMAX *cm);
int     cm_dir     (CLOMAX *cm);
int     cm_supp    (CLOMAX *cm);
int     cm_tree    (CLOMAX *cm, int i);

int     cm_add     (CLOMAX *cm, int item, SUPP_T supp);
void    cm_remove  (CLOMAX *cm, int n);
int     cm_tail    (CLOMAX *cm, const int *items, int n);
int     cm_update  (CLOMAX *cm, const int *items, int n, SUPP_T supp);

#ifndef NDEBUG
void    cm_show    (CLOMAX *cm, ITEMBASE *base, int ind);
#endif

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define cmt_memsys(t)    ((t)->mem)
#define cmt_cnt(t)       ((t)->cnt)
#define cmt_dir(t)       ((t)->dir)
#define cmt_supp(t)      ((t)->root.supp)
#define cmt_max(t)       ((t)->max)
#define cmt_valid(t)     ((t)->max >= 0)

#define cm_cnt(f)        ((f)->cnt)
#define cm_dir(f)        ((f)->dir)
#define cm_tree(f,i)     ((f)->trees[i])

#endif
