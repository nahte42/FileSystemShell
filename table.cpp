#include "Table.h"

Table::Table(string dn, int bs, int nb):Filesys(dn,bs,nb){
	
	flatfile = "flatfile";
	indexfile = "indexfile";
	int check0 = newfile(flatfile);
	int check1 = newfile(indexfile);
	if(check1 == 1 && check0 == 1)
		Build_Table("records.txt");
}

int Table::Build_Table(string input_file){
	ifstream infile;
	infile.open(input_file.c_str());
	string record;
	int count = 0;
	getline(infile,record);
	vector<string> key;
	vector<string> iblock;
	
	ostringstream outstream;
	while(infile.good()){
		string pkey = record.substr(0,5);
		vector<string> oblock = block(record, getblocksize());
		int blockid = addblock(flatfile, oblock[0]);
		outstream<<pkey<<" "<<blockid<<" ";
		count++;
		
		if (count == 4){
			vector<string> o2block = block(outstream.str(), getblocksize());
			addblock(indexfile, o2block[0]);
			count = 0;
			outstream.str("");
		}
		getline(infile, record);
	}
	return 1;
	
}

int Table::Search(string value){
	int index = IndexSearch(value);
	string mb;
	if (index != -1){
		readblock(flatfile, index, mb);
		int stop = 0;
		while(mb[++stop]!= '#');
		cout<<mb.substr(0,stop)<<endl;
		return 1;
	}
	return -1;
}

int Table::IndexSearch(string value){
	vector<string> keys;
	vector<int> blockid;
	
	istringstream instream;
	int fb = getfirstblock(indexfile);
	//int nb = nextblock(indexfile, fb);
	string buffer = "";
	
	readblock(indexfile, fb, buffer);
	instream.str(buffer);
	while(instream.good()){
		string p;
		int b;
		instream >> p >> b;
		keys.push_back(p);
		blockid.push_back(b);
	}
	for(int i = 0; i < keys.size(); i++){
		if(keys[i] == value)
			return blockid[i];
	}
	keys.clear();
	blockid.clear();
	
	int next_block = nextblock(indexfile, fb);
	if(next_block == -1 || next_block == 0){
		cout<<"Record does not exist\n";
		return -1;
	}
	else{
		istringstream instream2;
		while(next_block!= -1){
			buffer = "";
			readblock(indexfile, next_block, buffer);
			instream2.str(buffer);
			for(int i = 0; i < 4; i++){
				string p;
				int b;
				instream2>>p>>b;
				keys.push_back(p);
				blockid.push_back(b);
			}
			for(int i = 0; i < keys.size(); i++){
				if(keys[i] == value)
					return blockid[i];
			}
			instream.str("");
			next_block =  nextblock(indexfile, next_block);
		}
	}
	
	cout<<"Record does not exist\n";
	
	return -1;
}