/*
 * CS 492 Assignment 1
 * Dan Salerno and Jaymes Garland
 * 
 * Main file to run the second assignment
 *
 * 
 */

 #include "assign2.h"
 
 int main(int argc, char **argv) {
	 //input params
	 char* pathToPlist;
	 char *pathToPtrace;
	 int pageSize = 0;
	 char *replacementAlgo;
	 bool prePaging;
	 
	 //total number of memory locations for simulation
	 int numMemLocations = 512;
	 
	 //holds all the page tables from each process
	 std::vector<PageTable*> mainPageTables;
	 
	 //get input parameters and check their validity
	 if (argc != 6) {
		 std::cout << "Error: incorrect number of input parameters\n";
		 return -1;
	 }
	 
	 pathToPlist = argv[1];
	 pathToPtrace = argv[2];
	 pageSize = atoi(argv[3]);
	 replacementAlgo = argv[4];
	 if (!strcmp(argv[5], "+")) 
	 {
		 prePaging = 1;
	 }
	 else if (!strcmp(argv[5], "-"))
	 {
		 prePaging = 0;
	 }
	 else
	 {
		 std::cout << "Error: check input prepaging\n";
		 return -1;
	 }
	 
	 if (pageSize <= 0 || pageSize > 32 || !isPower2(pageSize))
	 {
		 std::cout << "Error: check input pagesize\n";
		 return -1;
	 }
	 
	 if (strcmp(replacementAlgo, "FIFO") && strcmp(replacementAlgo, "LRU") && strcmp(replacementAlgo, "Clock"))
	 {
		 std::cout << "Error: check input page replace method\n";
		 return -1;
	 }
	 
	 //read in plist
	 int processCount = 0;
	 std::ifstream plistinfile(pathToPlist);
	 std::string line;
	 
	 while (std::getline(plistinfile, line))
	 {
		 if (line.size() != 0)
		 {
			 std::string process;
			 std::string mem;
			 std::string test;
			 
			 //find the space character
			 int spaceIndex = line.find(' ');
			 
			 //split the line at space
			 process = line.substr(0, spaceIndex);
			 mem = line.substr(spaceIndex + 1);
			 
			 //make and initialize a new page table
			 PageTable *pt = new PageTable();
			 pt -> processID = process.c_str();
			 pt -> processMemory = atoi(mem.c_str());
			 
			 //add it to the vector
			 mainPageTables.push_back(pt);
			 
			 processCount++;
		 }
	 } 
	 
	 //fill up page table for each process
	 int memLocationsPerProcess = numMemLocations / processCount;
	 
	 //unique counter for page ids
	 int pageIDCounter = 1;
	 //for each process's page table
	 for (int i = 0; i < mainPageTables.size(); i++)
	 {
		 //calc number of pages the table needs
		 //note this is a ceiling, since if there are 5 memory locations in a page size 2 system,
		 //3 pages are needed to hold all 5 memory locations
		 int pages = ceil(mainPageTables[i] -> processMemory / (pageSize * 1.0));
		 
		 //for each page needed
		for (int j = 0; j < pages; j++)
		{
			//create a new page with unique id and add it to that process's page table
			Page* p = new Page(pageIDCounter);
			mainPageTables[i] -> table.push_back(p);
			
			//update pageIDCounter
			pageIDCounter++;
		}			
	 }
	 
	 //intial loading
	 //calculate how many pages each process should initially load
	int pagesToLoad = memLocationsPerProcess / pageSize;
	
	//for each process's page table
	for (int i = 0; i < mainPageTables.size(); i++)
	{
		//load all the pages that should load
		//make sure to stop when you have loaded the initial set of pages or if the intial set has encompassed all available pages
		for (int j = 0; j < pagesToLoad && j < mainPageTables[i] -> table.size(); j++)
		{
			//load with lastTimeAccessed of 0
			mainPageTables[i] -> table[j] -> load(0);
			
			//add to indexesInMemory
			mainPageTables[i] -> indexesInMemory.push_back(j);
		}
	}
	
	//read in ptrace
	std::ifstream ptraceinfile(pathToPtrace);
	
	//keep track of what line we are on for lastTimeAccessed
	unsigned long memAccesses = 0;
	//keep track of page replacements
	int pageReplacements = 0;
	while (std::getline(ptraceinfile, line))
	 {
		 if (line.size() != 0)
		 {
			 //incrememnt line
			 memAccesses++;
			 
			 //get process and memory location like before
			 std::string process;
			 std::string mem;
			 int spaceIndex = line.find(' ');
			 process = line.substr(0, spaceIndex);
			 mem = line.substr(spaceIndex + 1);
			 
			 //find the PageTable of the current process we are looking for
			 int tableIndex = -1;
			 for (int i = 0; i < mainPageTables.size(); i++)
			 {
				 if (mainPageTables[i] -> processID == process)
				 {
					 tableIndex = i;
					 break;
				 }
			 }
			 
			 //calculate which page we want to look for in the process's page table
			 int pageToCheck = ceil(atoi(mem.c_str()) / (pageSize * 1.0));
			 		 
			 //if it is currently loaded
			 if (mainPageTables[tableIndex] -> table[pageToCheck - 1] -> valid)
			 {
				 mainPageTables[tableIndex] -> table[pageToCheck - 1] -> load(memAccesses);
			 }
			 //if not, we need to do a replacement
			 else
			 {
				 //FIFO
				 if (!strcmp(replacementAlgo, "FIFO"))
				 {
					 mainPageTables[tableIndex] -> replaceFIFO(pageToCheck - 1);
				 }
				 //LRU
				 else if (!strcmp(replacementAlgo, "LRU"))
				 {
					 mainPageTables[tableIndex] -> replaceLRU(pageToCheck - 1, memAccesses);
				 }
				 //Clock
				 else
				 {
					 mainPageTables[tableIndex] -> replaceClock(pageToCheck - 1);
				 }
				 //add to pageReplacements
				 pageReplacements++;
				 
				 if (prePaging)
				 {
					 //find the next contigous page that is not in memory
					 int nextInvalidPageIndex = mainPageTables[tableIndex] -> getNextInvalidPageIndex(pageToCheck - 1);
					 
					 //swap that page if it's not -1 (no next invalid page was found)
					 if (nextInvalidPageIndex != -1)
					 {
						 memAccesses++;
						 if (!strcmp(replacementAlgo, "FIFO"))
						 {
							 mainPageTables[tableIndex] -> replaceFIFO(nextInvalidPageIndex);
						 }
						 else if (!strcmp(replacementAlgo, "LRU"))
						 {
							 mainPageTables[tableIndex] -> replaceLRU(nextInvalidPageIndex, memAccesses);
						 }
						 else
						 {
							 mainPageTables[tableIndex] -> replaceClock(nextInvalidPageIndex);
						 }
						 //dont count this 2nd page replacement
						 //pageReplacements++;
					 }
				 }
			 }
			 
		 }
	 } 
	 
	 /* for (int i = 0; i < mainPageTables[0] -> indexesInMemory.size(); i++)
	 {
		 //if (mainPageTables[0] -> table[i] -> valid)
		 //{
			 std::cout << mainPageTables[0] -> indexesInMemory[i] + 1 << '\n';
		 //}
	 } */
	 
	 //printout how many replacements we had
	 std::cout << "Page replacements: " << pageReplacements << '\n';
	 
		 	 
	 return 0;
 }
 
 //stackexchange is great
 bool isPower2(int n)
 {
	 return (n > 0 && ((n & (n - 1)) == 0));
 }