/*----------------------------------------------------------------------
  File    : fpgrowth.c
  Contents: fpgrowth algorithm for finding frequent item sets
  Author  : Christian Borgelt
  History : 2004.11.21 file created from eclat.c
            2004.11.22 pruning of tree projection to bonsai added
            2004.11.23 absolute/relative support output changed
            2004.12.09 filter added (binary logarithm of supp. quotient)
            2005.06.20 use of flag for "no item sorting" corrected
            2006.11.26 adapted to new structures ISFMTR and ISEVAL
            2007.02.13 adapted to modified module tabscan

	    Changes by Li Ma
	    2008.06.02 start modifying _report
	    2008.10.02 further modified _report. odds ratio added
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include "scan.h"
#include "fptree.h"
#ifdef STORAGE
#include "storage.h"
#endif


/*----------------------------------------------------------------------
  Preprocessor Definitions
----------------------------------------------------------------------*/
#define PRGNAME     "fpgrowth"
#define DESCRIPTION "find frequent item sets within cases     " \
                    "using a weighted fpgrowth algorithm"
#define VERSION     "version 2.1 (2008.10.02)        " \
                    "(c) 2004-2008   Christian Borgelt    "\
                    "modification by Li Ma (c) 2008      "

/* --- output flags --- */
#define OF_REL      0x01        /* print relative support */
#define OF_ABS      0x02        /* print absolute support */
#define OF_DEV      0x04        /* print deviation from indep. occ. */
#define OF_SCAN     0x08        /* convert names to scanable form */

#define OF_ITEMSET  0x10        /** whether to print itemset **/
#define OF_PVAL     0x20        /** whether to print -log Pvals only **/
#define OF_ODDSRATIO 0x40       /** whether to print odds-ratio **/
#define OF_DETAIL   0X80        /** whether to print detailed counts **/
#define OF_SUPPORT_AMONG_CASES 0x100
#define OF_SUPPORT_AMONG_ALL   0x200

/* --- error codes --- */
#define E_OPTION    (-5)        /* unknown option */
#define E_OPTARG    (-6)        /* missing option argument */
#define E_ARGCNT    (-7)        /* too few/many arguments */
#define E_STDIN     (-8)        /* double assignment of stdin */
#define E_SUPP      (-9)        /* invalid item set support */
#define E_ITEMCNT  (-10)        /* invalid number of items */
#define E_NOTAS    (-11)        /* no items or transactions */
#define E_UNKNOWN  (-20)        /** unknown error **/

#define E_PERM_LABEL (-18)      /** Case label file error **/
#define E_PVAL_MAT (-19)        /** P value matrix file error **/



#ifndef QUIET                   /* if not quiet version */
#define MSG(x)        x         /* print messages */
#else                           /* if quiet version */
#define MSG(x)                  /* suppress messages */
#endif

#define SEC_SINCE(t)  ((clock()-(t)) /(double)CLOCKS_PER_SEC)
#define RECCNT(s)     (ts_reccnt(is_tabscan(s)) \
                      - ((ts_delim(is_tabscan(s)) == TS_REC) ? 1 : 0))
#define BUFFER(s)     ts_buf(is_tabscan(s))

/*----------------------------------------------------------------------
  Constants
----------------------------------------------------------------------*/
#ifndef QUIET                   /* if not quiet version */
/* --- error messages --- */
static const char *errmsgs[] = {
  /* E_NONE      0 */  "no error\n",
  /* E_NOMEM    -1 */  "not enough memory\n",
  /* E_FOPEN    -2 */  "cannot open file %s\n",
  /* E_FREAD    -3 */  "read error on file %s\n",
  /* E_FWRITE   -4 */  "write error on file %s\n",
  /* E_OPTION   -5 */  "unknown option -%c\n",
  /* E_OPTARG   -6 */  "missing option argument\n",
  /* E_ARGCNT   -7 */  "wrong number of arguments\n",
  /* E_STDIN    -8 */  "double assignment of standard input\n",
  /* E_SUPP     -9 */  "invalid minimal support %g%%\n",
  /* E_ITEMCNT -10 */  "invalid number of items %d\n",
  /* E_NOTAS   -11 */  "no items or transactions to work on\n",
  /*    -12 to -15 */  NULL, NULL, NULL, NULL,
  /* E_ITEMEXP -16 */  "file %s, record %d: item expected\n",
  /* E_DUPITEM -17 */  "file %s, record %d: duplicate item %s\n",
  /** E_PERM_LABEL -18 **/ "Case labels error",
  /** E_PVAL_MAT -19 **/   "Pvalue matrix error",
  /** E_UNKNOWN -20 **/  "unknown error\n"
};
#endif

