/*----------------------------------------------------------------------
  File    : clomax.c
  Contents: prefix tree management for closed and maximal item sets
  Author  : Christian Borgelt
  History : 2009.10.08 file created
            2009.10.09 item order direction added
            2009.10.31 merge functions improved (for empty arguments)
            2009.11.12 root node made fixed element in tree structure
            2010.02.10 function cmt_proj() added (project a c/m tree)
            2010.03.11 function cmt_add() improved (create new nodes)
            2010.03.12 function cmt_xproj() added (project a c/m tree)
            2010.06.21 generalized by introducing definition of SUPP_T
            2010.07.09 function cmt_check() added (with given support)
            2010.07.21 early pruning of projection items added
            2010.07.22 closed/maximal item set filter functions added
            2010.07.29 functions _prune_pos/_neg() simplified
            2010.12.07 added some explicit type casts (for C++)
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "arrays.h"
#include "clomax.h"

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define pos(x,y)   ((x) < (y))  /* macros for item comparison */
#define neg(x,y)   ((x) > (y))  /* (ascending and descending) */

#define COPYERR   ((CMNODE*)-1) /* error indicator for _xcopy */

/*----------------------------------------------------------------------
  Prefix Tree Functions
----------------------------------------------------------------------*/

CMTREE* cmt_create (MEMSYS *mem, int dir, int size)
{                               /* --- create a c/m prefix tree */
  CMTREE *cmt;                  /* created c/m prefix tree */

  assert(size > 0);             /* check the function arguments */
  cmt = (CMTREE*)calloc(1, sizeof(CMTREE) +(size-1) *sizeof(int));
  if (!cmt) return NULL;        /* create a c/m prefix tree */
  cmt->size = size;             /* and initialize its fields */
  cmt->dir  = (dir < 0) ? -1 : 1;
  cmt->item = cmt->max  = -1;  /* mark the tree as empty/invalid */
  cmt->mem  = (mem) ? mem : ms_create(sizeof(CMNODE), 65535);
  if (!cmt->mem) { free(cmt); return NULL; }    /* block size 1MB */
  cmt->root.sibling = cmt->root.children = NULL;
  cmt->root.item    = -1;       /* initialize the root node */
  cmt->root.supp    =  0;       /* (no item, no other nodes) */
  return cmt;                   /* return the created prefix tree */
}  /* cmt_create() */

/*--------------------------------------------------------------------*/

static void _delete (CMNODE *node, MEMSYS *mem)
{                               /* --- recursively delete nodes */
  CMNODE *p;                    /* buffer for deallocation */

  assert(mem);                  /* check the function arguments */
  while (node) {                /* sibling list deletion loop */
    _delete(node->children,mem);/* recursively delete the children */
    p = node; node = node->sibling; ms_free(mem, p);            
  }                             /* finally delete the node itself */
}  /* _delete() */

/*--------------------------------------------------------------------*/

void cmt_clear (CMTREE *cmt)
{                               /* --- clear a c/m prefix tree */
  assert(cmt);                  /* check the function argument */
  _delete(cmt->root.children, cmt->mem);
  cmt->root.children = NULL;    /* delete all nodes and */
  cmt->root.supp     =  0;      /* reinitialize the root node */
  cmt->max           = -1;      /* prefix tree is now empty/invalid */
}  /* cmt_clear() */

/*--------------------------------------------------------------------*/

void cmt_delete (CMTREE *cmt, int delms)
{                               /* --- delete a c/m prefix tree */
  assert(cmt);                  /* check the function arguments */
  if      (delms >  0) ms_delete(cmt->mem);
  else if (delms >= 0) _delete(cmt->root.children, cmt->mem);
  free(cmt);                    /* delete memory system or nodes */
}  /* cmt_delete() */           /* and delete the base structure */

/*--------------------------------------------------------------------*/

