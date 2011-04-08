/* fpt.c (release mode)
 *
 * Use threshold for finding large itemsets with supports >= the threshold.
 * This is the implementation using the FP-tree structure according to the paper:
 * 	Jiawei Han, Yiwen Yin: Mining Frequent Patterns without Candidate Generation,
 * 	ACM SIGMOD 2000, pages 1-12.
 *
 * Program Input:
 *	A configuration file consisting of 6 parameters
 *	1. User specified maximum size of itemset to be mined
 *	   If this value is not larger than zero or
 *	   is greater than the greatest transaction size in the DB,
 *	   then it will be set to the greatest transaction size.
 *	2. Normalized support threshold, range: (0, 1]
 *	3. Total number of different items in the DB
 *	4. Total number of transactions in the DB
 *	5. Data file name
 *	6. Result file name for storing the large itemsets
 *
 * Program Output:
 *	The frequent itemsets are displayed from small to large sizes.
 *	For each size, the itemsets are sorted in descending order of their supports.
 *	The support of each large itemset will also be displayed (enclosed by a bracket).
 *	It will output to both screen (standard output) and the result file.
 *
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "resource.h"
#include "times.h"
#include <unistd.h>


/***** Data Structure *****/
/* Description:
 *	Each node of an FP-tree is represented by a 'FPnode' structure.
 *	Each node contains an item ID, count value of the item, and
 *	node-link as stated in the paper.
 *
 */
typedef struct FPnode *FPTreeNode;	/* Pointer to a FP-tree node */

typedef struct Childnode *childLink;	/* Pointer to children of a FP-tree node */

/*
 * Children of a FP-tree node
 */
typedef struct Childnode {
	FPTreeNode node;	/* A child node of an item */
	childLink next;		/* Next child */
} ChildNode;

/*
 * A FP-tree node
 */
typedef struct FPnode {
        int item;		/* ID of the item.
				   Value of ID is within the range [0, m-1]
				   where m is the total number of different items in the database. */
        int count;		/* Value of count of the item.
				   This is the number of transactions containing items in the portion
				   of the path reaching this node. */
	int numPath;  		/* Number of leaf nodes in the subtree
			           rooted at this node.  It is used to
				   check whether there is only a single path
				   in the FPgrowth function. */
	FPTreeNode parent;	/* Pointer to parent node */
        childLink children;	/* Pointer to children */
        FPTreeNode hlink;	/* Horizontal link to next node with same item */
} FPNode;


/*
 * A list to store large itemsets in descending order of their supports.
 * It stores all the itemsets of supports >= threshold.
 */
typedef struct Itemsetnode *LargeItemPtr;
typedef struct Itemsetnode {
	int support;
	int *itemset;
	LargeItemPtr next;
} ItemsetNode;

void FPgrowth(FPTreeNode T, FPTreeNode *headerTableLink, int headerSize, int *baseItems, int baseSize);

/***** Global Variables *****/
LargeItemPtr *largeItemset;	/* largeItemset[k-1] = array of large k-itemsets */
int *numLarge;			/* numLarge[k-1] = no. of large k-itemsets found. */
int *support1;			/* Support of 1-itemsets */
int *largeItem1;		/* 1-itemsets */

FPTreeNode root=NULL;		/* Initial FP-tree */
FPTreeNode *headerTableLink;	/* Corresponding header table */

int expectedK;			/* User input upper limit of itemset size to be mined */
int realK;			/* Actual upper limit of itemset size can be mined */
int threshold;			/* User input support threshold */
int numItem;			/* Number of items in the database */
int numTrans;			/* Number of transactions in the database */
char dataFile[100];		/* File name of the database */
char outFile[100];		/* File name to store the result of mining */


/******************************************************************************************
 * Function: destroyTree
 *
 * Description:
 *	Destroy the FPtree rooted by a node and free the allocated memory.
 *	For each tree node being visited, all the child nodes
 *	are destroyed in a recursive manner before the destroy of the node.
 *
 * Invoked from:
 * 	destroy()
 *
 * Input Parameter:
 *	node	-> Root of the tree/subtree to be destroyed.
 */
void destroyTree(FPTreeNode node)
{
 childLink temp1, temp2;

 if (node == NULL) return;

 temp1 = node->children;
 while(temp1 != NULL) {
	temp2 = temp1->next;
	destroyTree(temp1->node);
	free(temp1);
	temp1 = temp2;
 }

 free(node);

 return;
}


/******************************************************************************************
 * Function: destroy
 *
 * Description:
 *	Free memory of following variables.
 *	- largeItemset
 *	- numLarge
 *	- headerTableLink
 *	- root
 *
 * Invoked from:
 * 	main()
 *
 * Functions to be invoked:
 *	destroyTree()	-> Free memory from the FP-tree, root.
 *
 * Global variables (read only):
 *	- realK
 */
void destroy()
{
 LargeItemPtr aLargeItemset;
 int i;

 for (i=0; i < realK; i++) {
	aLargeItemset = largeItemset[i];
	while (aLargeItemset != NULL) {
		largeItemset[i] = largeItemset[i]->next;
		free(aLargeItemset->itemset);
		free(aLargeItemset);
		aLargeItemset = largeItemset[i];
	}
 }
 free(largeItemset);

 free(numLarge);

 free(headerTableLink);

 destroyTree(root);

 return;
}



/******************************************************************************************
 * Function: swap
 *
 * Description:
 *	Swap x-th element and i-th element of each of the
 *	two arrays, support[] and itemset[].
 *
 * Invoked from:
 *	q_sortD()
 *	q_sortA()
 *
 * Functions to be invoked: None
 *
 * Input Parameters:
 *	support	-> Corresponding supports of the items in itemset.
 *	itemset	-> Array of items.
 *	x, i	-> The two indexes for swapping.
 *
 * Global variable: None
 */
void swap(int *support, int *itemset, int x, int i)
{
 int temp;

 temp = support[x];
 support[x] = support[i];
 support[i] = temp;
 temp = itemset[x];
 itemset[x] = itemset[i];
 itemset[i] = temp;

 return;
}


