/*
 * CS 492 Assignment 1
 * Dan Salerno and Jaymes Garland
 * 
 * Function for the producer threads
 *
 * Each producer safely creates a product, fills in all its internal variables, and inserts it into the queue.
 * Also keeps track of the times when the producers start and finish all production
 */
 #include "assign1.h"
 
 void *producer(void *args) {
	 //get producer id from args
	int producer_id = *(int *)args;
	
	while (1) {
		//lock mutex
		pthread_mutex_lock(&global_vars.queue_mutex);
		
		//initalize production start time if not done already
		if (global_vars.production_started == false) 
		{
			clock_gettime(CLOCK_MONOTONIC, &global_vars.production_start_time);
			global_vars.production_started = true;
		}
		//check if queue is full (if necessary)
		if (global_vars.queue_size != 0)
		{
			//break from while loop if all products have been made so that producers that are done don't go to sleep anymore
			while (products_queue.size() >= global_vars.queue_size && global_vars.created_products < global_vars.num_products)
			{
				pthread_cond_wait(&global_vars.queue_full, &global_vars.queue_mutex);
			}
		}
		if (global_vars.created_products < global_vars.num_products) {		 
			//make product
			product *new_product = new product;

			//get life
			int life = rand() % 1024;
			
			//add to struct
			//get time 
			clock_gettime(CLOCK_MONOTONIC, &(new_product -> time_generated));
			new_product -> life = life;
			new_product -> product_id = 0;
			new_product -> insertion_times.push_back(new_product -> time_generated);
			
			//put id into the product
			new_product->product_id = global_vars.created_products;
			//increment products produced
			global_vars.created_products = global_vars.created_products + 1;
			 
			//add to queue
			products_queue.push(new_product);
			
			//output
			std::cout << "Producer " << producer_id << " has created product " << new_product -> product_id << std::endl;
			std::cout.flush();
			
			//broadcast that queue is not empty
			pthread_cond_broadcast(&global_vars.queue_empty);
			//done with critical section, so release lock
			pthread_mutex_unlock(&global_vars.queue_mutex);
			
			//sleep
			usleep(100 * 1000);
		}
		else {
			//initalize production end time if not done already
			if (global_vars.production_ended == false) 
			{
				clock_gettime(CLOCK_MONOTONIC, &global_vars.production_end_time);
				global_vars.production_ended = true;
			}
			
			//wakeup all producers so they can also exit
			pthread_cond_broadcast(&global_vars.queue_full);
			//done with critical section, so release lock
			pthread_mutex_unlock(&global_vars.queue_mutex);
			break;
		}
	}
	//exit thread
	pthread_exit(NULL);
 }