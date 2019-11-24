#include "filesys.h"

#ifndef TABLE_H
#define TABLE_H

class Table: public Filesys{
	private:
		string flatfile;
		string indexfile;
		int IndexSearch(string value);
	public:
		Table(string diskname, int bs, int nb);
		int Build_Table(string input_file);
		int Search(string value);
	
};

#endif