/******************************************************************************************
 * Function: q_sortD
 *
 * Description:
 * 	Quick sort two arrays, support[] and the corresponding itemset[],
 *	in descending order of support[].
 *
 * Invoked from:
 *	pass1()
 *	genConditionalPatternTree()
 *	q_sortD()
 *
 * Functions to be invoked:
 *	swap()
 *	q_sortD()
 *
 * Input Parameters:
 *      low		-> lower bound index of the array to be sorted
 *      high		-> upper bound index of the array to be sorted
 *      size		-> size of the array
 *	length		-> length of an itemset
 *
 * In/Out Parameters:
 *      support[]	-> array to be sorted
 *      itemset[]	-> array to be sorted
 */
void q_sortD(int *support, int *itemset, int low,int high, int size)
{
 int pass;
 int highptr=high++;     /* highptr records the last element */
 /* the first element in list is always served as the pivot */
 int pivot=low;

 if(low>=highptr) return;
 do {
	/* Find out, from the head of support[],
	 * the 1st element value not larger than the pivot's
	 */
	pass=1;
	while(pass==1) {
		if(++low<size) {
			if(support[low] > support[pivot])
				pass=1;
			else pass=0;
		} else pass=0;
	}

	/* Find out, from the tail of support[],
	 * the 1st element value not smaller than the pivot's
	 */
	pass=1;
	while(pass==1) {
		if(high-->0) {
			if(support[high] < support[pivot])
				pass=1;
			else pass=0;
		} else pass=0;
	}

	/* swap elements pointed by low pointer & high pointer */
	if(low<high)
		swap(support, itemset, low, high);
 } while(low<=high);

 swap(support, itemset, pivot, high);

 /* divide list into two for further sorting */
 q_sortD(support, itemset, pivot, high-1, size);
 q_sortD(support, itemset, high+1, highptr, size);

 return;
}


/******************************************************************************************
 * Function: q_sortA
 *
 * Description:
 * 	Quick sort two arrays, indexList[] and the corresponding freqItemP[],
 *	in ascending order of indexList[].
 *
 * Invoked from:
 *	buildTree()
 *	buildConTree()
 *	q_sortA()
 *
 * Functions to be invoked:
 *	swap()
 *	q_sortA()
 *
 * Input Parameters:
 *      low		-> lower bound index of the array to be sorted
 *      high		-> upper bound index of the array to be sorted
 *      size		-> size of the array
 *	length		-> length of an itemset
 *
 * In/Out Parameters:
 *      indexList[]	-> array to be sorted
 *      freqItemP[]	-> array to be sorted
 */
void q_sortA(int *indexList, int *freqItemP, int low, int high, int size)
{
 int pass;
 int highptr=high++;     /* highptr records the last element */
 /* the first element in list is always served as the pivot */
 int pivot=low;

 if(low>=highptr) return;
 do {
        /* Find out, from the head of indexList[],
	 * the 1st element value not smaller than the pivot's
	 */
        pass=1;
        while(pass==1) {
                if(++low<size) {
                        if(indexList[low] < indexList[pivot])
                                pass=1;
                        else pass=0;
                } else pass=0;
        }

        /* Find out, from the tail of indexList[],
	 * 1st element value not larger than the pivot's
	 */
        pass=1;
        while(pass==1) {
                if(high-->0) {
                        if(indexList[high] > indexList[pivot])
                                pass=1;
                        else pass=0;
                } else pass=0;
        }

        /* swap elements pointed by low pointer & high pointer */
        if(low<high)
                swap(indexList, freqItemP, low, high);
 } while(low<=high);

 swap(indexList, freqItemP, pivot, high);

 /* divide list into two for further sorting */
 q_sortA(indexList, freqItemP, pivot, high-1, size);
 q_sortA(indexList, freqItemP, high+1, highptr, size);

 return;
}


/******************************************************************************************
 * Function: addToLargeList
 *
 * Description:
 *	Add a large itemset, i.e. an itemset of support >= threshold,
 *	to the large itemsets list.
 *
 * Invoked from:
 *	FPgrowth()
 *	combine()
 *
 * Functions to be invoked: None
 *
 * Input parameters:
 *	pattern[]	-> The large itemset to be inserted
 *	patternSupport	-> Support of the itemset
 *	index		-> Number of items in the itemset - 1
 *
 * Global variables:
 *	numLarge[]	-> Increment this counter by 1 to represent
 *			   the current number of large (index+1)-itemsets found
 */
void addToLargeList(int *pattern, int patternSupport, int index)
{
 LargeItemPtr aLargeItemset;
 LargeItemPtr aNode, previous=NULL;
 int i;

 /* Create a node to store the itemset */
 aLargeItemset = (LargeItemPtr) malloc (sizeof(ItemsetNode));
 if (aLargeItemset == NULL) {
	printf("out of memory\n");
	exit(1);
 }
 aLargeItemset->itemset = (int *) malloc (sizeof(int) * (index+1));
 if (aLargeItemset->itemset == NULL) {
	printf("out of memory\n");
	exit(1);
 }

 /* Store the support of the itemset */
 aLargeItemset->support = patternSupport;

 /* Store the items of the itemset */
 for (i=0; i <= index; i++) {
	aLargeItemset->itemset[i] = pattern[i];
 }

 aLargeItemset->next = NULL;

 /* Assign aNode to point to the head of the resulting list */
 aNode = largeItemset[index];

 /* Insert the itemset to the (index+1)-itemset resulting list.
  * There are 3 cases for the insertion:
  *	1. The resulting list is empty
  *		-- insert the itemset to the head of the list
  *	2. The itemset should be inserted somewhere between
  *	   the head and tail of the list
  *		-- locate the suitable position of the list according to its support
  *		-- insert the itemset
  *	3. The itemset's support is less than the supports of all the itemsets in the list
  *		-- append the itemset at the end of the list
  */
 if (aNode == NULL) {
	/* Case 1: The resulting list is empty */
	largeItemset[index] = aLargeItemset;
 } else {
 	while ((aNode != NULL) && (aNode->support > patternSupport)) {
		previous = aNode;
		aNode = aNode->next;
 	}

	/* Case 2: Insert between head and tail of the list */
 	if (previous != NULL) {
		previous->next = aLargeItemset;
		aLargeItemset->next = aNode;
	} else {

		/* Case 3: Append to the end of the list */
		aLargeItemset->next = largeItemset[index];
		largeItemset[index] = aLargeItemset;
	}
 }

 /* Update the counter for the number of large (index+1)-itemsets in the list */
 (numLarge[index])++;

 return;
}


