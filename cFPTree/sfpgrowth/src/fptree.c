/*----------------------------------------------------------------------
  File    : fptree.c
  Contents: frequent pattern tree management
  Author  : Christian Borgelt
  History : 2004.11.21 file created
            2004.11.22 second projection added, bonsai pruning added
            2004.12.09 adapted to changed report function
            2004.12.10 adapted to general memory management system
            2004.12.28 bug in function fpt_delete fixed
            2005.12.06 bug in function _proj_level fixed
            2006.11.26 reuse of item set prefix made possible
            2008.01.22 special processing of chains added
            2008.01.23 bonsai pruning corrected (node merging)
	    
	    Changes by Li Ma
	    2008.06.02 _proj_path, _proj_level, _bonsai, fpt_create, _build
	    modified
	    2008.06.05 further updates to separartely generate frequent
	    among cases and among controls
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "fptree.h"
#ifdef STORAGE
#include "storage.h"
#endif

/*----------------------------------------------------------------------
  Preprocessor Definition
----------------------------------------------------------------------*/
#define BLKSIZE    6553         /* block size for memory management */

/*----------------------------------------------------------------------
  Type Definition
----------------------------------------------------------------------*/
typedef FPTREE* PROJFN (FPTREE *fpt, int item);

typedef struct {                /* --- structure for recursive search */
  int      min;                 /* minimum number of items */
  int      max;                 /* maximum number of items */
  int      supp;                /* minimum support (num. of trans.) */
  int      bonsai;              /* flag for pruning to bonsai */
  PROJFN   *proj;               /* projection function */
  FPTREPFN *report;             /* report function for results */
  void     *data;               /* user data for report function */
  int      cnt;                 /* number of frequent item sets */
  int      items[1];            /* item vector for reporting */
} FPRS;                         /* (structure for rec. search) */

/*----------------------------------------------------------------------
  Main Functions
----------------------------------------------------------------------*/

static FPTREE* _create (MEMSYS *mem, int cnt)
{                               /* --- create a base freq. pat. tree */
  FPTREE  *fpt;                 /* created frequent pattern tree */
  FPTLIST *list;                /* to traverse the node lists */

  assert(cnt > 0);              /* check the function arguments */
  fpt = (FPTREE*)malloc(sizeof(FPTREE) +(cnt-1) *sizeof(FPTLIST));
  if (!fpt) return NULL;        /* allocate the base structure */
  fpt->cnt = cnt;               /* and note the number of items */
  fpt->root.item = -1;          /* initialize the root node */
  fpt->root.cnt  =  0;          /* (dummy connecting the trees) */
  fpt->root.copy = fpt->root.parent = NULL;
  
  fpt->root.cnt_cases=0; /** initialize the counts among cases to 0 **/

  if (mem) fpt->mem = mem;      /* if a memory management is given, */
  else {                        /* simply store it, otherwise */
    fpt->mem = ms_create(sizeof(FPTNODE), BLKSIZE);
    if (!fpt->mem) { free(fpt); return NULL; }
  }                             /* create a memory management system */
  for (list = fpt->lists +cnt; --cnt >= 0; ) {
    (--list)->cnt = 0; list->node = NULL;
    list->cnt_among_cases = 0;    /** initialize count among cases**/
    
  }                             /* initialize the node list and */
  return fpt;                   /* return the created freq. pat. tree */
}  /* _create() */

/*--------------------------------------------------------------------*/