int cmt_add (CMTREE *cmt, const int *items, int n, SUPP_T supp)
{                               /* --- add an item set to a c/m tree */
  int    i;                     /* buffer for an item */
  CMNODE **p;                   /* pointer to insertion position */
  CMNODE *node;                 /* to insert new nodes */

  assert(cmt                    /* check the function arguments */
  &&    (items || (n <= 0)) && (supp >= 0));
  if (supp > cmt->max)          /* update maximum item set support */
    cmt->max = supp;            /* (prefix tree is now valid) */
  node = &cmt->root;            /* start at the root node */
  if (supp > node->supp)        /* adapt the empty set support */
    node->supp = supp;          /* (root represents empty set) */
  while (1) {                   /* traverse the items of the set */
    if (--n < 0) return 0;      /* if all items are processed, abort */
    i = *items++;               /* get the next item in the set and */
    p = &node->children;        /* traverse the list of children */
    if (cmt->dir < 0) while (*p && ((*p)->item > i)) p = &(*p)->sibling;
    else              while (*p && ((*p)->item < i)) p = &(*p)->sibling;
    node = *p;                  /* find the item/insertion position */
    if (!node || (node->item != i))
      break;                    /* if the item does not exist, abort */
    if (supp > node->supp) node->supp = supp;
  }                             /* update the item set support */
  node = (CMNODE*)ms_alloc(cmt->mem);
  if (!node) return -1;         /* create a new prefix tree node */
  node->item    = i;            /* store the current item and */
  node->supp    = supp;         /* the support of the item set */
  node->sibling = *p;           /* insert the created node */
  *p = node;                    /* into the sibling list */
  while (--n >= 0) {            /* traverse the rest of the items */
    node = node->children = (CMNODE*)ms_alloc(cmt->mem);
    if (!node) return -1;       /* create a new prefix tree node */
    node->item    = *items++;   /* store the current item and */
    node->supp    = supp;       /* the support of the item set */
    node->sibling = NULL;       /* there are no siblings yet */
  }
  node->children = NULL;        /* last created node is a leaf */
  return 0;                     /* return 'ok' */
}  /* cmt_add() */

/*--------------------------------------------------------------------*/

SUPP_T cmt_get (CMTREE *cmt, const int *items, int n)
{                               /* --- get support of an item set */
  int    i;                     /* buffer for an item */
  CMNODE *p;                    /* to traverse the nodes */

  assert(cmt && (items || (n <= 0))); /* check function arguments */
  p = &cmt->root;               /* start search at the root node */
  while (--n >= 0) {            /* traverse the items of the set */
    i = *items++;               /* try to find a child node */
    p = p->children;            /* with the next item in the set */
    if (cmt->dir < 0) while (p && (p->item > i)) p = p->sibling;
    else              while (p && (p->item < i)) p = p->sibling;
    if (!p || (p->item != i))   /* if a node with the next item */
      return -1;                /* does not exist in the tree, */
  }                             /* abort the search with failure */
  return p->supp;               /* return support of the item set */
}  /* cmt_get() */

/*--------------------------------------------------------------------*/

int cmt_check (CMTREE *cmt, const int *items, int n, SUPP_T supp)
{                               /* --- get support of an item set */
  int    i;                     /* buffer for an item */
  CMNODE *p;                    /* to traverse the nodes */

  assert(cmt                    /* check the function arguments */
  &&    (items || (n <= 0)) && (supp >= 0));
  p = &cmt->root;               /* start search at the root node */
  while (1) {                   /* traverse the items of the set */
    if (p->supp < supp) return 0;  /* check current node support */
    if (--n < 0) return p->supp;   /* if all items matched, abort */
    i = *items++;               /* try to find a child node */
    p = p->children;            /* with the next item in the set */
    if (cmt->dir < 0) while (p && (p->item > i)) p = p->sibling;
    else              while (p && (p->item < i)) p = p->sibling;
    if (!p || (p->item != i))   /* if a node with the next item */
      return -1;                /* does not exist in the tree, */
  }                             /* abort the search with failure */
}  /* cmt_check() */

/*--------------------------------------------------------------------*/

