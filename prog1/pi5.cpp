#include <pthread.h>
#include "stdlib.h"
#include "stdio.h"

#include <chrono>
#include <iostream>
#include <iomanip>


long num_threads;
long iterations;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
double sum;

void* pi(void* rank);

using namespace std;

int main(int argc, char* argv[]){
	if(argc != 3){
		cout << "Usage: ./progname <threads> <iterations>\n";
		return 1;
	}
	long thread;
	pthread_t* thread_handles;
	num_threads = atol(argv[1]);
	iterations = atol(argv[2]);
	auto begin = chrono::high_resolution_clock::now();
	thread_handles = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
	
	sum = 0.0;
	for(thread = 0; thread < num_threads; thread++){
		pthread_create(&thread_handles[thread], NULL,
			pi, (void*)thread);
	}
	
	for(thread = 0; thread < num_threads; thread++){
		pthread_join(thread_handles[thread], NULL);
	}
	sum *= 4.0;
	free(thread_handles);
	auto end = chrono::high_resolution_clock::now();
	
	auto duration = chrono::duration_cast<chrono::nanoseconds>(end - begin);
	
	cout << "pi5.cpp: num threads: " << setw(2) << num_threads << " iterations: " << setw(9) << iterations;
	cout << " nanoseconds taken: " << setw(15) << duration.count() << " final sum: " << setw(12) << sum << endl;

	return 0;	
}

void* pi(void* rank){
	long my_rank = (long) rank;
	long my_n = iterations / num_threads;
	long my_start = my_rank * my_n;
	long my_end = my_start + my_n;
	double factor;
	double my_sum = 0.0;
	if(my_start % 2 == 0){
		factor = 1.0;
	}
	else{
		factor = -1.0;
	}

	for(long i = my_start; i < my_end; i++, factor = -factor){
		my_sum += factor / (2.0 * i + 1);
	}

	pthread_mutex_lock(&mutex);
	sum += my_sum;
	pthread_mutex_unlock(&mutex);

	return NULL;
}