/******************************************************************************************
 * Function: combine
 *
 * Description:
 *	Make all possible combinations of itemsets for a single path FP-tree.
 *	Any of the combinations is a large itemset.
 *
 * Invoked from:
 *	FPgrowth()
 *	combine()
 *
 * Functions to be invoked:
 *	addToLargeList()
 *	combine()
 *
 * Input parameters:
 *  	*itemList	-> Array to hold all items in the FP-tree path
 *  	*support	-> Counts of the items in the path (in *itemList)
 *  	*base		-> A large itemset discovered which will be used to combine
 *			   with additional items in *itemList to form more large itemsets
 *  	start		-> Position in *itemList where the base is formed
 *              	   from subset of the set of items in the prefix to this position,
 *			   and new large pattern will combine the base with
 *			   one additional element from the suffix to this position.
 *  	baseSize	-> Size of the base, i.e. number of items in base
 *
 */
void combine(int *itemList, int *support, int start, int itemListSize, int *base, int baseSize)
{
 int *pattern;
 int i, j;

 if (baseSize >= realK) return;

 if (start == itemListSize) return;

 /* Create an array of size (baseSize+1) to store any itemset formed from
  * the union of *base and
  * any item of *itemsetListSize from the position of start to the end
  */
 pattern = (int *) malloc (sizeof(int) * (baseSize+1));
 if (pattern == NULL) {
	printf("out of memory\n");
	exit(1);
 }

 /* Insert all the items in base[] to pattern[]
  */
 for (j=0; j < baseSize; j++)
	pattern[j] = base[j];

 for (i=start; i < itemListSize; i++) {

	/* Append an item, itemList[i], to pattern[]
	 */
	pattern[baseSize] = itemList[i];

	/* Insert pattern[] to the result list of large (baseSizes+1)-itemsets.
	 * Support of this itemset = support[i]
	 */
	addToLargeList(pattern , support[i], baseSize);

	/* Form pattern[] with
	 * any item in *itemListSize from position (i+1) to the end of itemListSize
	 */
	combine(itemList, support, i+1, itemListSize, pattern, baseSize+1);
 }

 free(pattern);

 return;
}


/******************************************************************************************
 * Function: insert_tree
 *
 * Description:
 *  	This function is to insert a frequent pattern
 *  	of a transaction to the FP-tree (or conditional FP-tree).
 *  	The frequent pattern consists of a list of the frequent 1-items
 *  	of a transaction, and is sorted according to the sorted order of the
 *  		1. frequent 1-items in function pass1(), if it is the initial FP-tree;
 *		2. frequent 1-items in the conditional pattern base, if it is a conditional FP-tree.
 *  	This function is recursively invoked and insert the (ptr+1)th item of the
 *  	frequent pattern in the (ptr+1)th round of recursion.
 *
 *	There are 3 cases to handle the insertion of an item:
 *	1. The tree or subtree being visited has no children.
 *		Create the first child and store the info of the item
 *		to this first child.
 *	2. The tree or subtree has no children that match the current item.
 *		Add a new child node to store the item.
 *	3. There is a match between the item and a child of the tree.
 *		Increment the count of the child, and visit the subtree of this child.
 *
 * Invoked from:
 *	buildTree()
 *	buildConTree()
 *	insertTree()
 *
 * Functions to be invoked:
 *	insertTree()
 *
 * Parameters:
 *  - freqItemP : The list of frequent items of the transaction.
 *                They are sorted according to the order of frequent 1-items.
 *  - indexList : 'indexList[i]' is the corresponding index of the header table
 *                that represents the item 'freqItemP[i]'.
 *  - count     : The initial value for the 'count' of the FP-tree node for the current
 *                freqItemP[i].
 *		  It is equal to 1 if the FP-tree is the initial one,
 *		  otherwiese it is equal to the support of the base of
 *		  this conditional FP-tree.
 *  - ptr       : Number of items in the frequent pattern inserted so far.
 *  - length    : Number of items in the frequent pattern.
 *  - T         : The current FP-tree/subtree being visited so far.
 *  - headerTableLink : Header table of the FP-tree.
 *  - path      : Number of new tree path (i.e. new leaf nodes) created so far for the insertions.
 */
