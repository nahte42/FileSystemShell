#include "shell.h"

Shell::Shell(string dn, int bs, int nb):Table(dn,bs,nb){}

int Shell::dir(){
	vector<string> flist = ls();
	for(int i = 0; i < flist.size(); i++){
		cout<<flist[i]<<endl;
	}
	return 1;
}

int Shell::add(string file){
	//cout<<"Fat[0] in shell: "<<getfatz()<<endl;
	vector<string> s_block;
	newfile(file);
	cout<<"Input data into file\n"; 
	string data = "";
	getline(cin, data);
	while(data != ""){
		s_block = block(data, getblocksize());
		for(int i = 0; i < s_block.size(); i++){
			addblock(file, s_block[i]);
		}
		getline(cin, data);
		s_block.clear();
	}
	
	return 1;
}

int Shell::del(string file){
	int code = getfirstblock(file);
	vector<int> blocks_del;
	if(code == -1){
		cout<<"File does not exist\n";
		return 0;
	}
	if(code == 0){
		delblock(file, code);
		rmfile(file);
		return 1;
	}else{
		int iblock = code;
		while(iblock != -1){
			blocks_del.push_back(iblock);
			iblock = nextblock(file, iblock);
			
		}
		for(int i = 0; i < blocks_del.size(); i++){
			delblock(file, blocks_del[i]);
		}
	}
	rmfile(file);
	return 1;
}

int Shell::type(string file){
	string buffer;
	int code = getfirstblock(file);
	if(code == -1){
		cout<<"File not found\n";
		return 0;		
	}
	int iblock = code;
	while(iblock != 0){
		string b;
		getblock(iblock, b);
		for(int i = 0; i < b.size(); i++){
			if(b[i] != '#'){
				cout<<b[i];
			}
		}
		cout<<endl;
		iblock = nextblock(file, iblock);
		
	}
	return 1;
}
int Shell::copy(string file1, string file2){
	int code1 = getfirstblock(file1);
	int code2 = getfirstblock(file2);
	
	if(code1 == -1){
		cout<<"File not found\n";
		return 0;
	}
	
	if(code2 != -1){
		cout<<"File already exists\n";
		return 0;
	}
	
	int code3 = newfile(file2);
	if(code3 == 0){
		cout<<"No available space\n";
		return 0;
	}
	
	int iblock = code1;
	while(iblock != 0){
		string b;
		getblock(iblock, b);
		int code4 = addblock(file2, b);
		if(code4 == -1){
			cout<<"No space left\n";
			del(file2);
			return 0;
		}
		iblock = nextblock(file1, iblock);
	}
	
	
	return 1;
}