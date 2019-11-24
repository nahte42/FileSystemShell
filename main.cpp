/****************************
Ethan Jones (005977513)
CSE 461
11/23/2019

Finished Project For CSE 461
****************************/

#include <iostream>
#include <string>
#include "shell.h"
#include "Table.h"

using namespace std;

int main()
{
 //
 //This main program inputs commands to the shell.
 //It inputs commands as : command op1 op2
 //You should modify it to work for your implementation.
 //
 string s;
 string command="go";
 string op1,op2;
 //sdisk disk1("disk1",256,128);
 //Filesys fsys("disk1",256,128);
 Shell sh("disk1",256,128);
 //Table tbl("disk1", 256, 128, "flatfile", "indexfile");

 while (command != "quit")
     {
	   //cout<<"Fat[0]: "<<sh.getfatz()<<endl;
       command.clear();
       op1.clear();
       op2.clear();
       cout << "$";
       getline(cin,s);
       int firstblank=s.find(' ');
       if (firstblank < s.length()) s[firstblank]='#';
       int secondblank=s.find(' ');
       command=s.substr(0,firstblank);
       if (firstblank < s.length())
         op1=s.substr(firstblank+1,secondblank-firstblank-1);
       if (secondblank < s.length())
         op2=s.substr(secondblank+1);
       if (command=="dir")
          {
            sh.dir();
			// use the ls function
           }
       if (command=="add")
          {
			sh.add(op1);
            // The variable op1 is the new file
           }
       if (command=="del")
          {
			sh.del(op1);
            // The variable op1 is the file
           }
       if (command=="type")
          {
			sh.type(op1);
            // The variable op1 is the file
           }
       if (command=="copy")
          {
			sh.copy(op1,op2);
            // The variable op1 is the source file and the variable op2 is the destination file.
           }
		if(command == "search"){
			sh.Search(op1);
		}
       
      }

 return 0;
}