void insert_tree(int *freqItemP, int *indexList, int count, int ptr, int length,
			FPTreeNode T, FPTreeNode *headerTableLink, int *path)
{
 childLink newNode;
 FPTreeNode hNode;
 FPTreeNode hPrevious;
 childLink previous;
 childLink aNode;

 /* If all the items have been inserted */
 if (ptr == length) return;

 /* Case 1 : If the current subtree has no children */
 if (T->children == NULL) {
	/* T has no children */

	/* Create child nodes for this node */
	newNode = (childLink) malloc (sizeof(ChildNode));
	if (newNode == NULL) {
		printf("out of memory\n");
		exit(1);
	}

	/* Create a first child to store the item */
	newNode->node = (FPTreeNode) malloc (sizeof(FPNode));
	if (newNode->node == NULL) {
		printf("out of memory\n");
		exit(1);
	}

	/* Store information of the item */
	newNode->node->item = freqItemP[ptr];
	newNode->node->count = count;
	newNode->node->numPath = 1;
	newNode->node->parent = T;
	newNode->node->children = NULL;
	newNode->node->hlink = NULL;
	newNode->next = NULL;
	T->children = newNode;

	/* Link the node to the header table */
	hNode = headerTableLink[indexList[ptr]];
	if (hNode == NULL) {
		/* Place the node at the front of the horizontal link for the item */
		headerTableLink[indexList[ptr]] = newNode->node;
	} else {
		/* Place the node at the end using the horizontal link */
		while (hNode != NULL) {
			hPrevious = hNode;
			hNode = hNode->hlink;
		}

		hPrevious->hlink = newNode->node;
	}

	/* Insert next item, freqItemP[ptr+1], to the tree */
	insert_tree(freqItemP, indexList, count, ptr+1, length, T->children->node, headerTableLink, path);
	T->numPath += *path;

 } else {
	aNode = T->children;
	while ((aNode != NULL) && (aNode->node->item != freqItemP[ptr])) {
		previous = aNode;
		aNode = aNode->next;
	}

	if (aNode == NULL) {
		/* Case 2: Create a new child for T */

		newNode = (childLink) malloc (sizeof(ChildNode));
		if (newNode == NULL) {
			printf("out of memory\n");
			exit(1);
		}
		newNode->node = (FPTreeNode) malloc (sizeof(FPNode));
		if (newNode->node == NULL) {
			printf("out of memory\n");
			exit(1);
		}

		/* Store information of the item */
		newNode->node->item = freqItemP[ptr];
		newNode->node->count = count;
		newNode->node->numPath = 1;
		newNode->node->parent = T;
		newNode->node->children = NULL;
		newNode->node->hlink = NULL;
		newNode->next = NULL;
		previous->next = newNode;

		/* Link the node to the header table */
		hNode = headerTableLink[indexList[ptr]];
		if (hNode == NULL) {
			/* Place the node at the front of the horizontal link for the item */
			headerTableLink[indexList[ptr]] = newNode->node;
		} else {
			/* Place the node at the end using the horizontal link */
			while (hNode != NULL) {
				hPrevious = hNode;
				hNode = hNode->hlink;
			}
			hPrevious->hlink = newNode->node;
		}

		/* Insert next item, freqItemP[ptr+1], to the tree */
		insert_tree(freqItemP, indexList, count, ptr+1, length, newNode->node, headerTableLink, path);

		(*path)++;
		T->numPath += *path;

	} else {
		/* Case 3: Match an existing child of T */

		/* Increment the count */
		aNode->node->count += count;

		/* Insert next item, freqItemP[ptr+1], to the tree */
		insert_tree(freqItemP, indexList, count, ptr+1, length, aNode->node, headerTableLink, path);

		T->numPath += *path;
	}
 }

 return;
}


/******************************************************************************************
 * Function: buildConTree
 *
 * Description:
 *	Build a conditional FP-tree and the corresponding header table from
 *	a conditional pattern base.
 *
 * Invoked from:
 *	genConditionalPatternTree()
 *
 * Functions to be invoked:
 *	q_sortA()
 *	insert_tree()
 *
 * Parameters:
 *	*conRoot	-> Root of the conditional FP-tree
 *	**conHeader	-> Conditional header table for all the frequent items.
 *	conHeaderSize	-> Number of items (frequent) in the conditional header table
 *	*conLargeItem	-> Stores all the frequent items of the conditional pattern base
 *	*conLargeItemSupprt	-> The conditional support counts of the conditional items
 *				   "Conditional support" of an item is
 *				    the number of itemsets containing the item and the base items.
 *	T		-> The parent conditional FP-tree
 *	headerTable	-> The parent header table
 *	baseIndex	-> The index position of the parent header table that refer to the item
 *			   contained in the base of this conditional FP-tree
 *	headerSize	-> The number of items in the parent header table
 */
void buildConTree(FPTreeNode *conRoot, FPTreeNode **conHeader, int conHeaderSize, int *conLargeItem,
		int *conLargeItemSupport, FPTreeNode T, FPTreeNode *headerTable, int baseIndex, int headerSize)
{
 FPTreeNode aNode;
 FPTreeNode ancestorNode;
 int *freqItemP;	/* Holds all the frequent items of a transaction of the conditional pattern base */
 int *indexList;	/* Holds the index position of the parent header table for the corresponding item
			   in freqItemP[] */
 int path;
 int count;
 int i;

 /* create conditional header table
  */
 *conHeader = (FPTreeNode *) malloc (sizeof(FPTreeNode) * conHeaderSize);
 if (*conHeader == NULL) {
        printf("out of memory\n");
        exit(1);
 }
 for (i=0; i < conHeaderSize; i++)
        (*conHeader)[i] = NULL;

 /* create root of the conditional FP-tree
  */
 (*conRoot) = (FPTreeNode) malloc (sizeof(FPNode));
 if (*conRoot == NULL) {
        printf("out of memory\n");
        exit(1);
 }

 /* Initialize the root of the conditional FP-tree
  */
 (*conRoot)->numPath = 1;
 (*conRoot)->parent = NULL;
 (*conRoot)->children = NULL;
 (*conRoot)->hlink = NULL;

 freqItemP = (int *) malloc (sizeof(int) * conHeaderSize);
 if (freqItemP == NULL) {
        printf("out of memory\n");
        exit(1);
 }

 indexList = (int *) malloc (sizeof(int) * conHeaderSize);
 if (indexList == NULL) {
        printf("out of memory\n");
        exit(1);
 }

 /* Set aNode to the first node of the parent FP-tree
  * that contains the base item.
  */
 aNode = headerTable[baseIndex];

 /* Visit each path from the base item to the root
  * of the parent FP-tree and
  * extract all the frequent items of the conditional pattern base.
  * Sort this items in descending order of their frequency and
  * insert them to the conditional FP-tree.
  */
 while (aNode != NULL) {
	ancestorNode = aNode->parent;
	count = 0;

	/* Identify the frequent items in each path from the
	 * node containing the item in the base to the root.
	 * Each of such path is just like a transaction of the
	 * conditional pattern base.
	 * This identification can be done because
	 * conLargeItem[] contains all the frequent items
	 * of the conditional pattern base.
	 * Store the frequent items to freqItemP[], and
	 * the corresponding index position of the
	 * conditional header table to indexList[].
	 */
	while (ancestorNode != T) {
		for (i=0; i < conHeaderSize; i++) {
			if (ancestorNode->item == conLargeItem[i]) {
				freqItemP[count] = ancestorNode->item;
				indexList[count] = i;
				count++;
				break;
			}
		}
		ancestorNode = ancestorNode->parent;
	}

	/* Sort the frequent items in this path in ascending order
	 * of indexList[], i.e. sort in descending order of the
	 * frequency of the items in the conditional pattern base.
	 */
	q_sortA(indexList, freqItemP, 0, count-1, count);

	path = 0;

	/* Insert the frequent items to the conditional FP-tree.
	 */
	insert_tree(&(freqItemP[0]), &(indexList[0]), aNode->count, 0, count, *conRoot, *conHeader, &path);

	aNode = aNode->hlink;
 }

 free(freqItemP);
 free(indexList);

 return;
}


