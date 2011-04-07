/*----------------------------------------------------------------------
  File    : symtab.c
  Contents: symbol table management
  Author  : Christian Borgelt
  History : 1995.10.22 file created
            1995.10.30 functions made independent of symbol data
            1995.11.26 symbol types and visibility levels added
            1996.01.04 function st_clear() added (remove all symbols)
            1996.02.27 function st_insert() modified
            1996.06.28 dynamic bin array enlargement added
            1996.07.04 bug in hash bin reorganization removed
            1997.04.01 functions st_clear() and st_remove() combined
            1998.02.06 default table sizes changed
            1998.05.31 list of all symbols removed
            1998.06.20 deletion function moved to st_create()
            1998.09.01 bug in function _sort removed, assertions added
            1998.09.25 hash function improved
            1998.09.28 types ULONG and CCHAR removed, st_stats() added
            1999.02.04 long int changed to int (32 bit systems)
            1999.11.10 name/identifier map management added
            2003.08.15 renamed new to nel in st_insert() (C++ compat.)
            2004.12.15 function nim_trunc() added (truncate lists)
            2004.12.28 bug in function nim_trunc() fixed
            2008.08.11 function nim_getid() added
            2010.08.20 symbol table reorganization redesigned
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "symtab.h"
#ifdef NIMAPFN
#include "arrays.h"
#endif
#ifdef STORAGE
#include "storage.h"
#endif

/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define DFLT_INIT    16383      /* default initial hash table size */
#define DFLT_MAX   4194303      /* default maximal hash table size */
#define BLKSIZE       4096      /* block size for identifier array */

/*----------------------------------------------------------------------
  Default Hash Function
----------------------------------------------------------------------*/

static unsigned int _hdflt (const char *name, int type)
{                               /* --- default hash function */
  register unsigned int h = type;   /* hash value */
  while (*name) h ^= (h << 7) ^ (h << 1) ^ (unsigned int)(*name++);
  return h;                     /* compute hash value */
}  /* _hdflt() */

/*----------------------------------------------------------------------
  Auxiliary Functions
----------------------------------------------------------------------*/

static void _delsym (SYMTAB *tab)
{                               /* --- delete all symbols */
  int i;                        /* loop variable */
  STE *e, *t;                   /* to traverse the symbol list */

  assert(tab);                  /* check the function argument */
  for (i = tab->size; --i >= 0; ) {
    e = tab->bins[i];           /* traverse the bin array */
    tab->bins[i] = NULL;        /* clear the current bin */
    while (e) {                 /* traverse the bin list */
      t = e; e = e->succ;       /* note the symbol and get next */
      if (tab->delfn) tab->delfn(t+1);
      free(t);                  /* if a deletion function is given, */
    }                           /* call it and then deallocate */
  }                             /* the symbol table element */
}  /* _delsym() */

/*--------------------------------------------------------------------*/

static STE* _sort (STE *list)
{                               /* --- sort a transaction list */
  STE *a, *b = list;            /* to traverse the lists */
  STE **e;                      /* end of the output list */

  for (a = list->succ; a; ) {   /* traverse the list to sort */
    a = a->succ;                /* two steps on a, one step on list */
    if (a) { a = a->succ; list = list->succ; }
  }                             /* (split list into two halves) */
  a = list->succ;               /* get the second list and */
  list->succ = NULL;            /* terminate the first list */
  if (a->succ) a = _sort(a);    /* and sort them recursively */
  if (b->succ) b = _sort(b);    /* if longer than one element */
  for (e = &list; 1; ) {        /* transaction list merge loop */
    if (a->level < b->level) {  /* copy element from first  list */
      *e = a; e = &a->succ; if (!(a = *e)) break; }
    else {                      /* copy element from second list */
      *e = b; e = &b->succ; if (!(b = *e)) break; }
  }                             /* (sort by visibility level) */
  *e = (a) ? a : b;             /* append the non-empty list */
  return list;                  /* return the sorted list */
}  /* _sort() */

/*--------------------------------------------------------------------*/

