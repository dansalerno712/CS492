#include "assign3.h"

//intialize globals
Tree* glRoot = NULL;
Tree* glCurrentDir = NULL;
int glBlockSize = 0;
int glDiskSize = 0;
std::list<Block> Ldisk;
	
int main(int argc, char** argv)
{
	//file names for input
	char *dirFile = 0;
	char *fFile = 0;
	
	//check input
	if (argc != 9)
	{
		std::cout << "Error: incorrect number of inputs\n";
		return -1;
	}
	//get commandline options
	for (int i = 1; i < argc - 1; i = i + 2)
	{
		if (!strcmp(argv[i], "-b"))
		{
			glBlockSize = atoi(argv[i + 1]);
		}
		else if (!strcmp(argv[i], "-s"))
		{
			glDiskSize = atoi(argv[i + 1]);
		}
		else if (!strcmp(argv[i], "-f"))
		{
			fFile = argv[i + 1];
		}
		else if (!strcmp(argv[i], "-d"))
		{
			dirFile = argv[i + 1];
		}
	}
		
	//fill Ldisk with one big empty block
	Block b(0, ceil(glDiskSize * 1.0 / glBlockSize) - 1, true);
	Ldisk.push_back(b);
	
	//initial directory setup
	 int lineCount = 0;
	 std::ifstream dirfile(dirFile);
	 std::string line;
	 //read in file
	 while (std::getline(dirfile, line))
	 {
		 //skip empty lines
		 if (line.size() != 0)
		 {
			 //the first line is ./, but we just want . for the directory name
			 if (lineCount == 0)
			 {
				 File *f = new File(line.substr(0, line.find_last_of("/")), 0, true);
				 glRoot = new Tree(f);
			 }
			 else
			 {
				 //make a new file
				File *f = new File(line, 0, true);
				//find parent
				Tree* parent = find(glRoot, line.substr(0, line.find_last_of("/")));
				//add child to tree
				Tree* child = new Tree(f);
				addChild(parent, child);
			 }
			 lineCount++;
		 }
	 } 
	 
	 //initial file setup
	 std::ifstream fileInput(fFile);
	 std::vector<std::string> lineSplit;
	 //read in file
	 while (std::getline(fileInput, line))
	 {
		 //skip empty lines
		 if (line.size() != 0)
		 {
			 //split line by spaces
			lineSplit = str2vec(line, " ");
			//get file name, it is the last element of vector
			std::string fname = lineSplit.back();
			//get file size, it is 4 indexes back from last
			int fsize = atoi(lineSplit[lineSplit.size() - 5].c_str());
			//make new file
			File *f = new File(fname, fsize, false);
			//find parent
			Tree* parent = find(glRoot, fname.substr(0, fname.find_last_of("/")));
			//add new node
			Tree* child = new Tree(f);
			addChild(parent, child);
			
			/*TODO
			* YOU NEED TO ALLOCATE SPACE FOR THESE FILES
			* END TODO
			*/
			allocateFile(f);
		 }
	 } 
	 glCurrentDir = glRoot;
	 
	 std::string userInput;
	 
	 std::cout << "PLEASE READ THIS: In most cases, you need to use full paths for this assignment (i.e you need to type cd <fullpath>). However, for the make and \
	 create functions, since they make files ONLY in the current directory, whatever name you choose will be appended to the current directory \
	 and then added (ie if the current dir is ./test, typing create <file> will add ./test/<file> to the tree\n";
	 
	 while (true)
	 {
		 //print current directory
		 std::cout << "Assignment 3:" << glCurrentDir -> file -> name << "$ ";
		 //get input from user
		 getline(std::cin, userInput);
		 
		 //split by spaces
		 lineSplit = str2vec(userInput, " ");
		 
		 //find what function they want
		 if (lineSplit[0] == "cd")
		 {
			 cd(lineSplit);
		 }
		 else if (lineSplit[0] == "ls")
		 {
			 ls(lineSplit);
		 }
		 else if (lineSplit[0] == "mkdir")
		 {
			 mkdir(lineSplit);
		 }
		 else if (lineSplit[0] == "create")
		 {
			 create(lineSplit);
		 }
		 else if (lineSplit[0] == "append")
		 {
			 append(lineSplit);
		 }
		 else if (lineSplit[0] == "remove")
		 {
			 remove(lineSplit);
		 }
		 else if (lineSplit[0] == "delete")
		 {
			 deleteFS(lineSplit);
		 }
		 else if (lineSplit[0] == "exit")
		 {
			 exitFS(lineSplit);
			 break;
		 }
		 else if (lineSplit[0] == "dir")
		 {
			 dir(lineSplit);
		 }
		 else if (lineSplit[0] == "prfiles")
		 {
			 prfiles(lineSplit);
		 }
		 else if (lineSplit[0] == "prdisk")
		 {
			 prdisk(lineSplit);
		 }
		 else
		 {
			 std::cout << "Invalid Input\n";
		 }
	 }
	 
}