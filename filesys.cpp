#include"filesys.h"
#include"SDISK.h"

Filesys::Filesys(string dn, int nb, int bs):sdisk(dn,nb,bs){
	string buffer;
	rootsize = getblocksize()/13;
	fatsize = ((4*getnumberofblocks())/getblocksize()) + 1;
	getblock(1, buffer);
	cout<<buffer[0]<<endl;
	//cout<<"File sys was called correctly: \n";
	
	if(buffer[0] == '#'){
		for(int i = 0; i < rootsize; i++){
			filename.push_back("xxxxxxxx");
			firstblock.push_back(0);
		}
		fat.push_back(2+fatsize);
		fat.push_back(-1);
		for(int i = 1; i < fatsize; i++){
			fat.push_back(-1);
		}
		for(int i = fatsize+2; i < getnumberofblocks(); i++){
			fat.push_back(i+1);
		}
		fat[fat.size()-1] = 0;
	}
	else{
		istringstream instream, instream2;
		instream.str(buffer);
		for(int i =0; i < rootsize; i++){
			string f; int b;
			instream >> f >> b;
			filename.push_back(f);
			firstblock.push_back(b);
		}
		buffer.clear();
		for(int i = 0; i < fatsize; i++){
			string b;
			getblock(2+i, b);
			buffer += b;
		}
		instream2.str(buffer);
		for(int i = 0; i < getnumberofblocks(); i++){
			int n;
			instream2>>n;
			fat.push_back(n);
		}
	}
	fssynch();
}

int Filesys::fsclose(){
	fssynch();
	return 0;
}

int Filesys::fssynch(){
	ostringstream fat_buffer, root_buffer;
	
	for(int i = 0; i < filename.size(); i++){
		root_buffer<<filename[i]<<" "<<firstblock[i]<<" ";
	}
	for(int i = 0; i < fat.size(); i++){
		fat_buffer<<fat[i]<<" ";
	}
	
	string buffer = fat_buffer.str();
	vector<string> b = block(buffer, getblocksize());
	
	for(int i = 0; i < b.size(); i++){
		putblock(2+i, b[i]);
	}
	buffer.clear();
	b.clear();
	
	buffer = root_buffer.str();
	b = block(buffer, getblocksize());
	
	for(int i = 0; i < b.size(); i++){
		putblock(1+i, b[i]);
	}
	
	return 0;
}

int Filesys::newfile(string file){
	for(int  i = 0; i < rootsize; i++){
		if(filename[i] == file){
			cout<<"File Already Exists:\n";
			return 0;
		}
	}
	for(int  i = 0; i < rootsize; i++){//cout<<"Here\n";
		if(filename[i] == "xxxxxxxx"){//cout<<"And Here\n";
			filename[i] = file;
			fssynch();
			return 1;
		}
	}
	return 0;
}

int Filesys::rmfile(string file){
	for(int  i = 0; i < rootsize; i++){
		if(filename[i] == file){
			if(firstblock[i] != 0){
				cout<<"File Not Empty:\n";
				return 0;
			}
			else{
				filename[i] = "xxxxxxxx";
				fssynch();
				return 1;
			}
		}
	}
	return 0;
}

int Filesys::getfirstblock(string file){
	for(int  i = 0; i < rootsize; i++){
		if(filename[i] == file){
			return firstblock[i];
		}
	}
	cout<<"File Does Not Exist:\n";	
	return -1;
}

/*int Filesys::addblock(string file, string block){
	int first = getfirstblock(file);
	int allocate = fat[0];
	if(allocate == 0) //no free blocks
		return 0;
	fat[0] = fat[fat[0]];
	fat[allocate] = 0 ;
	if(first == 0){
		for(int i = 0; i < rootsize; i++){
			if(filename[i] == file){
				firstblock[i] = allocate;
				fssynch();
				putblock(allocate, block);
				return allocate;
			}
		}	
	}
	else{
		int b = first;
		while(fat[b] != 0){
			b = fat[b];
		}
		fat[b] = allocate;
		fssynch();
		putblock(allocate, block);
		return allocate;
	}
	
	return 0;
}*/

int Filesys::addblock(string file, string block){
	int first = getfirstblock(file);
	int allocate = fat[0];
	bool filefound = false;
	
	if(allocate == 0){
		cout<<"Disk is full\n";
		return -1;
	}
	
	if(first == 0){
		for(int i = 0; i < filename.size(); i++){
			if(filename[i] == file){
				firstblock[i] = allocate;
				fat[0] = fat[allocate];
				fat[allocate] = 0;
				filefound = true;
				break;
			}
		}
	}
	if(filefound){
		int next = first;
		while(fat[next] != 0){
			next = fat[next];
			fat[next] = allocate;
			fat[0] =fat[allocate];
			fat[allocate] = 0;
		}
	}
	else{
		cout<<"File not found\n";
		return -1;
	}
	
	putblock(allocate, block);
	fssynch();
	
	return allocate;
}



int Filesys::delblock(string file, int blocknumber){
	if(!checkblock(file, blocknumber)){
		return 0;
	}
	int deallocate = blocknumber;
	if( blocknumber == getfirstblock(file))
		for(int i = 0; i < filename.size(); i++){
			if(file == filename[i]){
				firstblock[i] = fat[blocknumber];
				break;
			}
		}
	else{
		int iblock = getfirstblock(file);
		while(fat[iblock] != blocknumber){
			iblock = fat[iblock];
		}
		fat[iblock] = fat[blocknumber];
	}
	fat[blocknumber] = fat[0];
	fat[0] = blocknumber;
	fssynch();
	return 1;
}

int Filesys::readblock(string file, int blocknumber, string& buffer){
	if(checkblock(file, blocknumber)){
		return getblock(blocknumber, buffer);
	}
	return 0;
}

int Filesys::writeblock(string file, int blocknumber, string buffer){
	if(checkblock(file, blocknumber)){
		return putblock(blocknumber, buffer);
	}
	return 0;
}

int Filesys::nextblock(string file, int blocknumber){
	if(checkblock(file, blocknumber)){
		return fat[blocknumber];
	}
	return -1;
}

bool Filesys::checkblock(string file, int blocknumber){
	int next_block, first_blk = getfirstblock(file);
	bool found = false;

	next_block = first_blk;
	while(next_block != 0){
		if(next_block == blocknumber){
			return true;
		}
		next_block = fat[next_block];
	}
	
	return false;
}

vector<string> Filesys::ls(){
	vector<string> flist;
	for(int i = 0; i < filename.size(); i++){
		if(filename[i] != "xxxxxxxx")
			flist.push_back(filename[i]);
	}
	
	return flist;
}

