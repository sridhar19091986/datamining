/*----------------------------------------------------------------------
  File    : Data.h
  Contents: data set management
  Author  : Bart Goethals
  Update  : 4/4/2003
  ----------------------------------------------------------------------*/

#include <stdio.h>

class Transaction
{
 public:
	
  Transaction(int l) : length(l) //整数数组
  {
	  t = new int[l];
  }
  Transaction(const Transaction &tr);//构造函数申明重载
  ~Transaction()
  {
	  delete t;
  }
	
  int length;
  int *t;
};

class Data
{
 public:
	
  Data(char *filename, int t=0);
  ~Data();
	
  Transaction *getNext();

 private:
	
  Transaction *getNextAs();
  Transaction *getNextAsFlat();//仅读取事务中项的序号
  Transaction *getNextAsQuest();
  Transaction *getNextBin();
	
  FILE *in;
  char *fn;
  int current;//控制读取事务时自动增长
  int type;
};
