#include "assign3.h"

/* A function to merge blocks in Ldisk
*/
void mergeLDisk()
{
	//loop through lDisk
	std::list<Block>::iterator it = Ldisk.begin();	
	std::list<Block>::iterator next;
	while(it != Ldisk.end())
	{	
		//get the next block from Ldisk
		next = it;
		next++;
		
		//unless the next block is the end
		if (next == Ldisk.end())
		{
			return;
		}
		
		
		//if they need to be merged
		if (it -> isFree == next -> isFree)
		{
			//set end of first block to end of 2nd block
			it -> endID = next -> endID;
			
			//remove next block
			Ldisk.erase(next);
		}
		else
		{
			//move iterator up
			it++;
		}
	}
}

/* A function to deallocate bytes from a file
* This function assumes that f's size parameter has been decremented before calling this function.
* This function will remove bytes from f, remove the block addresses from f, and also alter Ldisk if necessary
*/
void deallocateFile(File *f)
{
	//calculate how many blocks to remove
	int diff = f -> allocatedBytes - f -> size;
	int blocksToErase = diff / glBlockSize;
	
	//loop until we erase all blocks
	while (blocksToErase > 0)
	{
		//find the block id to erase
		std::list<int>::iterator it = f -> blockAddresses.end();
		//get the last block address from the linked list
		it--;
		
		//calculate blockID we want to erase
		int test = *it;
		int blockIDToErase = test / glBlockSize;
		
		
		//find block with id to erase
		std::list<Block>::iterator blockit;
		
		for (blockit = Ldisk.begin(); blockit != Ldisk.end(); blockit++)
		{
			if (blockIDToErase >= blockit -> startID && blockIDToErase <= blockit -> endID)
			{
				break;
			}
		}
		
		
		//split block
		//block that we will be inserting into Ldisk
		Block deleted(blockIDToErase, blockIDToErase, true);
		//if size of block we found is 1
		if (blockit -> startID == blockit -> endID)
		{
			blockit -> isFree = true;
		}
		//if size of block we found is 2
		else if (blockit -> endID - blockit -> startID == 1)
		{			
			//if we are freeing the 1st of the 2 ids in block
			if (blockIDToErase == blockit -> startID)
			{
				blockit -> startID = blockit -> endID;
				Ldisk.insert(blockit, deleted);
			}
			//if we are freeing the 2nd of the 2 ids in block
			else
			{
				blockit -> endID = blockit -> startID;
				
				//if this is the last block in memory, we cant increment the iterator so we just push_back
				if (blockit == Ldisk.end())
				{
					Ldisk.push_back(deleted);
				}
				else
				{
					blockit++;
					Ldisk.insert(blockit, deleted);
				}
			}
		}
		else
		{
			//if we are freeing first id
			if (blockit -> startID == blockIDToErase)
			{
				blockit -> startID++;
				Ldisk.insert(blockit, deleted);
			}
			//if we are freeing last id
			else if (blockit -> endID == blockIDToErase)
			{
				blockit -> endID--;
				if (blockit == Ldisk.end())
				{
					Ldisk.push_back(deleted);
				}
				else
				{
					blockit++;
					Ldisk.insert(blockit, deleted);
				}
			}
			//if we are freeing an interior id
			else
			{
				//split the block
				Block before(blockit -> startID, blockIDToErase - 1, false);
				Block after(blockIDToErase + 1, blockit -> endID, false);
				
				//insert new split set of blocks
				Ldisk.insert(blockit, before);				
				Ldisk.insert(blockit, deleted);
				Ldisk.insert(blockit, after);
				
				//erase old block
				Ldisk.erase(blockit);			
			}
		}
		
		blocksToErase--;
		
		//update f
		f -> allocatedBytes -= glBlockSize;
		f -> blockAddresses.erase(it);
	}
	//merge
	mergeLDisk();
}

