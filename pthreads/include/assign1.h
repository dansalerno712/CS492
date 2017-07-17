#ifndef _ASSIGN1_HH_
#define _ASSIGN1_HH_

//Includes
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <queue>
#include <time.h>
#include <limits>
#include <vector>

//global variables struct
typedef struct globals
{
	//input variables that need to be global
	int num_products;
	int queue_size;
	int scheduling_algorithm;
	int quantum;
	
	//vars we need to track
	int finished_products;
	int created_products;
	
	//mutexes/condition variables
	pthread_mutex_t queue_mutex;
	pthread_cond_t queue_empty;
	pthread_cond_t queue_full;
	
	//timing variables
	//turnaround
	double total_processing_time;
	double min_turnaround_time;
	double max_turnaround_time;
	//wait
	double total_wait_time;
	double min_wait_time;
	double max_wait_time;
	//production throughput
	struct timespec production_start_time;
	bool production_started;
	struct timespec production_end_time;
	bool production_ended;
	//consumption throughput
	struct timespec consumption_start_time;
	bool consumption_started;
	struct timespec consumption_end_time;
	bool consumption_ended;
		
} globals;

//product struct
typedef struct product
{
	int product_id;
	int life;
	struct timespec time_generated;
	//round robin wait time
	std::vector<struct timespec> insertion_times;
	std::vector<struct timespec> removal_times;
} product;

//functions
unsigned int fn(unsigned int);
void *producer(void *);
void *consumer(void *);

//declare global vars
extern globals global_vars;
extern std::queue<product*> products_queue;

#endif