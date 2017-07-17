#include "file.h"

//Constructor: sets the timestamp to the time it was created at
File::File(std::string n, int s, bool isD)
{
	name = n;
	size = s;
	isDirectory = isD;
	time(&timestamp);
	allocatedBytes = 0;
}

void File::print()
{
	std::cout << "Filename: " << name << "\tIs Directory: " << isDirectory << "\tSize: " << size << "\tTimestamp: " << ctime(&timestamp);
}

void File::printFull()
{
	std::cout << "Filename: " << name << "\tIs Directory: " << isDirectory << "\tSize: " << size << "\tTimestamp: " << ctime(&timestamp);
	
	//loop through the linked list and print each address
	std::list<int>::iterator it;

	std::cout << "Block addresses: ";
	
	for (it = blockAddresses.begin(); it != blockAddresses.end(); it++)
	{
		std::cout << *it << ", ";
	}
	
	std::cout << std::endl;
}