/******************************************************************************************
 * Function: genConditionalPatternTree
 *
 * Description:
 *	-- Form the conditional pattern base of each item in the header table.
 *	-- Build the conditional FP-tree for the item.
 *	-- Perfrom FPgrowth() for the conditional FP-tree.
 *
 * Parameters (In):
 *	pattern[]		-> Base items for the conditional FP-tree.
 *	baseSize		-> Number of base items -1.
 *	patternSupport		-> Support of the base items (it is a large itemset).
 *	T			-> FP-tree.
 *	headerIndex		-> Index position in the header table refering
 *				   the base item for the conditional FP-tree.
 *	headerSize		-> Number of items in the header table.
 *	*headerTableLink	-> Header table of the FP-tree (T).
 *
 * Invoked from:
 *	FPgrowth()
 *
 * Functions to be invoked:
 *	q_sortD()
 *	buildConTree()
 *	FPgrowth()
 *
 * Parameters (Out):
 *	conLargeItem[]		-> To hold frequent items in the conditional pattern base.
 *	conLargeItemSupport[]	-> To hold "conditional support" of each items in the conditional pattern base.
 *				   "Conditional support" of an item is
 *				   the number of itemsets containing the item and the base items.
 *
 * Global Variables (read only):
 *	threshold		-> Support threshold
 */
void genConditionalPatternTree(int *pattern, int baseSize, int patternSupport,
				int *conLargeItem, int *conLargeItemSupport, FPTreeNode T,
				int headerIndex, int headerSize, FPTreeNode *headerTableLink)
{
 int conHeaderSize;
 FPTreeNode *conHeader;
 FPTreeNode conRoot;
 FPTreeNode aNode, ancestorNode;
 int j;

 for (j=0; j < headerSize; j++)
	conLargeItemSupport[j] = 0;

 aNode = headerTableLink[headerIndex];
 conHeaderSize = 0;

 /* Find all the frequent items in the conditional pattern base.
  * -- Visit, in bottom-up manner, all the ancestor nodes of all the nodes containing this item.
  * -- Count the "conditional supports" of the items in the ancestor nodes
  *    (i.e. items in conditional pattern base), and store the values to conLargeSupport[].
  *    "Conditional support" of an item is
  *    the number of itemsets containing the item and the base items.
  * -- Items in the ancestor nodes having conditional supports >= threshold are added to
  *    conLargeItem[].
  */
 while (aNode != NULL) {
	ancestorNode = aNode->parent;

	while (ancestorNode != T) {

		for (j=0; j < headerSize; j++) {
			if (ancestorNode->item == headerTableLink[j]->item) {

				/* Increment the conditional support count
				 * for this ancestor item
				 */
				conLargeItemSupport[j] += aNode->count;

				if ((conLargeItemSupport[j] >= threshold) &&
				   (conLargeItemSupport[j] - aNode->count <
					threshold)) {

					/* Add the ancestor item to the conditional pattern base,
					 * and update the number of items in the
					 * conditional header table
					 */
					conLargeItem[j] = ancestorNode->item;
					conHeaderSize++;
				}
				break;
			}
		}
		ancestorNode = ancestorNode->parent;
	}

	/* Next node of the FP-tree containing the base item
	 */
	aNode = aNode->hlink;
 }

 /* Sort the items in the conditional pattern base in descending order of their
  * conditional support count
  */
 q_sortD(conLargeItemSupport, conLargeItem, 0, headerSize-1, headerSize);


 /* Generate the conditional FP-tree and mine recursively
  */
 if (conHeaderSize > 0) {

	/* Build conditional FP-tree
	 */
	buildConTree(&conRoot, &conHeader, conHeaderSize, conLargeItem, conLargeItemSupport,
			T, headerTableLink, headerIndex, headerSize);

	/* Mining
	 */
	FPgrowth(conRoot, conHeader, conHeaderSize, pattern, baseSize+1);

 	free(conHeader);
 	destroyTree(conRoot);
 }

 return;
}


/******************************************************************************************
 * Function: FPgrowth
 *
 * Description:
 *	Perform the FP-growth algorithm.
 *
 *	There are 2 cases for the FP-tree:
 *	Case 1:
 *		The tree consists of a single path only.
 * 		-- Form any combination of items in the path to
 *		   generate large itemsets containing the base items of this FP-tree.
 *	Case 2:
 *		The tree consists of more than one path.
 *		-- Visit the header table in a top-down manner, i.e. visit largest item first.
 *		-- Form the conditional pattern base of each item in the header table.
 *		-- Build the conditional FP-tree for the item.
 *		-- Perfrom FPgrowth() for the conditional FP-tree.
 *
 * Invoked from:
 *	main()
 *	genConditionalPatternTree()
 *
 * Functions to be invoked:
 *	combine()
 *	addLargeList()
 *	genConditionalPatternTree()
 *
 * Parameters (In):
 *	T			-> A FP-tree
 *	*headerTableLink	-> Header table
 *	headSize		-> Number of items in the header table
 *	*baseItems		-> The base items for this FP-tree
 *	baseSize		-> The number of base items
 *
 * Global Variables:
 *	threshold		-> Support threshold
 */