static void _reorg (SYMTAB *tab)
{                               /* --- reorganize a hash table */
  int i, k, size;               /* loop variables, new bin array size */
  STE **p, *e, *t;              /* to traverse symbol table elements */

  assert(tab);                  /* check the function argument */
  size = (tab->size << 1) +1;   /* calculate new bin array size */
  if ((size > tab->max)         /* if new size exceeds maximum */
  && ((size = tab->max) <= tab->size))
    return;                     /* set the maximal size */
  p = (STE**)calloc(size, sizeof(STE*));
  if (!p) return;               /* get an enlarged bin array */
  for (i = tab->size; --i >= 0; ) {
    for (e = tab->bins[i]; e; ) {
      t = e; e = e->succ;       /* traverse the old hash bins */
      k = tab->hash(t->name, t->type) % size;
      t->succ = p[k]; p[k] = t; /* compute the hash bin index */
    }                           /* and insert the symbol at */
  }                             /* the head of the bin list */
  free(tab->bins);              /* delete  the old bin array */
  tab->bins = p;                /* and set the new bin array */
  for (i = tab->size = size; --i >= 0; )
    if (p[i] && p[i]->succ)     /* sort all bin lists according */
      p[i] = _sort(p[i]);       /* to the visibility level */
}  /* _reorg() */

/*----------------------------------------------------------------------
  Symbol Table Functions
----------------------------------------------------------------------*/

SYMTAB* st_create (int init, int max, HASHFN hash, OBJFN delfn)
{                               /* --- create a symbol table */
  SYMTAB *tab;                  /* created symbol table */

  if (init <= 0) init = DFLT_INIT;  /* check and adapt the initial */
  if (max  <= 0) max  = DFLT_MAX;   /* and maximal bin array size */
  tab = (SYMTAB*)malloc(sizeof(SYMTAB));
  if (!tab) return NULL;        /* allocate symbol table body */
  tab->bins = (STE**)calloc(init, sizeof(STE*));
  if (!tab->bins) { free(tab); return NULL; }
  tab->level = tab->cnt = 0;    /* allocate the hash bin array */
  tab->size  = init;            /* and initialize fields */
  tab->max   = max;             /* of symbol table body */
  tab->hash  = (hash) ? hash : _hdflt;
  tab->delfn = delfn;
  tab->vsz   = INT_MAX;
  tab->ids   = NULL;
  return tab;                   /* return created symbol table */
}  /* st_create() */

/*--------------------------------------------------------------------*/

void st_delete (SYMTAB *tab)
{                               /* --- delete a symbol table */
  assert(tab && tab->bins);     /* check argument */
  _delsym(tab);                 /* delete all symbols, */
  free(tab->bins);              /* the hash bin array, */
  if (tab->ids) free(tab->ids); /* the identifier array, */
  free(tab);                    /* and the symbol table body */
}  /* st_delete() */

/*--------------------------------------------------------------------*/

void* st_insert (SYMTAB *tab, const char *name, int type, size_t size)
{                               /* --- insert a symbol */
  unsigned int h;               /* hash value */
  int i;                        /* index of hash bin */
  STE *e, *n;                   /* to traverse a bin list */

  assert(tab && name            /* check the function arguments */
      && ((size >= sizeof(int)) || (tab->vsz == INT_MAX)));
  if ((tab->cnt  > tab->size)   /* if the bins are rather full and */
  &&  (tab->size < tab->max))   /* table does not have maximal size, */
    _reorg(tab);                /* reorganize the hash table */

  h = tab->hash(name, type);    /* compute the hash value and */
  i = h % tab->size;            /* the index of the hash bin */
  for (e = tab->bins[i]; e; e = e->succ)
    if ((type == e->type) && (strcmp(name, e->name) == 0))
      break;                    /* check whether symbol exists */
  if (e && (e->level == tab->level))
    return EXISTS;              /* if symbol found on current level */

  #ifdef NIMAPFN                /* if name/identifier map management */
  if (tab->cnt >= tab->vsz) {   /* if the identifier array is full */
    int vsz, **tmp;             /* (new) id array and its size */
    vsz = tab->vsz +((tab->vsz > BLKSIZE) ? tab->vsz >> 1 : BLKSIZE);
    tmp = (int**)realloc(tab->ids, vsz *sizeof(int*));
    if (!tmp) return NULL;      /* resize the identifier array and */
    tab->ids = tmp; tab->vsz = vsz;  /* set new array and its size */
  }                             /* (no resizing for symbol tables */
  #endif                        /* since then tab->vsz = MAX_INT) */
  n = (STE*)malloc(sizeof(STE) +size +strlen(name) +1);
  if (!n) return NULL;          /* allocate memory for new symbol */
  n->name  = (char*)(n+1)+size; /* and organize it */
  strcpy(n->name, name);        /* note the symbol name, */
  n->type  = type;              /* the symbol type, and the */
  n->level = tab->level;        /* current visibility level */
  n->succ  = tab->bins[i];      /* insert new symbol at the head */
  tab->bins[i] = n++;           /* of the hash bin list */
  #ifdef NIMAPFN                /* if name/identifier maps are */
  if (tab->ids) {               /* supported and this is such a map */
    tab->ids[tab->cnt] = (int*)n;
    *(int*)n = tab->cnt;        /* store the new symbol */
  }                             /* in the identifier array */
  #endif                        /* and set the symbol identifier */
  tab->cnt++;                   /* increment the symbol counter */
  return n;                     /* return pointer to data field */
}  /* st_insert() */