static int _build (FPTREE *fpt, FPTNODE *parent,
                   TASET *taset, int lft, int rgt, int pos)
{                               /* --- recursively build f.p. tree */
  int     i, k, j;                 /* loop variable, buffer */
  int     item;                 /* to traverse the items at pos */
  FPTLIST *list;                /* to access the node lists */
  FPTNODE *node;                /* created freq. pattern tree node */

  int     cnt_among_cases;    /** the number of transections among cases for this node **/
  /*extern int *perm_label; */

  assert(fpt && taset && (pos >= 0)); /* check the function arguments */
  while ((lft <= rgt) && (tas_tsize(taset, lft) <= pos))
    lft++;                      /* skip trans. that are too short */
  if (lft > rgt) return 0;      /* check for an empty item range */
  item = k = tas_tract(taset, i = rgt)[pos];   /* get first item */
  do {                          /* traverse the longer transactions */
    while (--i >= lft) {        /* while not at start of section */
      k = tas_tract(taset, i)[pos];
      
      if (k != item) break;     /* try to find a transaction */
    }                           /* with a different item */
    node = ms_alloc(fpt->mem);  /* create a new tree node */
    if (!node) return -1;       /* for the current item and */
    list = fpt->lists +item;    /* get the corresponding node list */
    node->item   = item;        /* store the item and its support */   
    
    list->cnt   += node->cnt = rgt -i;
    
    cnt_among_cases = 0;
    for (j = i+1; j <= rgt; j++)
      cnt_among_cases += tas_tract_label(taset,j);
      /** cnt_among_cases += perm_label[j]; **/
    /** perm_label is a vector of control/case (0/1) labels randomly 
	generated for this permutation **/
    list->cnt_among_cases+=
      node->cnt_cases = cnt_among_cases; /** store its support among cases **/


    node->succ   = list->node;  /* insert the node into the list */
    list->node   = node;        /* (add at head of list) */
    node->parent = parent;      /* init. the parent pointer */
    node->copy   = NULL;        /* clear the copy pointer */
    if (_build(fpt, node, taset, i+1, rgt, pos+1) != 0)
      return -1;                /* build the child node recursively */
    item = k; rgt = i;          /* remove processed transaction from */
  } while (lft <= rgt);         /* the interval and note next item */
  return 0;                     /* return 'ok' */
}  /* _build() */

/*--------------------------------------------------------------------*/

FPTREE* fpt_create (TASET *taset)
{                               /* --- create a freq. pattern tree */
  FPTREE *fpt;                  /* created frequent pattern tree */
  /** int i; **/                        /** loop variable **/
  /**  extern int *perm_label; **/

  assert(taset);                /* check the function argument */
  fpt = _create(NULL, is_cnt(tas_itemset(taset)));
  if (!fpt) return NULL;        /* allocate a base f.p. tree */
  fpt->itemset  = tas_itemset(taset);
  fpt->root.cnt = tas_cnt(taset);

  fpt->root.cnt_cases = tas_cnt_cases(taset); /** total number of cases **/

  /** for (i = 0; i <= fpt->root.cnt-1; i++)
      fpt->root.cnt_cases += tas_tract_label(taset,i); **/
  /**  fpt->root.cnt_cases += perm_label[i]; **/   

  if ((fpt->root.cnt > 0)       /* recursively build the f.p. tree */
  &&  (_build(fpt, &fpt->root, taset, 0, fpt->root.cnt-1, 0) != 0)) {
    fpt_delete(fpt); return NULL; }
  return fpt;                   /* return the created tree */
}  /* fpt_create() */

/*----------------------------------------------------------------------
The above function assumes that the items in each transaction in taset
are sorted and that the transactions are sorted accordingly.
----------------------------------------------------------------------*/

void fpt_delete (FPTREE *fpt)
{                               /* --- delete a freq. pattern tree */
  assert(fpt);                  /* check the function argument */
  ms_delete(fpt->mem);          /* delete the memory system */
  free(fpt);                    /* and the base structure */
}  /* fpt_delete() */

/*--------------------------------------------------------------------*/

static void _prune (FPTREE *fpt)
{                               /* --- prune nodes for last item */
  FPTNODE *node, *buf;          /* to traverse the nodes to delete */

  assert(fpt);                  /* check the function argument */
  for (node = fpt->lists[--fpt->cnt].node; node; ) {
    buf  = node;                /* while there is another node */
    node = node->succ;          /* note the current node and */
    ms_free(fpt->mem, buf);     /* remove it from the list, */
  }                             /* and then delete the node */
}  /* _prune() */

/*--------------------------------------------------------------------*/

