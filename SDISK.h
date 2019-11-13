#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<vector>

using namespace std;

#ifndef SDISK_H
#define SDISK_H

class sdisk{
private:
	string diskname;
	int numberofblocks;
	int blocksize;
public:
	sdisk(string dn, int nb, int bs);
	int getblock(int blocknumber, string& buffer);
	int putblock(int blocknumber, string buffer);
	int getnumberofblocks();
	int getblocksize();
	vector<string> block(string s, int b);
};


#endif
