#ifndef _TREE_HH_
#define _TREE_HH_

//Includes
#include <iostream>
#include <vector>
#include "file.h"


/*
* This is an implementation of a general tree. Each tree node has a File* as its data member. It also keeps track of all of its
* children in a vector and also keeps track of its parent Tree*.
* 2 Tree classes are said to be equal if the filename (full path) of their File objects are equal
*/
class Tree
{
	public:
	File *file;
	std::vector<Tree *> children;
	Tree *parent;
	
	Tree(File *f);
};

//useful functions for this tree implementation. Check tree.cpp for more info.
void addChild(Tree* parent, Tree* child);
Tree *deleteChild(Tree* parent, Tree* child);
void printBFS(Tree* start);
Tree* find(Tree* start, std::string filename);


#endif