static void _detach (FPTREE *fpt)
{                               /* --- detach a projection */
  FPTNODE *node, *anc;          /* to traverse the ancestors */

  assert(fpt);                  /* check the function argument */
  fpt->root.copy = NULL;        /* clear the root's copy pointer */
  node = fpt->lists[--fpt->cnt].node;
  while (node) {                /* while there is another node */
    for (anc = node->parent; anc && anc->copy; anc = anc->parent)
      anc->copy = NULL;         /* clear the copy pointers */
    anc  = node;                /* note the current node and */
    node = node->succ;          /* remove it from the list, */
    ms_free(fpt->mem, anc);     /* then delete the node */
  }                             /* (prune deepest tree level) */
}  /* _detach() */

/*--------------------------------------------------------------------*/

static void _cleanup (FPTREE *fpt, FPTREE *proj)
{                               /* --- clean up after failure */
  assert(fpt && proj);          /* check the function argument */
  _detach(fpt);                 /* detach projection from tree */
  while (proj->cnt > 0) _prune(proj);
  free(proj);                   /* delete the projection */
}  /* _cleanup() */             /* (only called on failure) */

/*--------------------------------------------------------------------*/

static FPTREE* _proj_path (FPTREE *fpt, int item)
{                               /* --- project a freq. pattern tree */
  int     i;                    /* loop variable */
  FPTREE  *proj;                /* projected frequent pattern tree */
  FPTNODE *node, *anc, *copy;   /* to traverse the tree nodes */
  FPTNODE **prev;               /* to link copies to their parents */
  FPTLIST *lists;               /* to access the node lists */

  assert(fpt);                  /* check the function argument */
  proj = _create(fpt->mem, item);
  if (!proj) return NULL;       /* create a base freq. pattern tree */
  proj->itemset = fpt->itemset; /* note the underlying item set */
  fpt->root.copy = &proj->root; /* set the copy of the root node */
  lists = proj->lists;          /* get the node lists of the proj. */
  for (node = fpt->lists[item].node; node; node = node->succ) {
    prev = NULL;                /* traverse the nodes for the item */
    for (anc = node->parent; !anc->copy; anc = anc->parent) {
      anc->copy =               /* traverse and copy all ancestors */
      copy = ms_alloc(fpt->mem);        /* that are not yet copied */
      if (!copy) { _cleanup(fpt, proj); return NULL; }
      copy->item = i = anc->item;      /* store/copy the item */
      if (prev) *prev = copy;   /* set parent link from child */
      copy->succ = lists[i].node;
      lists[i].node = copy;     /* insert copy into corresp. list */
      lists[i].cnt += copy->cnt = node->cnt;

      lists[i].cnt_among_cases += 
	copy->cnt_cases = node->cnt_cases;
      /** update the counts among cases **/

      copy->copy = NULL;        /* copy the support of the node */
      prev = &copy->parent;     /* and note the parent pointer */
    }                           /* address for later linking */
    if (prev) *prev = anc->copy;/* set last parent link from child */
    for ( ; anc; anc = anc->parent) {
      anc->copy->cnt += node->cnt;
      anc->copy->cnt_cases += node->cnt_cases; /** **/

      if (anc->item >= 0) {
	lists[anc->item].cnt += node->cnt;
	lists[anc->item].cnt_among_cases += node->cnt_cases; /** **/
      }
    }                           /* traverse ancestors up to the root */
  }                             /* and sum the support values */
  _detach(fpt);                 /* detach created tree projection, */
  return proj;                  /* prune last level of orig. tree, */
}  /* _proj_path() */           /* and return created projection */

/*--------------------------------------------------------------------*/