/*----------------------------------------------------------------------
  Global Variables
----------------------------------------------------------------------*/
#ifndef QUIET
static char    *prgname;        /* program name for error messages */
#endif
static ITEMSET *itemset = NULL;     /* item set */
static TASET   *taset   = NULL;     /* transaction set */
static FPTREE  *fptree  = NULL;     /* frequent pattern tree */
static FILE    *in      = NULL;     /* input  file */
static FILE    *out     = NULL;     /* output file */
static int     tacnt    = 0;        /* number of transactions */
static ISEVAL  *isevl   = NULL;     /* item set evaluator */
static ISFMTR  *isfmt   = NULL;     /* item set formatter */
static double  mindev   = -DBL_MAX; /* minimal value of deviation */
static int     flags    = OF_PVAL | OF_ODDSRATIO | OF_SUPPORT_AMONG_CASES;   /** output flags **/
static char    *fmt     = "%.1f";   /* output format for support */


/** Global variables added by Li Ma **/
static FILE    *in_perm_labels;
static FILE    *in_pval_mat;

static double *pval_mat = NULL;    /** -log Pval grid **/
static double  pval_thre = 0;        /** threshold for -logPvalues **/

static int     cases_cnt = 0;      /** total number of cases (label 1) **/
static int     control_cnt = 0;    /** total number of controls (label 0) **/
static int     pattern_cnt = 0;    /** total number of patterns found **/

/*----------------------------------------------------------------------
  Item Set Report Function
----------------------------------------------------------------------*/

static int _report (int *ids, int cnt, int pfx,    
		    int supp, int supp_among_cases, 
		    void *data)  
     /** cnt is length of pattern, added supp_among_case **/
{                               /** --- report a frequent item set **/
  double dev = 0;               /* deviation from indep. occurrence */
  double pval;                 /** -log Pval **/
  extern double pval_thre;      /** -log Pval threshold **/
  extern int pattern_cnt;
  int supp_among_controls;      /** **/
  
  assert(ids);                  /* check the function arguments */
 
  supp_among_controls = supp - supp_among_cases;
  pval = pval_mat[supp_among_cases+(cases_cnt+1)*supp_among_controls];

  if (flags & OF_ITEMSET) isf_format(isfmt, ids, cnt, pfx); /** **/

  if (pval >= pval_thre) {
    pattern_cnt++; 
      
    if (flags & OF_DEV) {         /* if to filter w.r.t. deviation */
      dev = ise_eval(isevl, ids, cnt, pfx, supp);
      if (dev < mindev) return 0; /* compute and check the deviation */
    }
    /* from an independent occurrence */
    
    /** output -log Pval **/
    if (flags & OF_PVAL) {
      fprintf(out,"%.2f\t", pval);
    }
      
    if (flags & OF_ODDSRATIO) {
      fprintf(out,"%.2f\t",(supp_among_cases/(double)cases_cnt)/(supp_among_controls/(double)control_cnt));
    }

    if (flags & OF_SUPPORT_AMONG_CASES) {
      fprintf(out,"%.2f\t",100*supp_among_cases/(double)cases_cnt);
    }

    if (flags & OF_SUPPORT_AMONG_ALL) {
      fprintf(out,"%.2f\t",100*(supp_among_cases+supp_among_controls)/(double)(cases_cnt+control_cnt));
    }
    
    if (flags & (OF_REL | OF_ABS | OF_DEV)) {
      fputs("\t(", out);             /* print rel. and abs. support */
      
      if (flags & OF_REL) { fprintf(out, fmt, (supp/(double)tacnt) *100);
      fputs((flags & OF_ABS) ? "%/" : "%", out); }
      if (flags & OF_ABS) { fprintf(out, "%d", supp); }
      if (flags & OF_DEV) { fputs(", ", out); fprintf(out, fmt, dev); }
      /* print deviation from independence */
      fputs(")\t", out);
    }
 
    if (flags & OF_ITEMSET) { 
      isf_print(isfmt, out);        /** print the items of the set **/
      fputs("\t",out);
    }


    if (flags & OF_DETAIL) {
      fprintf(out, "(%d/%.1f%,%d/%.1f%)", supp_among_cases, 100*supp_among_cases/(double)cases_cnt,
	      supp_among_controls, 100*supp_among_controls/(double)control_cnt);
    }

    fputs("\n", out);            
    
  }
  
  
  return 1;                     /* return 'reported 1 item set' */
}  /* _report() */