#define MERGE(dir) \
static CMNODE* _merge_##dir (CMNODE *s1, CMNODE *s2, MEMSYS *mem)      \
{                               /* --- merge two node list */          \
  CMNODE *out, **end, *p;       /* output node list and end pointer */ \
                                                                       \
  assert(mem);                  /* check the function arguments */     \
  if (!s1) return s2;           /* if there is only one node list, */  \
  if (!s2) return s1;           /* simply return the other list */     \
  end = &out;                   /* start the output list */            \
  while (1) {                   /* node list merge loop */             \
    if      (dir(s1->item, s2->item)) {                                \
      *end = s1; end = &s1->sibling; s1 = *end; if (!s1) break; }      \
    else if (dir(s2->item, s1->item)) {                                \
      *end = s2; end = &s2->sibling; s2 = *end; if (!s2) break; }      \
    else {                      /* copy nodes with singular items */   \
      s1->children = _merge_##dir(s1->children, s2->children, mem);    \
      if (s1->supp < s2->supp)  /* merge the children recursively */   \
        s1->supp = s2->supp;    /* and update the node support */      \
      p    = s2; s2  =  s2->sibling; ms_free(mem, p);                  \
      *end = s1; end = &s1->sibling; s1 = *end;                        \
      if (!s1 || !s2) break;    /* move node from the first source */  \
    }                           /* to the output and delete the one */ \
  }                             /* from the second source */           \
  *end = (s1) ? s1 : s2;        /* append the remaining nodes */       \
  return out;                   /* return the merged prefix tree */    \
}  /* _merge() */

/*--------------------------------------------------------------------*/

MERGE(pos)                      /* function for ascending  item order */
MERGE(neg)                      /* function for descending item order */

/*--------------------------------------------------------------------*/

#define PRUNE(dir) \
static CMNODE* _prune_##dir (CMNODE *node, int item, MEMSYS *mem)      \
{                               /* --- prune a c/m prefix tree */      \
  CMNODE *p, *b = NULL;         /* buffer for merged subtrees */       \
                                                                       \
  assert(mem);                  /* check the function arguments */     \
  while (node && dir(node->item, item)) {                              \
    node->children =            /* prune children of current node */   \
    p = _prune_##dir(node->children, item, mem);                       \
    if (p) b = (!b) ? p : _merge_##dir(b, p, mem);                     \
    p    = node;                /* merge remaining children with */    \
    node = node->sibling;       /* the already collected subtrees */   \
    ms_free(mem, p);            /* and delete the processed node */    \
  }                             /* fimally merge with rem. nodes */    \
  return (!node) ? b : (!b) ? node : _merge_##dir(b, node, mem);       \
}  /* _prune() */

/*--------------------------------------------------------------------*/

PRUNE(pos)                      /* function for ascending  item order */
PRUNE(neg)                      /* function for descending item order */

/*--------------------------------------------------------------------*/

void cmt_prune (CMTREE *cmt, int item)
{                               /* --- prune a c/m prefix tree */
  CMNODE *p;                    /* to access the tree nodes */

  assert(cmt                    /* check the function arguments */
  &&    (item >= 0) && (item < cmt->size));
  cmt->item = item;             /* note the item pruned with and */
  p = &cmt->root;               /* prune up to, but excl. the item */
  p = p->children = (cmt->dir < 0)
    ? _prune_neg(p->children, item, cmt->mem)
    : _prune_pos(p->children, item, cmt->mem);
  cmt->max = (p && (p->item == item)) ? p->supp : 0;
}  /* cmt_prune() */            /* set the maximal item set support */

/*--------------------------------------------------------------------*/

static CMNODE* _copy (const CMNODE *src, MEMSYS *mem)
{                               /* --- copy a c/m subtree */
  CMNODE *dst, *node;           /* created copy of the node list */
  CMNODE **end = &dst;          /* end of the created copy */
  CMNODE *c;                    /* buffer for copied children */

  assert(src && mem);           /* check the function arguments */
  do {                          /* sibling copying loop */
    *end = node = (CMNODE*)ms_alloc(mem);
    if (!node) break;           /* create a copy for each node */
    node->item = src->item;     /* copy the item  */
    node->supp = src->supp;     /* and its support */
    c = src->children;          /* if there are children, copy them */
    if (c && !(c = _copy(c, mem))) break;
    node->children = c;         /* store the (copied) children */
    end = &node->sibling;       /* get the new list end */
    src = src->sibling;         /* (to append the next node) */
  } while (src);                /* check for another sibling */
  *end = NULL;                  /* terminate the copied list */
  if (src) { _delete(dst, mem); return NULL; }
  return dst;                   /* return the created copy */
}  /* _copy() */

/*--------------------------------------------------------------------*/