static FPTREE* _proj_level (FPTREE *fpt, int item)
{                               /* --- project a freq. pattern tree */
  int     i, k;                 /* loop variables */
  FPTREE  *proj;                /* projected frequent pattern tree */
  FPTNODE *node, *par, *copy;   /* to traverse the tree nodes */
  FPTLIST *lists;               /* to access the node lists */

  assert(fpt);                  /* check the function argument */
  proj = _create(fpt->mem, item);
  if (!proj) return NULL;       /* create a base freq. pattern tree */
  proj->itemset = fpt->itemset; /* note the underlying item set */
  fpt->root.copy = &proj->root; /* set the copy of the root node */
  lists = proj->lists;          /* get the node lists of the proj. */
  node = fpt->lists[item].node; /* first traverse the old leaves */
  for (k = item; k >= 0; ) {    /* traverse the tree levels */
    for ( ; node; node = node->succ) {
      par  = node->parent;      /* get each node's parent and */
      copy = par->copy;         /* the copy of this parent */
      if (copy) {               /* if a copy already exists */
        /* Note that if k == item, the only case in which */
        /* copy != NULL is when copy == &proj->root.      */
        if (k < item) node->parent = copy;
        copy->cnt += node->cnt; /* sum the number of transactions */
	copy->cnt_cases += node->cnt_cases; /** **/
        if (copy->item >= 0) {
	  lists[copy->item].cnt += node->cnt;
	  lists[copy->item].cnt_among_cases += node->cnt_cases; /** **/
	}
        continue;               /* update the item frequency */
      }                         /* otherwise create a copy */
      par->copy = copy = ms_alloc(fpt->mem);
      if (!copy) { _cleanup(fpt, proj); return NULL; }
      if (k < item) node->parent = copy;
      copy->item = i = par->item;  /* copy the item of the node and */
      copy->succ = lists[i].node;  /* insert copy into corresp. list */
      lists[i].node = copy;        /* in the projected tree */
      lists[i].cnt += copy->cnt = node->cnt;

      lists[i].cnt_among_cases += copy->cnt_cases = node->cnt_cases; /** **/

      copy->parent  = par->parent;
      copy->copy    = NULL;     /* set the support of the node */
    }                           /* and the parent pointer */
    node = lists[--k].node;     /* go to the created node list */
  }
  _detach(fpt);                 /* detach created tree projection, */
  return proj;                  /* prune last level of orig. tree, */
}  /* _proj_level() */          /* and return created projection */

/*----------------------------------------------------------------------
Of the above two projection methods, method _proj_path generally seems
to yield better results, which is why it is the default.
----------------------------------------------------------------------*/

static void _bonsai (FPTREE *fpt, int supp) /** modified to prune based on support among cases **/
{                               /* --- prune projection to bonsai */
  int     i, k;                 /* loop variables, buffer */
  FPTLIST *lists;               /* to traverse the level lists */
  FPTNODE *node, *anc;          /* to traverse the tree nodes */

  lists = fpt->lists;           /* buffer the tree level vector */
  for (i = fpt->cnt; (--i >= 0) && (lists[i].cnt_among_cases < supp); ) /** **/
    _prune(fpt);                /* prune deepest tree levels */
  for (k = 0; k < fpt->cnt; k++)/* traverse the levels downwards */
    if ((lists[k].cnt_among_cases < supp) && (lists[k].cnt_among_cases > 0)) /** **/
      break;                    /* find first infrequent item */
  for (i = k; ++i < fpt->cnt;){ /* traverse the deeper tree levels */
    if (lists[i].cnt_among_cases < supp)    /** skip tree levels that refer **/
      continue;                 /** to an infrequent item among the cases**/
    for (node = lists[i].node; node; node = node->succ) {
      anc = node->parent;       /* traverse the node list */
      while ((anc->item >= 0) && (lists[anc->item].cnt_among_cases < supp))
        anc = anc->parent;      /** find frequent ancestor or root **/
      if (anc->copy && (anc->copy->item == anc->item))
        anc = anc->copy;        /* if ancestor is merged, get dest. */
      if (!anc->copy            /* if ancestor has not been visited */
      ||  (anc->copy->item != i)){ /* or child is on another level, */
        anc->copy    = node;    /* note the current node as a child */
        node->parent = anc; }   /* and set ancestor as new parent */
      else {                    /* if child on this level is known */
        node->copy = anc->copy; node->copy->cnt += node->cnt; 
	node->copy->cnt_cases += node->cnt_cases; /** **/
      }
    }                           /* merge current node to this child */
  }
  for ( ; k < fpt->cnt; k++) {  /* traverse the deeper tree levels */
    node = lists[k].node;       /* get the node list of the level */
    if (!node) continue;        /* skip empty tree levels */
    if (lists[k].cnt_among_cases < supp) {  /** if a level is to be deleted, **/
      do {                      /* delete the node list: */
        anc  = node;            /* note the current node, */
        node = node->succ;      /* go to the successor node, */
        ms_free(fpt->mem, anc); /* and delete the current node */
      } while (node);           /* while the list is not empty */
      lists[k].cnt = 0; lists[k].node = NULL;
      lists[k].cnt_among_cases = 0; /** **/
      continue;                 /* clear the counter and the list */
    }
    while (node->succ) {        /* while not at last list element */
      if (!node->succ->copy || (node->succ->copy->item != k)) {
        node->parent->copy = node->copy = NULL;
        node = node->succ;      /* if the successor was not merged, */
        continue;               /* clear possible child pointers */
      }                         /* and keep the successor node */
      anc = node->succ;         /* if the successor was merged, */
      node->succ = anc->succ;   /* note the successor node */
      ms_free(fpt->mem, anc);   /* and remove it from the list, */
    }                           /* then delete the node */
    node->parent->copy = node->copy = NULL;
  }                             /* clear possible child pointers */
}  /* _bonsai() */

