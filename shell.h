#include "Table.h"

#ifndef SHELL_H
#define SHELL_H

class Shell : public Table{
public:
	Shell(string filename, int blocksize, int numberofblocks);
	~Shell(){fsclose();}
	int dir();
	int add(string file);
	int del(string file);
	int type(string file);
	int copy(string file1, string file2);	
};
#endif