void FPgrowth(FPTreeNode T, FPTreeNode *headerTableLink, int headerSize, int *baseItems, int baseSize)
{
 int count;
 int i, j;
 int *pattern;
 int patternSupport;
 FPTreeNode aNode = NULL;
 int *conLargeItem;
 int *conLargeItemSupport;


 if (baseSize >= realK) return;
 if (T == NULL) return;

 /* Create array, conLargeItem, to store the items in the header table;
  * and also an array, conLargeItemSupport, to store the corresponding count value
  */
 conLargeItem = (int *) malloc (sizeof(int) * headerSize);
 conLargeItemSupport = (int *) malloc (sizeof(int) * headerSize);
 if ((conLargeItem == NULL) || (conLargeItemSupport == NULL)) {
	printf("out of memory\n");
	exit(1);
 }

 if (T->numPath == 1) {

	/* Case 1: Single Path */

	count = 0;
	if (T->children != NULL) aNode = T->children->node;

	/* Visit the path in top-down manner, and store the items and count values
	 */
	while (aNode != NULL) {
		conLargeItem[count] = aNode->item;
		conLargeItemSupport[count] = aNode->count;
		count++;
		if (aNode->children != NULL)
			aNode = aNode->children->node;
		else	aNode = NULL;
	}

	/* Form any combination of items in the path to
	 * generate large itemsets containing the base items stored in 'baseItems'
	 */
	combine(conLargeItem, conLargeItemSupport, 0, count, baseItems, baseSize);

	free(conLargeItem);
	free(conLargeItemSupport);

 } else {
	/* Multiple Path */

	/* Create an array to store the base items for a conditional FP-tree.
	 * Size of the base should be (baseSize + 1).
	 */
	pattern = (int *) malloc (sizeof(int) * (baseSize + 1));
	if (pattern == NULL) {
		printf("out of memory\n");
		exit(1);
	}

 	/* Visit the header table in a top-down manner.
	 * -- Find the conditional pattern base for each base item in the header table.
	 * -- Find the frequent items of the conditional pattern base of the base item.
	 * -- Build conditional FP-tree for the base item.
	 * -- Recursively mine the conditional FP-tree.
	 */
	for (i=0; i < headerSize; i++) {

		/* Add the item to the base of its conditional FP-tree
		 */
		pattern[0] = headerTableLink[i]->item;

		/* Add the base of T to the base of the conditional FP-tree
		 */
		for (j=0; j < baseSize; j++) {
			pattern[j+1] = baseItems[j];
		}

		aNode = headerTableLink[i];
		patternSupport = 0;

		/* Count the support of the base of the conditional FP-tree
		 */
		while (aNode != NULL) {
			patternSupport += aNode->count;
			aNode = aNode->hlink;
		}

		/* Add the itemset formed by the base items
		 * to the resulting list because it must be large.
		 */
		addToLargeList(pattern, patternSupport, baseSize);

		/* Find frquent items, build conditional FP-tree and perform mining.
		 */
		genConditionalPatternTree(pattern, baseSize, patternSupport,
				conLargeItem, conLargeItemSupport, T,
				i, headerSize, headerTableLink);
	}

	free(pattern);
	free(conLargeItem);
	free(conLargeItemSupport);
 }

 return;
}



/******************************************************************************************
 * Function: pass1()
 *
 * Description:
 *	Scan the DB and find the support of each item.
 *	Find the large 1-itemsets according to the support threshold.
 *
 * Invoked from:
 *	main()
 *
 * Functions to be invoked:
 *	q_sortD()
 *
 * Global variables:
 *	largeItem1[]	-> Array to store 1-itemsets
 *	support1[]	-> Support[i] = support of the 1-itemset stored in largeItem[i]
 *	largeItemset[]	-> largeItemset[i] = resulting list for large (i+1)-itemsets
 *	realK		-> Maximum size of itemset to be mined
 *	numLarge[]	-> numLarge[i] = Number of large (i+1)-itemsets discovered so far
 *
 * Global variables (read only):
 *	numTrans	-> number of transactions in the database
 *	expectedK	-> User specified maximum size of itemset to be mined
 *	dataFile	-> Database file
 *
 */
void pass1()
{
 int transSize;
 int item;
 int maxSize=0;
 FILE *fp;
 int i, j;

 /* Initialize the 1-itemsets list and support list */
 support1 = (int *) malloc (sizeof(int) * numItem);
 largeItem1 = (int *) malloc (sizeof(int) * numItem);
 if ((support1 == NULL) || (largeItem1 == NULL)) {
	printf("out of memory\n");
	exit(1);
 }

 for (i=0; i < numItem; i++) {
	support1[i] = 0;
	largeItem1[i] = i;
 }

 /* scan DB to count the frequency of each item */

 if ((fp = fopen(dataFile, "r")) == NULL) {
        printf("Can't open data file, %s.\n", dataFile);
        exit(1);
 }

 /* Scan each transaction of the DB */
 for (i=0; i < numTrans; i++) {

	/* Read the transaction size */
	fscanf(fp, "%d", &transSize);

	/* Mark down the largest transaction size found so far */
	if (transSize > maxSize)
		maxSize = transSize;

	/* Read the items in the transaction */
	for (j=0; j < transSize; j++) {
		fscanf(fp, "%d", &item);
		support1[item]++;
	}
 }
 fclose(fp);

 /* Determine the upper limit of itemset size to be mined according to DB and user input.
  * If the user specified maximum itemset size (expectedK) is greater than
  * the largest transaction size (maxSize) in the database, or  exptectedK <= 0,
  * then set  realK = maxSize;
  * otherwise  realK = expectedK
  */
 realK = expectedK;
 if ((maxSize < expectedK) || (expectedK <= 0))
	realK = maxSize;
 printf("max transaction sizes = %d\n", maxSize);
 printf("max itemset size (K_max) to be mined  = %d\n", realK);

 /* Initialize large k-itemset resulting list and corresponding support list */
 largeItemset = (LargeItemPtr *) malloc (sizeof(LargeItemPtr) * realK);
 numLarge = (int *) malloc (sizeof(int) * realK);

 if ((largeItemset == NULL) || (numLarge == NULL)) {
	printf("out of memory\n");
	exit(1);
 }

 for (i=0; i < realK; i++)  {
	largeItemset[i] = NULL;
	numLarge[i] = 0;
 }

 /* Sort the supports of 1-itemsets in descending order */
 q_sortD(&(support1[0]), largeItem1, 0, numItem-1, numItem);

 /*
 for (i=0; i < numItem; i++)
 	printf("%d[%d] ", largeItem1[i], support1[i]);
 printf("\n");
 */

 numLarge[0] = 0;
 while ((numLarge[0] < numItem) && (support1[numLarge[0]] >= threshold))
	(numLarge[0])++;

 printf("\nNo. of large 1-itemsets (numLarge[0]) = %d\n", numLarge[0]);

 return;
}