/*--------------------------------------------------------------------*/

int st_remove (SYMTAB *tab, const char *name, int type)
{                               /* --- remove a symbol/all symbols */
  int i;                        /* index of hash bin */
  STE **p, *e;                  /* to traverse a hash bin list */

  assert(tab);                  /* check for a valid symbol table */
  if (!name) {                  /* if no symbol name given */
    _delsym(tab);               /* delete all symbols */
    tab->cnt = tab->level = 0;  /* reset visibility level */
    return 0;                   /* and symbol counter */
  }                             /* and return 'ok' */
  i = tab->hash(name, type) % tab->size;
  p = tab->bins +i;             /* compute index of hash bin */
  while (*p) {                  /* and traverse the bin list */
    if (((*p)->type == type) && (strcmp(name, (*p)->name) == 0))
      break;                    /* if symbol found, abort loop */
    p = &(*p)->succ;            /* otherwise get the successor */
  }                             /* in the hash bin */
  e = *p;                       /* if the symbol does not exist, */
  if (!e) return -1;            /* abort the function with failure */
  *p = e->succ;                 /* remove symbol from hash bin */
  if (tab->delfn) tab->delfn(e+1);      /* delete user data */
  free(e);                      /* and symbol table element */
  tab->cnt--;                   /* decrement symbol counter */
  return 0;                     /* return 'ok' */
}  /* st_remove() */

/*--------------------------------------------------------------------*/

void* st_lookup (SYMTAB *tab, const char *name, int type)
{                               /* --- look up a symbol */
  int i;                        /* index of hash bin */
  STE *e;                       /* to traverse a hash bin list */

  assert(tab && name);          /* check arguments */
  i = tab->hash(name, type) % tab->size;
  e = tab->bins[i];             /* compute index of hash bin */
  while (e) {                   /* and traverse the bin list */
    if ((e->type == type) && (strcmp(name, e->name) == 0))
      return e +1;              /* if symbol found, return its data */
    e = e->succ;                /* otherwise get the successor */
  }                             /* in the hash bin */
  return NULL;                  /* return 'not found' */
}  /* st_lookup() */

/*--------------------------------------------------------------------*/

void st_endblk (SYMTAB *tab)
{                               /* --- remove one visibility level */
  int i;                        /* loop variable */
  STE *e, *t;                   /* to traverse bin lists */

  assert(tab);                  /* check for a valid symbol table */
  if (tab->level <= 0) return;  /* if on level 0, abort */
  for (i = tab->size; --i >= 0; ) {  /* traverse the bin array */
    e = tab->bins[i];           /* remove all symbols of higher level */
    while (e && (e->level >= tab->level)) {
      t = e; e = e->succ;       /* note symbol and get successor */
      if (tab->delfn) tab->delfn(t+1);
      free(t);                  /* delete user data and */
      tab->cnt--;               /* symbol table element and */
    }                           /* decrement symbol counter */
    tab->bins[i] = e;           /* set new start of bin list */
  }
  tab->level--;                 /* go up one level */
}  /* st_endblk() */

