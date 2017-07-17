#include "PageTable.h"

//sets some variables to intial values
PageTable::PageTable()
{
	processMemory = 0;
	clockIndex = 0;
}

/* FIFO replacenent method
* Keeps indexesInMemory as a FIFO queue
*/
void PageTable::replaceFIFO(int addIndex)
{
	//get page index that is being unloaded
	int indexToUnload = indexesInMemory.front();
	
	//unload that page
	table[indexToUnload] -> valid = 0;
	
	//load the new page
	table[addIndex] -> valid = 1;
	
	//remove old index from FIFO queue
	indexesInMemory.erase(indexesInMemory.begin());
	
	//add new index to end of FIFO queue
	indexesInMemory.push_back(addIndex);
	
	//printout
	//std::cout << "For process " << processID << ", replacing page number " << table[indexToUnload] -> pageNumber << " with page number " \
	 << table[addIndex] -> pageNumber << '\n';
}

//LRU Replacement
void PageTable::replaceLRU(int addIndex, unsigned long accessTime)
{
	//find page to replace (page with min lastAccessTime)
	unsigned long min = -1; //sets min to the largest unsigned long value
	int minIndex = -1;
	
	//loop through all pages loaded in memeory
	for (int i = 0; i < indexesInMemory.size(); i++)
	{
		//upadate min and minIndex if necessary
		if (table[indexesInMemory[i]] -> lastTimeAccessed < min)
		{
			min = table[indexesInMemory[i]] -> lastTimeAccessed;
			minIndex = i;
		}
	}
	
	//unload old page
	table[indexesInMemory[minIndex]] -> valid = 0;
	
	//load new page and update lastTimeAccessed
	table[addIndex] -> valid = 1;
	table[addIndex] -> lastTimeAccessed = accessTime;
	
	//replace old index in vector with new index
	indexesInMemory[minIndex] = addIndex;
	
	//printout
	//std::cout << "For process " << processID << ", replacing page number " << table[minIndex] -> pageNumber << " with page number " \
	 << table[addIndex] -> pageNumber << '\n';
}

//Clock Replacement
void PageTable::replaceClock(int addIndex)
{
	//find index of first R that is 0
	while (table[indexesInMemory[clockIndex]] -> R == 1)
	{
		//if R is one, set it to 0 and move on
		table[indexesInMemory[clockIndex]] -> R = 0;
		clockIndex = (clockIndex + 1) % indexesInMemory.size();
	}
	
	//unload page at clockIndex
	table[indexesInMemory[clockIndex]] -> valid = 0;
	
	//printout
	//std::cout << "For process " << processID << ", replacing page number " << table[indexesInMemory[clockIndex]] -> pageNumber << " with page number " \
	 << table[addIndex] -> pageNumber << '\n';
	
	//load new page
	table[addIndex] -> valid = 1;
	table[addIndex] -> R = 1;
	
	//remove old index from indexesInMemory and replace with new
	indexesInMemory[clockIndex] = addIndex;
	
	//increment clock index one more time
	clockIndex = (clockIndex + 1) % indexesInMemory.size();
}

//For pre-paging
int PageTable::getNextInvalidPageIndex(int startingIndex)
{
	//start looking one past the index we just loaded
	int i = (startingIndex + 1) % table.size();
	
	//loop until you find an invalid page
	while(table[i] -> valid)
	{
		//if you get back to the starting point, then all pages are loaded, so return -1
		if (i == startingIndex)
			return -1;
		
		//increment/loop i
		i = (i + 1) % table.size();
	}
	
	return i;
}