/*----------------------------------------------------------------------
For clarity and memory efficiency the above function already removes the
pruned tree levels. This is not really necessary as it would otherwise
be done in the search function, once the level to be removed becomes the
deepest in the pruned tree.
----------------------------------------------------------------------*/

static void _chain (FPTNODE *node, int d, int pfx, int max, int max_cases, FPRS *rs)
{                               /** --- process an item chain **/
  int k, n;                     /* output flag, buffer */
  int n_cases;                  /** buffer for counts among cases **/
 
  d++;                          /* increment the recursion depth */
  while (1) {                   /* traverse the item list */
    n = (node->cnt < max) ? node->cnt : max;
    n_cases = (node->cnt_cases < max_cases) ? node->cnt_cases : max_cases;  /** **/ 
    /** n_cases = node->cnt_cases; **/ /** **/

    k = 0;                      /* get support and clear output flag */
    rs->items[d-1] = node->item;/* store the next item */
    if (d >= rs->min) {         /* if the item set is large enough */
      k = rs->report(rs->items, d, pfx, n, n_cases, rs->data); 
      if (k) { rs->cnt++; pfx = d-1; }
    }                           /* report the frequent item set */
    node = node->parent;        /* remove the processed head item */
    if (node->item < 0) return; /* check for an empty tail list */
    if (d < rs->max) _chain(node, d, pfx +k, n, n_cases, rs);
  }                             /* process the tail with head item */
}  /* _chain() */               /* and without (in the next loop) */

/*--------------------------------------------------------------------*/