/******************************************************************************************
 * Function: buildTree()
 *
 * Description:
 *	Build the initial FP-tree.
 *
 * Invoked from:
 *	main()
 *
 * Functions to be invoked:
 *	insert_tree()
 *	q_sortA()
 *
 * Global variables:
 *	root		-> Pointer to the root of this initial FP-tree
 *	headerTableLink	-> Header table for this initial FP-tree
 *
 * Global variables (read only):
 *	numLarge[]	-> Large k-itemsets resulting list for k = 1 to realK
 */
void buildTree()
{
 int *freqItemP;	/* Store frequent items of a transaction */
 int *indexList;	/* indexList[i] = the index position in the large 1-item list storing freqItemP[i] */
 int count;		/* Number of frequent items in a transaction */
 FILE *fp;		/* Pointer to the database file */
 int transSize;		/* Transaction size */
 int item;		/* An item in the transaction */
 int i, j, m;
 int path;		/* Number of new tree paths (i.e. new leaf nodes) created so far */


 /* Create header table */
 headerTableLink = (FPTreeNode *) malloc (sizeof(FPTreeNode) * numLarge[0]);
 if (headerTableLink == NULL) {
	printf("out of memory\n");
	exit(1);
 }
 for (i=0; i < numLarge[0]; i++)
	headerTableLink[i] = NULL;

 /* Create root of the FP-tree */
 root = (FPTreeNode) malloc (sizeof(FPNode));
 if (root == NULL) {
	printf("out of memory\n");
	exit(1);
 }

 /* Initialize the root node */
 root->numPath = 1;
 root->parent = NULL;
 root->children = NULL;
 root->hlink = NULL;

 /* Create freqItemP to store frequent items of a transaction */
 freqItemP = (int *) malloc (sizeof(int) * numItem);
 if (freqItemP == NULL) {
	printf("out of memory\n");
	exit(1);
 }

 indexList = (int *) malloc (sizeof(int) * numItem);
 if (indexList == NULL) {
	printf("out of memory\n");
	exit(1);
 }


 /* scan DB and insert frequent items into the FP-tree */
 if ((fp = fopen(dataFile, "r")) == NULL) {
        printf("Can't open data file, %s.\n", dataFile);
        exit(1);
 }


 for (i=0; i < numTrans; i++) {

	/* Read the transaction size */
	fscanf(fp, "%d", &transSize);

	count = 0;
 	path = 0;

	for (j=0; j < transSize; j++) {

		/* Read a transaction item */
		fscanf(fp, "%d", &item);

		/* Store the item to the frequent list, freqItemP,
		 * if it is a large 1-item.
		 */
		for (m=0; m < numLarge[0]; m++) {
			if (item == largeItem1[m]) {
				/* Store the item */
				freqItemP[count] = item;
				/* Store the position in the large 1-itemset list storing this item */
				indexList[count] = m;
				count++;
				break;
			}
		}
	}

	/* Sort the items in the frequent item list in ascending order of indexList,
	 * i.e. sort according to the order of the large 1-itemset list
	 */
	q_sortA(indexList, freqItemP, 0, count-1, count);

	/* Insert the frequent patterns of this transaction to the FP-tree. */
	insert_tree(&(freqItemP[0]), &(indexList[0]), 1, 0, count, root, headerTableLink, &path);
 }
 fclose(fp);

 free(freqItemP);
 free(indexList);
 free(largeItem1);
 free(support1);

 return;
}


/******************************************************************************************
 * Function: displayResult
 *
 * Description:
 *	Output the large itemsets of all sizes
 *	to both screen and result file.
 *
 * Invoked from:
 *	main()
 *
 * Functions to be invoked: None
 *
 * Global variables (read only):
 *	realK		-> maximum size of itemsets
 *	numLarge[]	-> numLarge[i] = large (i+1)-itemsets' resulting list
 *	outFile		-> result file to store the output result
 */
void displayResult()
{
 LargeItemPtr aLargeItemset;
 FILE *fp;
 int i, j;

 if ((fp = fopen(outFile, "w")) == NULL) {
        printf("Can't open data file, %s.\n", outFile);
        exit(1);
 }
 fprintf(fp, "K_{max} = %d\n\n", realK);

 for (i=0; i < realK; i++) {

	fprintf(fp, "%d Large %d-itemsets:\n", numLarge[i], i+1);

	if (numLarge[i] == 0) break;

	printf("\n%d Large %d-itemsets[support]:\n", numLarge[i], i+1);

	/* Visit the large (i+1)-itemsets' resulting list */
	aLargeItemset = largeItemset[i];

	/* print out the large (i+1)-itemsets */
	while (aLargeItemset != NULL) {

		/* print an (i+1)-itemset */
		for (j=0; j <= i; j++) {
			printf("%d ", aLargeItemset->itemset[j]);
			fprintf(fp, "%d ", aLargeItemset->itemset[j]);
		}

		/* print the support of the itemset */
		printf("[%d]\n", aLargeItemset->support);
		fprintf(fp, "[%d]\n", aLargeItemset->support);

		aLargeItemset = aLargeItemset->next;
	}
	printf("\n");
	fprintf(fp, "\n");
 }

 fclose(fp);

 return;
}


