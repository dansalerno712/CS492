#include "tree.h"

Tree::Tree(File* f)
{
	file = f;
	parent = NULL;
}

/*
* Adds Tree* child to parent's children vector and sets the parent data member of child to be parent
*/
void addChild(Tree* parent, Tree* child)
{
	parent -> children.push_back(child);
	child -> parent = parent;
}

/*
* A simple print function that prints the File data member of each tree node. The tree is traversed in breadth first search order.
*/
void printBFS(Tree *start)
{
	if (start != NULL)
	{
		//temp Tree to mark current progress
		Tree* temp = start;
		//A "Queue" to order the Tree nodes
		std::vector<Tree *> q;
		
		//keep going until temp is NULL (out of Tree nodes in the Tree to traverse)
		while (temp != NULL)
		{
			//print the current node's file
			temp -> file -> print();
			
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
}

/*
* A simple function to find and return a Tree node that holds a file with a specific file name. This functions essentially 
* the same as printBFS, but stops when it finds the correct Tree node and returns it.
* If the node is not found, it returns NULL
*/
Tree *find(Tree *start, std::string filename)
{
	if (start != NULL)
	{
		Tree* temp = start;
		std::vector<Tree *> q;
		
		while (temp != NULL)
		{
			//only difference from print BFS is here, if the current Tree node's file name is what we are looking for,
			//break out of the loop so we can return
			if (temp -> file -> name == filename)
			{
				break;
			}
			
			for (int i = 0; i < temp -> children.size(); i++)
			{
				q.push_back(temp -> children[i]);
			}
			
			if (q.size() == 0)
			{
				temp = NULL;
			}
			else
			{
				temp = q[0];
				q.erase(q.begin());
			}
		}
		
		return temp;
	}
	else
	{
		return NULL;
	}
}

/* This function assumes that you are allowed to delete this child, meaning that it has no children.
 * It returns the tree that was deleted so that you can free its memory
*/
Tree  *deleteChild(Tree* parent, Tree* child)
{
	//To delete a node from the tree, all we need to do is remove it from it's parent's children vector
	for (int i = 0; i < parent -> children.size(); i++)
	{
		//if this Tree from the parent's child vector is the node we want to delete
		if (parent -> children[i] -> file -> name == child -> file -> name)
		{
			//erase it from the parent's vector
			parent -> children.erase(parent -> children.begin() + i);
		}
	}
	
	//get rid of the child's reference to parent
	child -> parent = NULL;
	
	return child;
}

