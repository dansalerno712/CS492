#ifndef _ASSIGN3_HH_
#define _ASSIGN3_HH_

//Includes
#include <iostream>
#include "file.h"
#include "tree.h"
#include "block.h"
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string.h>
#include <list>
#include <cmath>

//globals
extern Tree* glRoot;
extern Tree* glCurrentDir;
extern int glDiskSize;
extern int glBlockSize;
extern std::list<Block> Ldisk;

//main
int main(int, char**);

//a function that splits a string by a separator character and returns a vector with the split string
std::vector<std::string> str2vec(std::string s, std::string separator);

//file system functions - found in fsFunctions.cpp
void allocateFile(File *f);
void deallocateFile(File *f);
void mergeLDisk();
void cd(std::vector<std::string> lineSplit);
void ls(std::vector<std::string> lineSplit);
void mkdir(std::vector<std::string> lineSplit);
void create(std::vector<std::string> lineSplit);
void append(std::vector<std::string> lineSplit);
void remove(std::vector<std::string> lineSplit);
void deleteFS(std::vector<std::string> lineSplit);
void exitFS(std::vector<std::string> lineSplit);
void dir(std::vector<std::string> lineSplit);
void prfiles(std::vector<std::string> lineSplit);
void prdisk(std::vector<std::string> lineSplit);

#endif