static int _search (FPTREE *fpt, int d, int pfx, FPRS *rs)
{                               /* --- rec. search frequent patterns */
  int     i, k, n;              /* loop variables, buffer */
  int     *refs;                /* counters for node references */
  FPTREE  *proj;                /* projected frequent pattern tree */
  FPTLIST *list;                /* node list for current item */
  FPTNODE *node, *tmp;          /* to traverse the tree nodes */

  assert(fpt);                  /* check the function argument */

  /* --- find the chain section --- */
  refs = rs->items +d;          /* get the reference counters */
  for (n = 0; n < fpt->cnt; n++) {
    node = fpt->lists[n].node;  /* traverse the items and their lists */
    if (!node) continue;        /* skip empty lists */
    if (node->succ) break;      /* list must have at most one elem. */
    i = node->parent->item;     /* check references to the parent */
    if ((i >= 0) && (++refs[i] > 1)) break;
    refs[n] = 0;                /* there must be no more than one */
  }                             /* parent reference to each node */

  /* --- process the tree section --- */
  d++;                          /* increment the recursion depth */
  for (i = fpt->cnt; --i >= n; ) {
    list = fpt->lists +i;       /* traverse the items and their lists */
    if (list->cnt_among_cases < rs->supp) { /** skip infrequent items **/
      _prune(fpt); continue; }  /* (just prune their node list) */
    rs->items[d-1] = i; k = 0;  /* store the last item of the set */
    if (d >= rs->min) {         /* if the item set is large enough */
      k = rs->report(rs->items, d, pfx, list->cnt, list->cnt_among_cases, rs->data); /** **/
      if (k) { rs->cnt++; pfx = d-1; }
    }                           /* report the frequent item set */
    if ((d >= rs->max)          /* if maximal size has been reached */
    ||  (i <= 0)) {             /* or at first (and thus only) item, */
      _prune(fpt); continue; }  /* skip the recursion */
    proj = rs->proj(fpt, i);    /* project the freq. pattern tree, */
    if (rs->bonsai)             /* if the bonsai flag is set, */
      _bonsai(proj, rs->supp);  /* prune the projection to a bonsai */
    k = _search(proj, d, pfx +k, rs);
    free(proj);                 /* recursively search the projection */
    if (k) return -1;           /* if an error occurred, abort */
  }                             /* (after this only chains remain) */

  /* --- process remaining chains --- */
  --d;                          /* decrement the recursion depth */
  while (--fpt->cnt >= 0) {     /* traverse the items in chains */
    node = fpt->lists[fpt->cnt].node;
    if (!node) continue;        /* skip empty node lists */
    if (!rs->bonsai) {          /* if not pruned to bonsai */
      if (node->cnt_cases < rs->supp){/** remove infrequent items **/
        ms_free(fpt->mem, node); continue; }
      do {                      /* traverse the chain */
        for (tmp = node->parent; tmp->cnt_cases < rs->supp; )
          tmp = tmp->parent;    /** remove infrequent items **/
        node = node->parent = tmp;
      } while (node->item >= 0);/* keep frequent items */
      node = fpt->lists[fpt->cnt].node;
    }                           /* get the chain start again */
    _chain(node, d, pfx, fpt->root.cnt, fpt->root.cnt_cases, rs); /** **/
    do {                        /* process an item chain */
      fpt->lists[node->item].node = NULL;
      tmp  = node;              /* note the current node */
      node = node->parent;      /* and go to its parent */
      ms_free(fpt->mem, tmp);   /* delete the buffered node */
    } while (node->item >= 0);  /* while not at the end of the chain */
  }
  return 0;                     /* return 'ok' */
}  /* _search() */

/*--------------------------------------------------------------------*/

int fpt_search (FPTREE *fpt, int supp, int min, int max, int mode,
                FPTREPFN report, void *data)
{                               /* --- search frequent patterns */
  int  n;                       /* number of frequent item sets */
  FPRS *rs;                     /* structure for recursive search */

  assert(fpt                    /* check the function arguments */
	 && (supp > 0) && (max >= min) && (min >= 0) && report);
  rs = (FPRS*)malloc(sizeof(FPRS) +(fpt->cnt -1) *sizeof(int));
  if (!rs) return -1;           /* create recursive search structure */
  rs->supp   = (supp > 0) ? supp : 1;
  rs->min    = min;             /* initialize the fields */
  rs->max    = max;             /* with the given parameters */
  rs->bonsai = (mode & FPT_BONSAI)  ? -1 : 0;
  rs->proj   = (mode & FPT_ALTPROJ) ? _proj_level : _proj_path;
  rs->report = report;
  rs->data   = data;
  rs->cnt    = 0;               /* initialize the item set counter */
  if (_search(fpt, 0, 0, rs) != 0)
    return -1;                  /* search the tree recursively */
  n = rs->cnt; free(rs);        /* delete the search structure and */
  return n;                     /* return number of freq. item sets */
}  /* fpt_search() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

void fpt_show (FPTREE *fpt, const char *title)
{                               /* --- show a freq. pattern tree */
  int     i;                    /* loop variable */
  FPTNODE *node;                /* to traverse the node lists */

  printf("\n%s\n", title);      /* leave one line empty */
  for (i = 0; i < fpt->cnt; i++) {         /* traverse the items */
    printf("%s ", is_name(fpt->itemset, i));  /* print the item */
    printf("(%d):", fpt->lists[i].cnt);       /* and its support */
    for (node = fpt->lists[i].node; node; node = node->succ) {
      printf(" %d", node->cnt); /* print the node support */
      if (node->parent)         /* if the parent exists */
        printf("[%s]", is_name(fpt->itemset, node->parent->item));
    }                           /* print the item in the parent */
    printf("\n");               /* terminate the line */
  }                             /* after each node list */
}  /* fpt_show() */

#endif
