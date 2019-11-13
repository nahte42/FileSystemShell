#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include"SDISK.h"

using namespace std;

#ifndef FILESYS_H
#define FILESYS_H

class Filesys: public sdisk{
private:
	int rootsize;
	int fatsize;
	vector<string> filename;
	vector<int> firstblock;
	vector<int> fat;

public:
	Filesys(string dn, int nb, int bs);
	int fsclose();
	int fssynch();
	int newfile(string file);
	int rmfile(string file);
	int getfirstblock(string file);
	int addblock(string file, string block);
	int delblock(string file, int blocknumber);
	int readblock(string file, int blocknumber, string& buffer);
	int writeblock(string file, int blocknumber, string buffer);
	int nextblock(string file, int blocknumber);
	bool checkblock(string file, int blocknumber);
	vector<string> ls();
};
#endif