CMTREE* cmt_proj (CMTREE *dst, CMTREE *src, int item)
{                               /* --- project a c/m prefix tree */
  CMTREE *arg = dst;            /* buffer for destination argument */
  CMNODE *p;                    /* to traverse the tree nodes */

  assert(src                    /* check the function arguments */
  &&    (item >= 0) && (item < src->size)
  &&    (!dst || (dst->dir == src->dir)));
  if (!dst) dst = cmt_create(src->mem, src->dir, src->size-1);
  if (!dst) return NULL;        /* create/reinit. destination tree */
  src->item = item;             /* note the projection item and */
  dst->item = -1;               /* clear maximum item set support */
  dst->root.supp = dst->max = src->max = 0;
  assert(!dst->root.children);  /* check for an empty destination */
  p = &src->root;               /* if the prefix tree is empty, */
  if (!p->children) return dst; /* directly return the destination */
  p = p->children = (src->dir < 0) /* prune top level up to the item */
    ? _prune_neg(p->children, item, src->mem)
    : _prune_pos(p->children, item, src->mem);
  if (!p || (p->item != item))  /* if projection item is missing, */
    return dst;                 /* directly return the destination */
  dst->root.supp = src->max = p->supp;   /* set projection support */
  if (p->children) {            /* if projection item has children */
    dst->root.children = p = _copy(p->children, dst->mem);
    if (!p) { if (!arg) cmt_delete(dst, 0); return NULL; }
  }                             /* copy the child nodes to the dest. */
  p = &src->root;               /* prune the projection item */
  p->children = (src->dir < 0)  /* from the source prefix tree */
              ? _prune_neg(p->children, item-1, src->mem)
              : _prune_pos(p->children, item+1, src->mem);
  return dst;                   /* return the destination tree */
}  /* cmt_proj() */

/*--------------------------------------------------------------------*/

