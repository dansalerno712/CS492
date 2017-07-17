/*
 * CS 492 Assignment 1
 * Dan Salerno and Jaymes Garland
 * 
 * Main file to run the first assignment
 *
 * This main creates and initializes all global variables, threads, mutexes, condition variables, and the queue
 * then outputs timing data about the run of the program
 */
 
 #include "assign1.h"
 
 //initalize globals
 globals global_vars;
 std::queue<product*> products_queue;
 
 int main(int argc, char **argv) {
	 //input vars and error checking
	 if (argc != 8) {
		 std::cerr << "Error: Incorrect number of inputs\n";
		 return -1;
	 }
	 int producer_threads = atoi(argv[1]);
	 int consumer_threads = atoi(argv[2]);
	 int num_products = atoi(argv[3]);
	 int queue_size = atoi(argv[4]);
	 int scheduling_algorithm = atoi(argv[5]);
	 int quantum = atoi(argv[6]);
	 int seed = atoi(argv[7]);
	 
	 if (producer_threads <= 0 || consumer_threads <= 0 || num_products < 0 || queue_size < 0 || scheduling_algorithm < 0 || scheduling_algorithm > 1 || quantum <= 0) {
		 std::cerr << "Error: Bad input values\n";
		 return -1;
	 }
	 
	 //initalize srand
	 srand(seed);
	 
	 //put input vars into global variable
	 global_vars.num_products = num_products;
	 global_vars.queue_size = queue_size;
	 global_vars.scheduling_algorithm = scheduling_algorithm;
	 global_vars.quantum = quantum;
	 
	 //init global vars
	 global_vars.finished_products = 0;
	 global_vars.created_products = 0;
	 global_vars.total_processing_time = 0;
	 global_vars.min_turnaround_time = std::numeric_limits<double>::max();
	 global_vars.max_turnaround_time = 0;
	 global_vars.total_wait_time = 0;
	 global_vars.min_wait_time = std::numeric_limits<double>::max();
	 global_vars.max_wait_time = 0;
	 global_vars.production_started = false;
	 global_vars.production_ended = false;
	 global_vars.consumption_started = false;
	 global_vars.consumption_ended = false;
	 
	 //init mutexes and cond vars
	 pthread_mutex_init(&global_vars.queue_mutex, NULL);
	 pthread_cond_init(&global_vars.queue_empty, NULL);
	 pthread_cond_init(&global_vars.queue_full, NULL);
	 
	 //create threads
	 pthread_t producers[producer_threads];
	 pthread_t consumers[consumer_threads];
	 int producer_ids[producer_threads];
	 int consumer_ids[consumer_threads];
	 
	 //variables for total processing time (run time of program)
	 struct timespec start, finish;
	 double total_processing_time;
	 clock_gettime(CLOCK_MONOTONIC, &start);
	 
	 for (int i = 0; i < producer_threads; i++) 
	 {
		 producer_ids[i] = i;
		 pthread_create(&producers[i], NULL, (void*(*)(void*))(&producer), (void*)&producer_ids[i]);
	 }
	 for (int j = 0; j < consumer_threads; j++) 
	 {
		 consumer_ids[j] = j;
		 pthread_create(&consumers[j], NULL, (void*(*)(void*))(&consumer), (void*)&consumer_ids[j]);
	 }
	 
	 //join all threads
	 for (int i = 0; i < producer_threads; i++)
	 {
		 pthread_join(producers[i], NULL);
	 }
	 for (int i = 0; i < consumer_threads; i++)
	 {
		 pthread_join(consumers[i], NULL);
	 }
	 
	 //get finish time
	 clock_gettime(CLOCK_MONOTONIC, &finish);
	 //printout of all timing functions
	 std::cout << "\n\n===================== Timing ======================" << std::endl;
	 //total processing time - gets in seconds and adds nanoseconds as well
	 total_processing_time = (finish.tv_sec - start.tv_sec);
	 total_processing_time += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	 std::cout << "Total processing time in seconds: " << total_processing_time << std::endl;
	 std::cout << "Min turnaround time in seconds: " << global_vars.min_turnaround_time << std::endl;
	 std::cout << "Max turnaround time in seconds: " << global_vars.max_turnaround_time << std::endl;
	 std::cout << "Avg turnaround time in seconds: " << global_vars.total_processing_time / global_vars.num_products << std::endl;
	 std::cout << "Min wait time in seconds: " << global_vars.min_wait_time << std::endl;
	 std::cout << "Max wait time in seconds: " << global_vars.max_wait_time << std::endl;
	 std::cout << "Avg wait time in seconds: " << global_vars.total_wait_time / global_vars.num_products << std::endl;
	 
	 //get total time producing in order to calculate producer throughput
	 double production_time = 0;
	 production_time = global_vars.production_end_time.tv_sec - global_vars.production_start_time.tv_sec;
	 production_time += (global_vars.production_end_time.tv_nsec - global_vars.production_start_time.tv_nsec) / 1000000000.0;
	 std::cout << "Producer throughput (products / minute): " << global_vars.num_products / (production_time / 60) << std::endl;
	 
	 //get total time consumer in order to calculate consumer throughput
	 double consumption_time = 0;
	 consumption_time = global_vars.consumption_end_time.tv_sec - global_vars.consumption_start_time.tv_sec;
	 consumption_time += (global_vars.consumption_end_time.tv_nsec - global_vars.consumption_start_time.tv_nsec) / 1000000000.0;
	 std::cout << "Consumer throughput (products finished / minute): " << global_vars.num_products / (consumption_time / 60) << std::endl;
	 
	 //flush cout
	 std::cout.flush();
	 
	 //cleanup mutexes and condition variables
	 pthread_mutex_destroy(&global_vars.queue_mutex);
	 pthread_cond_destroy(&global_vars.queue_empty);
	 pthread_cond_destroy(&global_vars.queue_full);
	 
	 //exit all threads
	 pthread_exit(0);
 }