/* Function to allocate data to a file
* Assumes f's size parameter is changed to the new size before this function is called
* if the disk runs out of space, it sets the size parameter of f to be the amount of bytes is has allocated
*/
void allocateFile(File *f)
{
	//calculate how many blocks are needed
	int diff = f -> size - f -> allocatedBytes;
	if (diff < 0)
	{
		diff = 0;
	}
	
	int blocksNeeded = ceil((diff * 1.0) / glBlockSize);
	
	while (blocksNeeded > 0)
	{
		//find the first free block in Ldisk
		std::list<Block>::iterator it;	
		for (it = Ldisk.begin(); it != Ldisk.end(); it++)
		{
			if (it -> isFree)
			{
				break;
			}
		}
		//check to make sure we actually found a block
		if (it == Ldisk.end())
		{
			//if we didn't find a free block, the disk is full
			std::cout << "Error: disk is full\n";
			//set size to be however much f is currently allocated
			//not sure if this is correct, but it seems wrong to leave the file's size to be something larger than available memory
			f -> size = f -> allocatedBytes;
			//merge 
			mergeLDisk();
			return;
		}
		
		//get size of block we found\n
		int foundSize = it -> endID - it -> startID + 1;
		
		//possibly split blocks
		if (blocksNeeded >= foundSize)
		{
			//set whole block to used
			it -> isFree = false;
			
			//add bytes to file
			f -> allocatedBytes += foundSize * glBlockSize;
			
			//decrement blocksNeeded
			blocksNeeded -= foundSize;
			
			//add block addresses to file
			for (int i = it -> startID; i <= it -> endID; i++)
			{
				//add address, which is id * block size
				f -> blockAddresses.push_back(i * glBlockSize);
			}
		}
		else
		{
			//need to split ldisk block
			Block temp(it -> startID, it -> startID + blocksNeeded - 1, false);
			
			//add block addresses to file
			for (int i = temp.startID; i <= temp.endID; i++)
			{
				//add address, which is id * block size
				f -> blockAddresses.push_back(i * glBlockSize);
			}
			
			//insert new disk block
			Ldisk.insert(it, temp);
			
			//edit current blocks startID
			it -> startID = temp.endID + 1;
			
			//add to f's allocatedBlocks
			f -> allocatedBytes += blocksNeeded * glBlockSize;
			
			//decrement blocks needed
			blocksNeeded = 0;
		}
	}
	
	mergeLDisk();
}

/* Function to change directories
* Takes two options:
* 1) cd .. will change to the parent directory, if it exists
* 2) cd <dirname> will change the current directory to dirname, if it exists
*/
void cd(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 2)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//name to search for
	std::string searchName = lineSplit[1];
	//if cd ..
	if (searchName == "..")
	{
		//only change current dir to parent if it exists
		if (glCurrentDir -> parent == NULL)
		{
			std::cout << "Error: parent directory does not exist\n";
		}
		else
		{
			glCurrentDir = glCurrentDir -> parent;
		}
	}
	else
	{
		//search for the directory
		Tree *searchNode = find(glRoot, searchName);
		//if not found
		if (searchNode == NULL)
		{
			std::cout << "Error: directory not found\n";
		}
		//if user tries to cd to a file
		else if (searchNode -> file -> isDirectory == false)
		{
			std::cout << "Error: not a directory\n";
		}
		//user cd to a valid directory
		else
		{
			glCurrentDir = searchNode;
		}
	}
}

/* Function to print the contents of the current directory.
* This prints the name of each file and whether or not it is a directory. Each entry is on its own line
*/
void ls(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 1)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	//print
	for (int i = 0; i < glCurrentDir -> children.size(); i++)
	{
		std::cout << glCurrentDir -> children[i] -> file -> name << "\t isDir: " << glCurrentDir -> children[i] -> file -> isDirectory << '\n';
	}
}

/* Function to make a directory
* This creates a new, empty directory in the current directory. Since this is explicitly specified, DO NOT type the full path of the directory
* you want to create here, only type the name. The current directory will be prepended to the input name in order to make the full path
*/
void mkdir(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 2)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	//add the new directory to the tree
	File *f = new File(glCurrentDir -> file -> name + "/" + lineSplit[1], 0, true);
	Tree *child = new Tree(f);
	addChild(glCurrentDir, child);
}

/* Function to make a file
* This creates a new, empty file in the current directory. Since this is explicitly specified, DO NOT type the full path of the file
* you want to create here, only type the name. The current directory will be prepended to the input name in order to make the full path
*/
void create(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 2)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//add new file to tree
	File *f = new File(glCurrentDir -> file -> name + "/" + lineSplit[1], 0, false);
	Tree *child = new Tree(f);
	addChild(glCurrentDir, child);
}

void append(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 3)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//find file
	Tree *searched = find(glRoot, lineSplit[1]);
	
	if (searched == NULL)
	{
		std::cout << "Error: could not find file\n";
	}
	//make sure it is a file
	else if (searched -> file -> isDirectory)
	{
		std::cout << "Error: can not perform operation on directory\n";
	}
	else
	{
		//increment size
		searched -> file -> size += atoi(lineSplit[2].c_str());
		//allocate
		allocateFile(searched -> file);
		//update timestamp
		time(&(searched -> file -> timestamp));
	}
}

void remove(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 3)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//find node
	Tree *searched = find(glRoot, lineSplit[1]);
	
	if (searched == NULL)
	{
		std::cout << "Error: could not find file\n";
		return;
	}
	//make sure it is a directory
	else if (searched -> file -> isDirectory)
	{
		std::cout << "Error: can not perform operation on directory\n";
	}
	else
	{
		//decrement file size
		searched -> file -> size -= atoi(lineSplit[2].c_str());
		
		if (searched -> file -> size < 0)
		{
			searched -> file -> size = 0;
		}
		
		//deallocate
		deallocateFile(searched -> file);
		//update timestamp
		time(&(searched -> file -> timestamp));
	}
}