#define XCOPY(dir) \
static CMNODE* _xcopy_##dir (const CMNODE *src, MEMSYS *mem,           \
                             const int *keep)                          \
{                               /* --- copy a c/m subtree */           \
  CMNODE *dst, *node;           /* created copy of the node list */    \
  CMNODE **end = &dst;          /* end of the created copy */          \
  CMNODE *c, *b = NULL;         /* buffer for copied children */       \
                                                                       \
  assert(src && mem);           /* check the function arguments */     \
  do {                          /* sibling copying loop */             \
    c = src->children;          /* if there children, copy them */     \
    if (c && ((c = _xcopy_##dir(c, mem, keep)) == COPYERR)) break;     \
    if (keep[src->item]) {      /* if to copy the node itself */       \
      *end = node = (CMNODE*)ms_alloc(mem);                            \
      if (!node) break;         /* create a copy of the node */        \
      node->item = src->item;   /* copy the item and the support */    \
      node->supp = src->supp;   /* into the created copy */            \
      node->children = c;       /* set the (copied) children */        \
      end = &node->sibling; }   /* get the new list end */             \
    else if (c)                 /* if there are copied children */     \
      b = (b) ? _merge_##dir(b, c, mem) : c;                           \
    src = src->sibling;         /* merge them into a buffer */         \
  } while (src);                /* check for another sibling */        \
  *end = NULL;                  /* terminate the copied list */        \
  if (src) { _delete(dst, mem); return COPYERR; }                      \
  return (!b) ? dst : (!dst) ? b : _merge_##dir(dst, b, mem);          \
}  /* _xcopy() */               /* return the created copy */

/*--------------------------------------------------------------------*/

XCOPY(pos)                      /* function for ascending  item order */
XCOPY(neg)                      /* function for descending item order */

/*--------------------------------------------------------------------*/

CMTREE* cmt_xproj (CMTREE *dst, CMTREE *src, int item,
                   const int *keep, int n)
{                               /* --- project a c/m prefix tree */
  int    i;                     /* loop variable */
  CMTREE *arg = dst;            /* buffer for destination argument */
  CMNODE *p;                    /* to traverse the tree nodes */

  assert(src                    /* check the function arguments */
  &&    (item >= 0) && (item < src->size)
  &&    (!dst || (dst->dir == src->dir)) && keep);
  if (!dst) dst = cmt_create(src->mem, src->dir, src->size-1);
  if (!dst) return NULL;        /* create the destination tree */
  src->item = item;             /* note the projection item and */
  dst->item = -1;               /* clear maximum item set support */
  dst->root.supp = dst->max = src->max = 0;
  assert(!dst->root.children);  /* check for an empty destination */
  p = &src->root;               /* prune top level up to the item */
  p = p->children = (src->dir < 0)
    ? _prune_neg(p->children, item, src->mem)
    : _prune_pos(p->children, item, src->mem);
  if (!p || (p->item != item))  /* if projection item is missing, */
    return dst;                 /* directly return the destination */
  dst->root.supp = src->max = p->supp;   /* set projection support */
  if (p->children) {            /* if projection item has children */
    for (i = n; --i >= 0; )     /* traverse the items to keep and */
      dst->keep[keep[i]] = 1;   /* set the corresponding flags */
    p = (dst->dir < 0)          /* copy branch of projection item */
      ? _xcopy_neg(p->children, dst->mem, dst->keep)
      : _xcopy_pos(p->children, dst->mem, dst->keep);
    for (i = n; --i >= 0; )     /* traverse the items to keep and */
      dst->keep[keep[i]] = 0;   /* clear the corresponding flags */
    if (p == COPYERR) { if (!arg) cmt_delete(dst, 0); return NULL; }
    dst->root.children = p;     /* check for copying error and */
  }                             /* store the copied branch */
  p = &src->root;               /* prune the projection item */
  p->children = (src->dir < 0)  /* from the source prefix tree */
              ? _prune_neg(p->children, item-1, src->mem)
              : _prune_pos(p->children, item+1, src->mem);
  return dst;                   /* return the destination tree */
}  /* cmt_xproj() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

static void _indent (int k)
{ while (--k >= 0) printf("   "); }

/*--------------------------------------------------------------------*/

static void _show (CMNODE *node, ITEMBASE *base, int ind)
{                               /* --- recursively show nodes */
  assert(ind >= 0);             /* check the function arguments */
  while (node) {                /* traverse the node list */
    _indent(ind);               /* indent the output line */
    if (base) printf("%s/%d:", ib_name(base, node->item), node->item);
    else      printf(   "%d:", node->item);
    printf("%d\n", node->supp); /* print the node information */
    _show(node->children, base, ind+1);
    node = node->sibling;       /* recursively show the child nodes, */
  }                             /* then go to the next node */
}  /* _show() */

/*--------------------------------------------------------------------*/

void cmt_show (CMTREE *cmt, ITEMBASE *base, int ind)
{                               /* --- print a c/m prefix tree */
  _indent(ind);                 /* indent the output line */
  if (!cmt) {                   /* check whether tree exists */
    printf("(null)\n"); return; }
  if (cmt->max < 0) {           /* check whether tree is valid */
    printf("invalid\n"); return; }
  printf("item: ");             /* print the associated item */
  if (base) printf("%s/%d", ib_name(base, cmt->item), cmt->item);
  else      printf(   "%d", cmt->item);
  printf(" (%d)\n", cmt->max);  /* print root node information */
  _indent(ind);                 /* indent the output line */
  printf("*:%d\n",  cmt->root.supp);
  _show(cmt->root.children, base, ind+1);
}  /* cmt_show() */             /* recursively show the nodes */

#endif
/*----------------------------------------------------------------------
  Closed/Maximal Filter Functions
----------------------------------------------------------------------*/

CLOMAX* cm_create (int dir, int size)
{                               /* --- create a c/m filter object */
  CLOMAX *cm;                   /* created closed/maximal filter */
  CMTREE *t;                    /* created root prefix tree */

  assert(size > 0);             /* check the function arguments */
  cm = (CLOMAX*)calloc(1, sizeof(CLOMAX) +size *sizeof(CMTREE*));
  if (!cm) return NULL;         /* create a closed/maximal filter */
  cm->size = size;              /* and initialize its fields */
  cm->dir  = (dir < 0) ? -1 : 1;
  cm->cnt  = 0;                 /* there is no prefix yet */
  cm->mem  = ms_create(sizeof(CMNODE), 65535);
  if (!cm->mem) { free(cm); return NULL; }  /* block size 1MB */
  cm->trees[0] = t = cmt_create(cm->mem, dir, size);
  if (!t) { cm_delete(cm); return NULL; }
  cmt_add(t, NULL, 0, 0);       /* create and init. a root tree */
  return cm;                    /* return the created c/m filter */
}  /* cm_create() */

/*--------------------------------------------------------------------*/

void cm_delete (CLOMAX *cm)
{                               /* --- delete a c/m filter object */
  int i;                        /* loop variable */

  assert(cm);                   /* check the function argument */
  for (i = 0; cm->trees[i]; i++)/* traverse the c/m prefix trees */
    cmt_delete(cm->trees[i],-1);/* and delete them */
  ms_delete(cm->mem);           /* delete underlying memory system */
  free(cm);                     /* delete the base structure */
}  /* cm_delete() */

/*--------------------------------------------------------------------*/

SUPP_T cm_supp (CLOMAX *cm)
{                               /* --- get support of current prefix */
  assert(cm);                   /* check the function argument */
  return (cm->cnt > 0) ? cmt_max (cm->trees[cm->cnt-1])
                       : cmt_supp(cm->trees[0]);
}  /* cm_supp() */

/*--------------------------------------------------------------------*/

SUPP_T cm_add (CLOMAX *cm, int item, SUPP_T supp)
{                               /* --- add an item to the prefix */
  CMTREE *t, **p;               /* to access the c/m prefix trees */

  assert(cm                     /* check the function arguments */
  &&    (item >= 0) && (item < cm->size));
  p = cm->trees +cm->cnt;       /* get the current prefix tree */
  if (!*p || !cmt_valid(*p)) {  /* if there is no (valid) tree, */
    t = p[-1];                  /* get the parent tree (source), */
    t = cmt_proj(*p,t,t->item); /* project it to the corresponding */
    if (!t) return -1; *p = t;  /* extension item (end of prefix), */
  }                             /* and store the created projection */
  cmt_prune(t = *p, item);      /* prune the current prefix tree */
  if (t->max >= supp) return 0; /* check the support of the prefix */
  return ++cm->cnt;             /* and count the added item */
}  /* cm_add() */

/*--------------------------------------------------------------------*/

void cm_remove (CLOMAX *cm, int n)
{                               /* --- remove items from the prefix */
  assert(cm && (n >= 0));       /* check the function arguments */
  if (n > cm->cnt)              /* cannot remove more items */
    n = cm->cnt;                /* than have been added before */
  for ( ; --n >= 0; cm->cnt--)  /* traverse and clear the trees */
    if (cm->trees[cm->cnt]) cmt_clear(cm->trees[cm->cnt]);
}  /* cm_remove() */

/*--------------------------------------------------------------------*/

int cm_tail (CLOMAX *cm, const int *items, int n)
{                               /* --- prune with the tail items */
  int    s;                     /* support of the tail item set */
  CMTREE *t, **p;               /* to access the c/m prefix trees */

  assert(cm && (items || (n <= 0))); /* check the function arguments */
  if (n == 0) return 1;         /* empty tails can be ignored */
  if (cm->cnt <= 0) return 0;   /* check for a non-empty prefix */
  p = cm->trees +cm->cnt;       /* get the current prefix tree and */
  t = p[-1];                    /* project parent with the tail items */
  t = cmt_xproj(*p, t, t->item, items, n);
  if (!t) return -1; *p = t;    /* set the created projection */
  if (n < 0) return 0;          /* if not to check support, abort */
  s = cmt_get(t, items, n);     /* get support of given tail item set */
  return (s > 0) ? s : 0;       /* and check whether it is positive */
}  /* cm_tail() */

/*--------------------------------------------------------------------*/

int cm_update (CLOMAX *cm, const int *items, int n, SUPP_T supp)
{                               /* --- update filter with found set */
  int    i;                     /* loop variable */
  CMTREE *t;                    /* to traverse the prefix trees */

  assert(cm                     /* check function arguments */
  &&    (items || (n <= 0)) && (supp >= 0));
  for (i = 0; i < cm->cnt; i++) {
    t = cm->trees[i];           /* traverse the c/m prefix trees */
    while (*items != t->item) { ++items; --n; }
    if (cmt_add(t, ++items, --n, supp) < 0)
      return -1;                /* add the proper suffix of the set */
  }                             /* to report to the c/m prefix trees */
  return 0;                     /* return 'ok' */
}  /* cm_update() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

void cm_show (CLOMAX *cm, ITEMBASE *base, int ind)
{                               /* --- print a closed/maximal filter */
  int i;                        /* loop variable */

  assert(cm);                   /* check the function arguments */
  for (i = 0; i <= cm->cnt; i++)/* traverse and print the c/m trees */
    cmt_show(cm->trees[i], base, ind);
}  /* cm_show() */

#endif
