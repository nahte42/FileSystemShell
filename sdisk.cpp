#include "SDISK.h"

sdisk::sdisk(string dn, int nb, int bs){
	diskname = dn;
	numberofblocks = nb;
	blocksize = bs;
	
	fstream diskfile;
	diskfile.open(diskname.c_str(), ios::in);
	if(diskfile.good())
		diskfile.close();
	else{
		diskfile.open(diskname.c_str(), ios::out);
		for(int i = 0; i < nb*bs; i++)
			diskfile<<"#";
		diskfile.close();
	}
	
}	

int sdisk::getblock(int blocknumber, string& buffer){
	fstream diskfile;
	diskfile.open(diskname.c_str(), ios::out | ios::in);
	diskfile.seekp(blocknumber*blocksize);
	char tmp;
	
	for(int i = blocknumber*blocksize; i <= ((blocknumber*blocksize)+blocksize-1); i++){
		diskfile.get(tmp);
		buffer+=tmp;
	}
	diskfile.close();

	return 0;
}

int sdisk::putblock(int blocknumber, string buffer){
	fstream diskfile;
	diskfile.open(diskname.c_str(), ios::out | ios::in);
	diskfile.seekp(blocknumber*blocksize);
	
	for(int i = blocknumber*blocksize, counter = 0; i <= ((blocknumber*blocksize)+blocksize-1); i++){
		diskfile << buffer[counter++];
	}
	
	diskfile.close();
	return 0;
}





vector<string> sdisk::block(string s, int b){
	vector<string> blocks;
	int number_of_blocks = s.size()/b;

	if((s.size()%b)==0)
		number_of_blocks = s.size()/b;
	else
		number_of_blocks = (s.size()/b) + 1;

	for(int i = 0; i < number_of_blocks; i++){
		blocks.push_back(s.substr(b*i, b));
	}

	int lastblock = blocks.size()-1;
	for(int i = blocks[lastblock].length(); i < b; i++){
		blocks[lastblock]+="#";
	}


	return blocks;

}


int sdisk::getnumberofblocks(){
	return numberofblocks;
}

int sdisk::getblocksize(){
	return blocksize;
}







