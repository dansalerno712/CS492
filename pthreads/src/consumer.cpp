/*
 * CS 492 Assignment 1
 * Dan Salerno and Jaymes Garland
 * 
 * Function for the consumer threads
 *
 * Each consumer safely grabs a product from the queue. If it is FCFS, the consumer immediately consumes the whole product 
 * (while unlocking the mutex during processing time so that other consumers can do work) and then calculates timing data about the product.
 * For RR scheduling, the consumer either processes the quantum and inserts the product (safely) back into the queue, or processes the rest of
 * the life of the product and then calculates timing data about that product. The consumers also keep track of start and end consumption times
 * for analysis of throughput later.
 */
 #include "assign1.h"

 
 void *consumer(void *args) {
	 //get consumer id from args
	int consumer_id = *(int *)args;
	
	while (1) {
		//lock mutex
		pthread_mutex_lock(&global_vars.queue_mutex);
		//initalize consumption start time if not done already
		if (global_vars.consumption_started == false) 
		{
			clock_gettime(CLOCK_MONOTONIC, &global_vars.consumption_start_time);
			global_vars.consumption_started = true;
		}
		
		//wait if queue is empty and we aren't done consuming products so that consumers dont sleep when all consumption is done
		while(products_queue.size() <= 0 && global_vars.finished_products < global_vars.num_products) 
		{
			pthread_cond_wait(&global_vars.queue_empty, &global_vars.queue_mutex);
		}
		//if we aren't done consuming products
		if (global_vars.finished_products < global_vars.num_products)
		{
			//get info from global vars
			int algorithm = global_vars.scheduling_algorithm;
			int quantum = global_vars.quantum;
			//get product from queue
			product *product_from_queue = products_queue.front();
			//consume the product -- First Come First Serve
			if (algorithm == 0) {
				//remove from queue
				products_queue.pop();
				//broadcast to sleeping producers
				pthread_cond_broadcast(&global_vars.queue_full);
				//get removal time
				struct timespec product_removal_time;
				clock_gettime(CLOCK_MONOTONIC, &product_removal_time);
				//unlock
				pthread_mutex_unlock(&global_vars.queue_mutex);
				//should be unlocked around this processing time so that other consumers can get mutex and grab a product rather than waiting
				//run fn(10) for the life of the product
				for (int i = 0; i < product_from_queue -> life; i++)
				{
					fn(10);
				}
				
				//get mutex to alter global vars
				pthread_mutex_lock(&global_vars.queue_mutex);
				
				//printout
				std::cout << "Consumer " << consumer_id << " has completed product " << product_from_queue -> product_id << std::endl;
				std::cout.flush();
				
				//increment number of finished products
				global_vars.finished_products += 1;
				
				//add processing time of this product to global vars
				//turn around time
				struct timespec product_processing_finish_time;
				double product_turnaround_time = 0;
				clock_gettime(CLOCK_MONOTONIC, &product_processing_finish_time);
				product_turnaround_time = product_processing_finish_time.tv_sec - product_from_queue -> time_generated.tv_sec;
				product_turnaround_time += (product_processing_finish_time.tv_nsec - product_from_queue -> time_generated.tv_nsec) / 1000000000.0;
				
				global_vars.total_processing_time += product_turnaround_time;
				
				if (product_turnaround_time < global_vars.min_turnaround_time)
					global_vars.min_turnaround_time = product_turnaround_time;
				
				if (product_turnaround_time > global_vars.max_turnaround_time)
					global_vars.max_turnaround_time = product_turnaround_time;
				
				//wait time
				double product_wait_time = 0;
				product_wait_time = product_removal_time.tv_sec - product_from_queue -> time_generated.tv_sec;
				product_wait_time += (product_removal_time.tv_nsec - product_from_queue -> time_generated.tv_nsec) / 1000000000.0;
				
				global_vars.total_wait_time += product_wait_time;
				if (product_wait_time < global_vars.min_wait_time)
					global_vars.min_wait_time = product_wait_time;
				
				if (product_wait_time > global_vars.max_wait_time)
					global_vars.max_wait_time = product_wait_time;
				
				
				//unlock
				pthread_mutex_unlock(&global_vars.queue_mutex);
				
				//sleep
				usleep(100 * 1000);
			}
			//consume product -- Round Robin
			else
			{
				//remove from queue
				products_queue.pop();
				//we don't broadcast to the producer threads here because the product is not being permanently removed
				//get removal time
				struct timespec product_removal_time;
				clock_gettime(CLOCK_MONOTONIC, &product_removal_time);
				//if quantum isn't enough to full consume the product
				if (product_from_queue -> life >= quantum) 
				{
					//unlock
					pthread_mutex_unlock(&global_vars.queue_mutex);
					//add removal time
					product_from_queue -> removal_times.push_back(product_removal_time);
					//decrement life of product
					product_from_queue -> life = product_from_queue -> life - quantum;
					
					//run quantum times
					for (int i = 0; i < quantum; i++)
					{
						fn(10);
					}
					
					//get mutex to alter global vars
					pthread_mutex_lock(&global_vars.queue_mutex);
					//put product back in queue
					struct timespec product_reinsertion_time;
					products_queue.push(product_from_queue);
					clock_gettime(CLOCK_MONOTONIC, &product_reinsertion_time);
					product_from_queue -> insertion_times.push_back(product_reinsertion_time);
					//broadcast only to waiting consumers since this product isn't being permanently removed from queue
					pthread_cond_broadcast(&global_vars.queue_empty);
					//unlock mutex
					pthread_mutex_unlock(&global_vars.queue_mutex);
				}
				//quantum is enough to finish the product consumptions
				else
				{
					//broadcast to waiting producers
					pthread_cond_broadcast(&global_vars.queue_full);
					//unlock
					pthread_mutex_unlock(&global_vars.queue_mutex);
					//add removal time 
					product_from_queue -> removal_times.push_back(product_removal_time);
					
					//finish the product's life
					for (int i = 0; i < product_from_queue -> life; i++)
					{
						fn(10);
					}
				
					//get mutex to alter global vars
					pthread_mutex_lock(&global_vars.queue_mutex);
					
					//printout
					std::cout << "Consumer " << consumer_id << " has completed product " << product_from_queue -> product_id << std::endl;
					std::cout.flush();
					
					//update finished_products and broadcast
					global_vars.finished_products += 1;
					
					//add processing time of this product to global vars
					struct timespec product_processing_finish_time;
					double product_turnaround_time = 0;
					clock_gettime(CLOCK_MONOTONIC, &product_processing_finish_time);
					product_turnaround_time = product_processing_finish_time.tv_sec - product_from_queue -> time_generated.tv_sec;
					product_turnaround_time += (product_processing_finish_time.tv_nsec - product_from_queue -> time_generated.tv_nsec) / 1000000000.0;
					
					global_vars.total_processing_time += product_turnaround_time;
					
					if (product_turnaround_time < global_vars.min_turnaround_time)
						global_vars.min_turnaround_time = product_turnaround_time;
					
					if (product_turnaround_time > global_vars.max_turnaround_time)
						global_vars.max_turnaround_time = product_turnaround_time;
					
					//wait time
					double wait_time = 0;
					for (int i = 0; i < product_from_queue -> insertion_times.size(); i++)
					{
						wait_time = product_from_queue -> removal_times.at(i).tv_sec - product_from_queue -> insertion_times.at(i).tv_sec;
						wait_time += (product_from_queue -> removal_times.at(i).tv_nsec - product_from_queue -> insertion_times.at(i).tv_nsec) / 1000000000.0;
					}
					
					global_vars.total_wait_time += wait_time;
					
					if (wait_time < global_vars.min_wait_time)
						global_vars.min_wait_time = wait_time;
					if (wait_time > global_vars.max_wait_time)
						global_vars.max_wait_time = wait_time;
					
					
					//unlock mutex
					pthread_mutex_unlock(&global_vars.queue_mutex);
				}
				
				//sleep
				usleep(100 * 1000);
			}
		}
		else
		{
			//initalize consumption end time if not done already
			if (global_vars.consumption_ended == false) 
			{
				clock_gettime(CLOCK_MONOTONIC, &global_vars.consumption_end_time);
				global_vars.consumption_ended = true;
			}
			
			//wakeup all consumers so they can also exit
			pthread_cond_broadcast(&global_vars.queue_empty);
			//unlock
			pthread_mutex_unlock(&global_vars.queue_mutex);
			break;
		}
	}
	//exit thread
	pthread_exit(0);
 }
 