/*--------------------------------------------------------------------*/
#ifndef NDEBUG

void st_stats (const SYMTAB *tab)
{                               /* --- compute and print statistics */
  const STE *e;                 /* to traverse hash bin lists */
  int i;                        /* loop variable */
  int used;                     /* number of used hash bins */
  int len;                      /* length of current bin list */
  int min, max;                 /* min. and max. bin list length */
  int cnts[10];                 /* counter for bin list lengths */

  assert(tab);                  /* check for a valid symbol table */
  min = INT_MAX; max = used = 0;/* initialize variables */
  for (i = 10; --i >= 0; ) cnts[i] = 0;
  for (i = tab->size; --i >= 0; ) { /* traverse the bin array */
    len = 0;                    /* determine bin list length */
    for (e = tab->bins[i]; e; e = e->succ) len++;
    if (len > 0) used++;        /* count used hash bins */
    if (len < min) min = len;   /* determine minimal and */
    if (len > max) max = len;   /* maximal list length */
    cnts[(len >= 9) ? 9 : len]++;
  }                             /* count list length */
  printf("number of symbols  : %d\n", tab->cnt);
  printf("number of hash bins: %d\n", tab->size);
  printf("used hash bins     : %d\n", used);
  printf("minimal list length: %d\n", min);
  printf("maximal list length: %d\n", max);
  printf("average list length: %g\n", (double)tab->cnt/tab->size);
  printf("ditto, of used bins: %g\n", (double)tab->cnt/used);
  printf("length distribution:\n");
  for (i = 0; i < 9; i++) printf("%5d ", i);
  printf("   >8\n");
  for (i = 0; i < 9; i++) printf("%5d ", cnts[i]);
  printf("%5d\n", cnts[9]);
}  /* st_stats() */

#endif
/*----------------------------------------------------------------------
  Name/Identifier Map Functions
----------------------------------------------------------------------*/
#ifdef NIMAPFN

NIMAP* nim_create (int init, int max, HASHFN hash, OBJFN delfn)
{                               /* --- create a name/identifier map */
  NIMAP *nim;                   /* created name/identifier map */

  nim = st_create(init, max, hash, delfn);
  if (!nim) return NULL;        /* create a name/identifier map */
  nim->vsz = 0;                 /* and clear the id. array size */
  return nim;                   /* return created name/id map */
}  /* nim_create() */

/*--------------------------------------------------------------------*/

int nim_getid (NIMAP *nim, const char *name)
{                               /* --- get an item identifier */
  STE *p = (STE*)nim_byname(nim, name);
  return (p) ? *(int*)p : -1;   /* look up the given name and */
}  /* nim_getid() */            /* return its identifier or -1 */

/*--------------------------------------------------------------------*/

void nim_sort (NIMAP *nim, CMPFN cmpfn, void *data, int *map, int dir)
{                               /* --- sort name/identifier map */
  int i;                        /* loop variable */
  int **p;                      /* to traverse the value array */

  assert(nim && cmpfn);         /* check the function arguments */
  ptr_qsort(nim->ids, nim->cnt, cmpfn, data);
  if (!map) {                   /* if no conversion map is requested */
    for (p = nim->ids +(i = nim->cnt); --i >= 0; )
      **--p = i; }              /* just set new identifiers */
  else {                        /* if a conversion map is requested, */
    p = nim->ids +(i = nim->cnt);       /* traverse the sorted array */
    if (dir < 0)                /* if backward map (i.e. new -> old) */
      while (--i >= 0) { map[i] = **--p; **p = i; }
    else                        /* if forward  map (i.e. old -> new) */
      while (--i >= 0) { map[**--p] = i; **p = i; }
  }                             /* (build conversion map) */
}  /* nim_sort() */

/*--------------------------------------------------------------------*/

void nim_trunc (NIMAP *nim, int n)
{                               /* --- truncate name/identifier map */
  int *id;                      /* to access the identifiers */

  while (nim->cnt > n) {        /* while to remove mappings */
    id = nim->ids[nim->cnt -1]; /* get the identifier object */
    st_remove(nim, st_name(id), 0);
  }                             /* remove the symbol table element */
}  /* nim_trunc() */

#endif
