#include "Page.h"

Page::Page(int id) 
{
	pageNumber = id;
	valid = 0;
	R = 0;
	lastTimeAccessed = 0;
}

void Page::load(unsigned long accessTime)
{
	//also sets R to be 1 (for clock) and updates lastTimeAccessed (for LRU)
	valid = 1;
	R = 1; 
	lastTimeAccessed = accessTime;
}