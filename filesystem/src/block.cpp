#include "block.h"

//Constructor
Block::Block(int start, int end, bool free)
{
	startID = start;
	endID = end;
	isFree = free;
}

