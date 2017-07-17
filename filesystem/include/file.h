#ifndef _FILE_HH_
#define _FILE_HH_

//Includes
#include <iostream>
#include <ctime>
#include <list>

/*
* This class represents a file. It holds the name, size, timestamp, and will soon hold an Lfile vector
*/
class File
{
	public:
	std::string name;
	int size;
	time_t timestamp;
	bool isDirectory;
	//number of bytes currently allocated to this file in the file system
	int allocatedBytes;
	//linked list of block addresses
	std::list<int> blockAddresses;
	
	File(std::string n, int s, bool isD);
	
	//A simple print method that shows the name, size, isDirectory, and timestamp of a file
	void print();
	//just like print() but also includes the blockAddresses
	void printFull();
};


#endif