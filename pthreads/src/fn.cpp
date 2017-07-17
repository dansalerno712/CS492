/*
 * CS 492 Assignment 1
 * Dan Salerno and Jaymes Garland
 * 
 * File that defines fn(n) to simulate product life
 *
 */
 
 #include "assign1.h"
 
 /* fn(n): calculates the nth fibonacci number
  * INPUT: unsigned int n: which fibonacci number to calculates
  * OUTPUT: unsigned int: the nth fibonacci number
  */
  unsigned int fn(unsigned int n) {
	  if (n < 0) {
		  std::cerr << "Error: Invalid fn input\n";
	  }
	  else {
		  if (n == 0) {
			  return 1;
		  }		  
		  else if (n == 1) {
			  return 1;
		  }
		  else {
			  return fn(n - 1) + fn(n - 2);
		  }
		  
	  }
  }