void deleteFS(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 2)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//find node
	Tree *searched = find(glRoot, lineSplit[1]);
	
	if (searched == NULL)
	{
		std::cout << "Error: could not find file\n";
		return;
	}
	
	//parent
	Tree *parent = searched -> parent;
	
	//directory
	if (searched -> file -> isDirectory)
	{
		//make sure it is empty
		if (searched -> children.size() != 0)
		{
			std::cout << "Error: Directory is not empty, can not be removed\n";
		}
		else
		{
			//delete
			deleteChild(parent, searched);
		}
	}
	else
	{
		//set size to 0
		searched -> file -> size = 0;
		//deallocate
		deallocateFile(searched -> file);
		
		//delete
		deleteChild(parent, searched);
		
		//update parent timestamp
		time(&(parent -> file -> timestamp));
	}
}

void exitFS(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 1)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//temp Tree to mark current progress
	Tree* temp = glRoot;
	//A "Queue" to order the Tree nodes
	std::vector<Tree *> q;
	
	//keep going until temp is NULL (out of Tree nodes in the Tree to traverse)
	while (temp != NULL)
	{		
		//add all of the current Tree node's children to q
		for (int i = 0; i < temp -> children.size(); i++)
		{
			q.push_back(temp -> children[i]);
		}
		
		//free memory
		delete temp -> file;
		delete temp;
		
		//if q is empty, we have traversed the whole tree, so set temp to NULL so we exit
		if (q.size() == 0)
		{
			temp = NULL;
		}
		//set temp to the next Tree in the queue, then remove that Tree from the queue
		else
		{
			temp = q[0];
			q.erase(q.begin());
		}
	}	
}

/* Function to print the directory structure
* This prints the entire directory tree in BFS order. Check comments in the printBFS function for more
*/
void dir(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 1)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	printBFS(glRoot);
}

void prfiles(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 1)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}
	
	//temp Tree to mark current progress
	Tree* temp = glRoot;
	//A "Queue" to order the Tree nodes
	std::vector<Tree *> q;
	
	//keep going until temp is NULL (out of Tree nodes in the Tree to traverse)
	while (temp != NULL)
	{
		if (!temp -> file -> isDirectory) {
			//print the current node's file
			temp -> file -> printFull();
		}
		
		//add all of the current Tree node's children to q
		for (int i = 0; i < temp -> children.size(); i++)
		{
			q.push_back(temp -> children[i]);
		}
		
		//if q is empty, we have traversed the whole tree, so set temp to NULL so we exit
		if (q.size() == 0)
		{
			temp = NULL;
		}
		//set temp to the next Tree in the queue, then remove that Tree from the queue
		else
		{
			temp = q[0];
			q.erase(q.begin());
		}
	}
}

void prdisk(std::vector<std::string> lineSplit)
{
	//check input parameter length
	if (lineSplit.size() != 1)
	{
		std::cout << "Error: incorrect number of paramters\n";
		return;
	}

	//loop through list
	std::list<Block>::iterator it;
	
	//print free or in use chunks
	for (it = Ldisk.begin(); it != Ldisk.end(); it++)
	{
		if (it -> isFree)
		{
			std::cout << "Free: " << it -> startID << "-" << it -> endID << "\n";
		}
		else
		{
			std::cout << "In use: " << it -> startID << "-" << it -> endID << "\n";
		}
	}
	
	//calculate fragmentation
	int fragmentation = 0;
	
	//temp Tree to mark current progress
	Tree* temp = glRoot;
	//A "Queue" to order the Tree nodes
	std::vector<Tree *> q;
	
	//keep going until temp is NULL (out of Tree nodes in the Tree to traverse)
	while (temp != NULL)
	{
		//add the fragmentation from each file
		if (!temp -> file -> isDirectory) {
			fragmentation += temp -> file -> allocatedBytes - temp -> file -> size;
		}
		
		//add all of the current Tree node's children to q
		for (int i = 0; i < temp -> children.size(); i++)
		{
			q.push_back(temp -> children[i]);
		}
		
		//if q is empty, we have traversed the whole tree, so set temp to NULL so we exit
		if (q.size() == 0)
		{
			temp = NULL;
		}
		//set temp to the next Tree in the queue, then remove that Tree from the queue
		else
		{
			temp = q[0];
			q.erase(q.begin());
		}
	}
	
	std::cout << "Fragmentation: " << fragmentation << " bytes\n";
}