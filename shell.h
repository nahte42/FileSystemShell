#include "filesys.h"

#ifndef SHELL_H
#define SHELL_H

class Shell : public Filesys{
public:
	Shell(string filename, int blocksize, int numberofblocks);
	int dir();
	int add(string file);
	int del(string file);
	int type(string file);
	int copy(string file1, string file2);	
};
#endif