/*----------------------------------------------------------------------
  Main Functions
----------------------------------------------------------------------*/

static void error (int code, ...)
{                               /* --- print an error message */
  #ifndef QUIET                 /* if not quiet version */
  va_list    args;              /* list of variable arguments */
  const char *msg;              /* error message */

  assert(prgname);              /* check the program name */
  if (code < E_UNKNOWN) code = E_UNKNOWN;
  if (code < 0) {               /* if to report an error, */
    msg = errmsgs[-code];       /* get the error message */
    if (!msg) msg = errmsgs[-E_UNKNOWN];
    fprintf(stderr, "\n%s: ", prgname);
    va_start(args, code);       /* get variable arguments */
    vfprintf(stderr, msg, args);/* print error message */
    va_end(args);               /* end argument evaluation */
  }
  #endif
  #ifndef NDEBUG                /* if debug version */
  if (isfmt)   isf_delete(isfmt);  /* clean up memory */
  if (isevl)   ise_delete(isevl);  /* and close files */
  if (fptree)  fpt_delete(fptree);
  if (taset)   tas_delete(taset, 0);
  if (itemset) is_delete(itemset);
  if (in  && (in  != stdin))  fclose(in);
  if (out && (out != stdout)) fclose(out);
  #endif
  #ifdef STORAGE                /* if storage debugging */
  showmem("at end of program"); /* check memory usage */
  #endif
  exit(code);                   /* abort the program */
}  /* error() */

/*--------------------------------------------------------------------*/