/******************************************************************************************
 * Function: input
 *
 * Description:
 *	Read the input parameters from the configuration file.
 *
 * Invoked from:
 *	main()
 *
 * Functions to be invoked: None
 *
 * Input parameters:
 *	*configFile	-> The configuration file
 *
 * Global variables:
 *	expectedK		-> User specified maximum size of itemset to be mined
 *	thresholdDecimal	-> Normalized support threshold, range: (0, 1]
 *	numItem			-> Total number of different items in the DB
 *	numTrans		-> Total number of transactions in the DB
 *	dataFile		-> Data file
 *	outFile			-> Result file for storing the large itemsets
 */
void input(char *configFile)
{
 FILE *fp;
 float thresholdDecimal;

 if ((fp = fopen(configFile, "r")) == NULL) {
        printf("Can't open config. file, %s.\n", configFile);
        exit(1);
 }

 fscanf(fp, "%d %f %d %d", &expectedK, &thresholdDecimal, &numItem, &numTrans);
 fscanf(fp, "%s %s", dataFile, outFile);
 fclose(fp);

 printf("expectedK = %d\n", expectedK);
 printf("thresholdDecimal = %f\n", thresholdDecimal);
 printf("numItem = %d\n", numItem);
 printf("numTrans = %d\n", numTrans);
 printf("dataFile = %s\n", dataFile);
 printf("outFile = %s\n\n", outFile);
 threshold = thresholdDecimal * numTrans;
 if (threshold == 0) threshold = 1;
 printf("threshold = %d\n", threshold);

 return;
}


/******************************************************************************************
 * Function: main
 *
 * Description:
 *	This function reads the config. file for six input parameters,
 *	finds the frequent 1-itemsets, builds the initial FP-tree
 *	using the frequent 1-itemsets and
 *	peforms the FP-growth algorithm of the paper.
 *	It measure both CPU and I/O time for build tree and mining.
 *
 * Functions to be invoked:
 *	input()		-> Read config. file
 *	pass1()		-> Scan DB and find frquent 1-itemsets
 *	buildTree()	-> Build the initial FP-tree
 *	FPgrowth()	-> Start mining
 *
 * Parameters:
 *	Config. file name
 */
void main(int argc, char *argv[])
{
 float  userTime, sysTime;
 struct rusage myTime1, myTime2, myTime3;
 int headerTableSize;

 /* Usage ------------------------------------------*/
 printf("\nFP-tree: Mining large itemsets using user support threshold\n\n");
 if (argc != 2) {
        printf("Usage: %s <config. file>\n\n", argv[0]);
	printf("Content of config. file:\n");
	printf("  Line 1: Upper limit of large itemsets size to be mined\n");
	printf("  Line 2: Support threshold (normalized to [0, 1])\n");
	printf("  Line 3: No. of different items in the DB\n");
	printf("  Line 4: No. of transactions in the DB\n");
	printf("  Line 5: File name of the DB\n");
	printf("  Line 6: Result file name to store the large itemsets\n\n");
        exit(1);
 }

 /* read input parameters --------------------------*/
 printf("input\n");
 input(argv[1]);

 getrusage(RUSAGE_SELF,&myTime1);

 /* pass 1 : Mine the large 1-itemsets -------------*/
 printf("\npass1\n");
 pass1();

 /* Mine the large k-itemsets (k = 2 to realK) -----*/
 if (numLarge[0] > 0) {

	/* create FP-tree --------------------------*/
 	printf("\nbuildTree\n");
 	buildTree();

	getrusage(RUSAGE_SELF,&myTime2);

	/* mining frequent patterns ----------------*/
 	printf("\npassK\n");
	headerTableSize = numLarge[0];
	numLarge[0] = 0;
 	FPgrowth(root, headerTableLink, headerTableSize, NULL, 0);

 	getrusage(RUSAGE_SELF,&myTime3);

 	/* output result of large itemsets ---------*/
 	printf("\nresult\n");
 	displayResult();

 	/* output execution time ---------------------*/
 	printf("Build tree time:\n");
 	userTime =
       		((float) (myTime2.ru_utime.tv_sec  - myTime1.ru_utime.tv_sec)) +
       		((float) (myTime2.ru_utime.tv_usec - myTime1.ru_utime.tv_usec)) * 1e-6;
 	sysTime =
       		((float) (myTime2.ru_stime.tv_sec  - myTime1.ru_stime.tv_sec)) +
       		((float) (myTime2.ru_stime.tv_usec - myTime1.ru_stime.tv_usec)) * 1e-6;

 	printf("User time : %f seconds\n", userTime);
 	printf("System time : %f seconds\n\n", sysTime);

 	printf("FP-tree growth time:\n");
 	userTime =
       		((float) (myTime3.ru_utime.tv_sec  - myTime2.ru_utime.tv_sec)) +
       		((float) (myTime3.ru_utime.tv_usec - myTime2.ru_utime.tv_usec)) * 1e-6;
 	sysTime =
       		((float) (myTime3.ru_stime.tv_sec  - myTime2.ru_stime.tv_sec)) +
       		((float) (myTime3.ru_stime.tv_usec - myTime2.ru_stime.tv_usec)) * 1e-6;

 	printf("User time : %f seconds\n", userTime);
 	printf("System time : %f seconds\n\n", sysTime);

	printf("Total execution time:\n");
 	userTime =
       		((float) (myTime3.ru_utime.tv_sec  - myTime1.ru_utime.tv_sec)) +
       		((float) (myTime3.ru_utime.tv_usec - myTime1.ru_utime.tv_usec)) * 1e-6;
 	sysTime =
       		((float) (myTime3.ru_stime.tv_sec  - myTime1.ru_stime.tv_sec)) +
       		((float) (myTime3.ru_stime.tv_usec - myTime1.ru_stime.tv_usec)) * 1e-6;

 	printf("User time : %f seconds\n", userTime);
 	printf("System time : %f seconds\n", sysTime);
	printf("Total time: %f seconds\n\n", userTime + sysTime);

 }

 /* free memory ------------------------------------*/
 printf("\ndestroy\n");
 destroy();

 return;
}

