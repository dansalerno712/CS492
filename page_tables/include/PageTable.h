#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <vector>
#include <string>
#include "Page.h"
#include <iostream>

/*
* This class defines a PageTable.
* processID is the id value from plist
* processMemory is the total memory units needed by this process, also from plist
* table is a vector of all the pages needed to hold processMemory memory units
* indexesInMemory keeps track of the indexes of table that are loaded in memory
* clockIndex is the index used for clock replacement
*
* A note about indexesInMemory:
* For FIFO, this vector acts as a FIFO queue. We will remove the index at the top of the queue, and add new indexes at the end
* For clock, this acts as the clock vector. clockIndex will loop around this vector, and any swaps will be made in place.
* This is not used in any special way for LRU
*/
class PageTable
{
public:
  std::string processID;
  int processMemory;
  std::vector<Page*> table;
  std::vector<int> indexesInMemory;
  int clockIndex;

  //constructor
  PageTable();
  
  //replacement methods (explained in cpp file)
  void replaceFIFO(int addIndex);
  void replaceLRU(int addIndex, unsigned long accessTime);
  void replaceClock(int addIndex);
  int getNextInvalidPageIndex(int startingIndex);
};

#endif