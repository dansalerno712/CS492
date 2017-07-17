#ifndef _BLOCK_HH_
#define _BLOCK_HH_

//Includes


/*
* This class represents a disk block. It holds the start and end id and whether or not it is used.
* A block with only 1 id will have the same start and end ID 
*/
class Block
{
	public:
	int startID;
	int endID;
	bool isFree;
	
	Block(int start, int end, bool free);
};


#endif