int main (int argc, char *argv[])
{                               /* --- main function */
  int     i, k = 0, n;          /* loop variables, counters */
  char    *s;                   /* to traverse the options */
  char    **optarg = NULL;      /* option argument */
  char    *fn_in   = NULL;      /* name of input  file */
  char    *fn_out  = NULL;      /* name of output file */
  
  char    *blanks  = NULL;      /* blanks */
  char    *fldseps = NULL;      /* field  separators */
  char    *recseps = NULL;      /* record separators */
  char    *cominds = NULL;      /* comment indicators */
  double  supp     = 0.1;       /* minimal support (in percent) */
  int     min      =  1;        /* minimal size of item set */
  int     max      =  5;        /* maximal size of item set */
  int     sort     = -2;        /* flag for item sorting and recoding */
  int     mode     = FPT_BONSAI;/* tree projection mode */
  int     heap     =  1;        /* flag for heap sort vs. quick sort */
  int     *map;                 /* identifier map for recoding */
  clock_t t;                    /* timer for measurements */

  char    *fn_perm_labels = NULL; /** name of the file for class labels **/
  char    *fn_pval_mat = NULL;   /** name of the file for (-log)Pval matrix **/
  int     curr_label; /** **/
  double  curr_pval;   /** **/
  int     n1,n2; /** **/
  int     n_pvals; /** **/


  #ifndef QUIET                 /* if not quiet version */
  prgname = argv[0];            /* get program name for error msgs. */

  /* --- print usage message --- */
  if (argc > 1) {               /* if arguments are given */
    fprintf(stderr, "%s - %s\n", argv[0], DESCRIPTION);
    fprintf(stderr, VERSION); } /* print a startup message */
  else {                        /* if no arguments given */
    printf("usage: %s [options] infile outfile permfile pvalfile\n", argv[0]);
    printf("%s\n", DESCRIPTION);
    printf("%s\n", VERSION);
    printf("-l#      threshold for -log Pvalues (default: 0)\n"); /** **/
    printf("-v       print count detail\n"); /** **/
    printf("-w       print itemset\n");      /** **/
    printf("-x       print total relative support\n"); /** **/
    printf("-m#      minimal number of items per item set "
                    "(default: %d)\n", min);
    printf("-n#      maximal number of items per item set "
                    "(default: %d)\n", max);
    printf("-s#      minimal support of an item set "
                    "(default: %g%%)\n", supp *100);
    printf("         (positive: percentage, "
                    "negative: absolute number)\n");
    printf("-d#      minimal binary logarithm of support quotient "
                    "(default: none)\n");
    printf("-p#      output format for the item set support "
                    "(default: \"%s\")\n", fmt);
    printf("-a       print absolute support "
                    "(number of transactions)\n");
    printf("-g       write output in scanable form "
                    "(quote certain characters)\n");
    printf("-q#      sort items w.r.t. their frequency (default: %d)\n"
           "         (1: ascending, -1: descending, 0: do not sort,\n"
           "          2: ascending, -2: descending w.r.t. "
                    "transaction size sum)\n", sort);
    printf("-u       use alternative tree projection method\n");
    printf("-z       do not prune tree projections to bonsai\n");
    printf("-j       use quicksort to sort the transactions "
                    "(default: heapsort)\n");
    printf("-i#      ignore records starting with a character "
                    "in the given string\n");
    printf("-b/f/r#  blank characters, field and record separators\n"
           "         (default: \" \\t\\r\", \" \\t\", \"\\n\")\n");
    printf("infile   file to read transactions from\n");
    printf("outfile  file to write frequent item sets to\n");
    printf("permfile file to read case/control labels\n");
    printf("pvalfile file to read (-log)P-value grid\n");
    return 0;                   /* print a usage message */
  }                             /* and abort the program */
  #endif  /* #ifndef QUIET */

  /* --- evaluate arguments --- */
  for (i = 1; i < argc; i++) {  /* traverse arguments */
    s = argv[i];                /* get option argument */
    if (optarg) { *optarg = s; optarg = NULL; continue; }
    if ((*s == '-') && *++s) {  /* -- if argument is an option */
      while (*s) {              /* traverse options */
        switch (*s++) {         /* evaluate switches */
	  case 'l': pval_thre = strtod(s, &s);      break; /** **/
	  case 'v': flags |= OF_DETAIL;               break; /** **/
	  case 'w': flags |= OF_ITEMSET;            break; /** **/
	  case 'x':   flags |= OF_REL;              break; /** **/
	  case 'm': min    = (int)strtol(s, &s, 0); break;
          case 'n': max    = (int)strtol(s, &s, 0); break;
          case 's': supp   = 0.01*strtod(s, &s);    break;
          case 'd': mindev =      strtod(s, &s);    break;
          case 'p': optarg = &fmt;                  break;
          case 'a': flags |= OF_ABS;                break;
          case 'g': flags |= OF_SCAN;               break;
          case 'q': sort   = (int)strtol(s, &s, 0); break;
          case 'u': mode  |=  FPT_ALTPROJ;          break;
          case 'z': mode  &= ~FPT_BONSAI;           break;
          case 'j': heap   = 0;                     break;
          case 'i': optarg = &cominds;              break;
          case 'b': optarg = &blanks;               break;
          case 'f': optarg = &fldseps;              break;
          case 'r': optarg = &recseps;              break;
          default : error(E_OPTION, *--s);          break;
        }                       /* set option variables */
        if (optarg && *s) { *optarg = s; optarg = NULL; break; }
      } }                       /* get option argument */
    else {                      /* -- if argument is no option */
      switch (k++) {            /* evaluate non-options */
      case  0: fn_in  = s;      break; /* input file */
      case  1: fn_out = s;      break; /* output file */
      case  2: fn_perm_labels = s;   break; /** permutation subset file **/
      case  3: fn_pval_mat = s; break; /** file for the (-log) pval matrix **/
      default: error(E_ARGCNT); break;
      }                         /* note filenames */
    }
  }
  if (optarg) error(E_OPTARG);  /* check option argument */
  if (k != 4) error(E_ARGCNT);  /** and the number of arguments **/
  if (supp > 1)                 /* check the minimal support */
    error(E_SUPP, supp);        /* (< 0: absolute number) */
  if (min <= 0) error(E_ITEMCNT, min);   /* check the limits */
  if (max <= 0) error(E_ITEMCNT, max);   /* for the set size */
  if (mindev > -DBL_MAX) flags |= OF_DEV;

 
  /* --- create item set and transaction set --- */
  itemset = is_create(-1);      /* create an item set and */
  if (!itemset) error(E_NOMEM); /* set the special characters */
  is_chars(itemset, blanks, fldseps, recseps, cominds);
  taset = tas_create(itemset);  /* create a transaction set */
  if (!taset) error(E_NOMEM);   /* to store the transactions */
  MSG(fprintf(stderr, "\n"));   /* terminate the startup message */

  

  /** --- read transactions and permutation labels --- **/
  MSG(fprintf(stderr, "reading permutation labels ...\n"));
  if (fn_perm_labels && *fn_perm_labels)
    in_perm_labels = fopen(fn_perm_labels, "r");
  else error(E_FOPEN, fn_perm_labels);


  t = clock();                  /* start the timer */
  if (fn_in && *fn_in)          /* if an input file name is given, */
    in = fopen(fn_in, "r");     /* open input file for reading */
  else {                        /* if no input file name is given, */
    in = stdin; fn_in = "<stdin>"; }   /* read from standard input */
  MSG(fprintf(stderr, "reading %s ... ", fn_in));
  if (!in) error(E_FOPEN, fn_in);
  for (tacnt = 0; 1; tacnt++) { /* transaction read loop */
    
    fscanf(in_perm_labels, "%d", &curr_label); /** read the label for this transection **/
 
    k = is_read(itemset, in, curr_label);   /* read the next transaction */
    if (k < 0) error(k, fn_in, RECCNT(itemset), BUFFER(itemset));
    if (k > 0) break;           /* check for error and end of file */
    if (tas_add(taset, NULL, 0, curr_label) != 0) 
      error(E_NOMEM);           /* add the loaded transaction to the transaction set */   
    
    cases_cnt += curr_label; control_cnt += 1-curr_label;
  }                             
  
  fclose(in_perm_labels);       /** close the permutation label file **/

  if (in != stdin) fclose(in);  /* if not read from standard input, */
  in = NULL;                    /* close the input file */
  n  = is_cnt(itemset);         /* get the number of items */
  MSG(fprintf(stderr, "[%d item(s),", n));
  MSG(fprintf(stderr, " %d transaction(s)] done ", tacnt));
  MSG(fprintf(stderr, "[%.2fs].", SEC_SINCE(t)));
  if ((n <= 0) || (tacnt <= 0)) error(E_NOTAS);
  MSG(fprintf(stderr, "\n"));   /* check for at least one transaction */
  if (supp >= 0)                /* if relative support is given */
    supp = ceil(cases_cnt *supp);   /** compute absolute support (among cases) **/
  else {                        /* if absolute support is given */
    supp = ceil(-100  *supp);   /* make the support value positive */
    if (!(flags & OF_ABS)) flags = (flags & ~OF_REL) | OF_ABS;
  }                             /* switch to absolute support output */


  /* --- sort and recode items --- */
  MSG(fprintf(stderr, "filtering, sorting and recoding items ... "));
  t   = clock();                /* start the timer */
  map = (int*)malloc(is_cnt(itemset) *sizeof(int));
  if (!map) error(E_NOMEM);     /* create an item identifier map */
  n = is_recode(itemset, (int)supp, sort, map);
  is_trunc(itemset, n);         /* truncate the itemset and */
  tas_recode(taset, map, n);    /* recode the loaded transactions */
  free(map);                    /* delete the item identifier map */
  if (flags & OF_DEV) {         /* if to compute deviation */
    isevl = ise_create(itemset, tacnt);
    if (!isevl) error(E_NOMEM); /* create an item set evaluator */
  }                             /* (to exploit same prefixes) */
  MSG(fprintf(stderr, "[%d item(s)] ", n));
  MSG(fprintf(stderr, "done [%.2fs].", SEC_SINCE(t)));
  if (n <= 0) error(E_NOTAS);   /* print a log message and */
  MSG(fprintf(stderr, "\n"));   /* check the number of items */



   
  /**
  MSG(fprintf(stderr, "reading permutation labels ..."));
  if (fn_perm_labels && *fn_perm_labels)
    in_perm_labels = fopen(fn_perm_labels, "r");
  else error(E_FOPEN, fn_perm_labels);
 
  perm_label = (int *) malloc(tacnt * sizeof(int));
  
  i = 0;
  while (fscanf(in_perm_labels, "%d", &curr_label) != EOF) {
    if ( i <= tacnt - 1 ) {cases_cnt += perm_label[i] = curr_label;control_cnt += 1 - curr_label;}
    else error(E_PERM_LABEL); 
    i++;
  }
 
  if ( i < tacnt ) error(E_PERM_LABEL);  
  fclose(in_perm_labels); **/
  /** check the number of items in the permuation label file **/


  MSG(fprintf(stderr, "done. %d cases and %d controls in %d observations.\n", 
	      cases_cnt, control_cnt, tacnt));

    /** --- read in the P-value grid --- **/
  MSG(fprintf(stderr, "reading (-log)Pval grid ..."));
  n_pvals = (cases_cnt+1)*(tacnt-cases_cnt+1); /** total number of p-values in the grid **/
  pval_mat=(double *)malloc(n_pvals * sizeof(double));
  pval_mat[0] = -999; /** for (0,0) there is no p-value defined so I give it -999. **/

  if (fn_pval_mat && *fn_pval_mat)
    in_pval_mat = fopen(fn_pval_mat, "r");
  else error(E_FOPEN, fn_pval_mat);

  while (fscanf(in_pval_mat, "%d%d%lf", &n1, &n2, &curr_pval) != EOF) {
    pval_mat[n1+n2*(cases_cnt+1)] = curr_pval;
    if (n1 > cases_cnt || n2 > control_cnt) error(E_PVAL_MAT);  /** **/
  }

  fclose(in_pval_mat);
  MSG(fprintf(stderr, "done. %d grid points read.\n", n_pvals));



  /* --- create a frequent pattern tree --- */
  MSG(fprintf(stderr, "creating frequent pattern tree ... "));
  t = clock();                  /* start the timer */
  tas_sort(taset, heap);        /* sort the transactions */
  fptree = fpt_create(taset);   /* create a frequent pattern tree */
  if (!fptree) error(E_NOMEM);  /* to represent the transactions */
  tas_delete(taset, 0);         /* delete the transaction set */
  taset = NULL;                 /* and print a success message */
  MSG(fprintf(stderr, "done [%.2fs].\n", SEC_SINCE(t)));
 
  

  /* --- find frequent item sets --- */
  t = clock();                  /* start the timer */
  if (fn_out && *fn_out)        /* if an output file name is given, */
    out = fopen(fn_out, "w");   /* open the output file */
  else {                        /* if no output file name is given, */
    out = stdout; fn_out = "<stdout>"; }    /* write to std. output */
  MSG(fprintf(stderr, "writing %s ... ", fn_out));
  if (!out)   error(E_FOPEN, fn_out);
  isfmt = isf_create(itemset, flags & OF_SCAN);
  if (!isfmt) error(E_NOMEM);   /* create an item set formatter */
  k = fpt_search(fptree, (int)supp, min, max, mode, _report, NULL);
  if (k < 0)  error(E_NOMEM);   /* search for frequent item sets */
  if (fflush(out) != 0) error(E_FWRITE, fn_out);
  if (out != stdout) fclose(out);
  out = NULL;                   /* close the output file */
  MSG(fprintf(stderr, "[%d set(s)] done ", pattern_cnt)); /** **/
  MSG(fprintf(stderr, "[%.2fs].\n", SEC_SINCE(t)));

  /* --- clean up --- */
  #ifndef NDEBUG                /* if this is a debug version */
  isf_delete(isfmt);            /* delete the item set formatter, */
  if (isevl) ise_delete(isevl); /* the item set evaluator, */
  fpt_delete(fptree);           /* the frequent pattern tree, */
  is_delete(itemset);           /* and the item set */
  #endif
  #ifdef STORAGE                /* if storage debugging */
  showmem("at end of program"); /* check memory usage */
  #endif

  /** free(perm_label);**/ /** **/
  free(pval_mat);  /** **/

  return 0;                     /* return 'ok' */
}  /* main() */
