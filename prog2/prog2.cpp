#include <pthread.h>
#include "stdlib.h"
#include "stdio.h"

#include <chrono>
#include <time.h>
#include <random>
#include <iostream>
#include <iomanip>


long num_threads;
long N;
double* mat_a;
double* mat_b;
double* mat_c;

void* mat_mul(void* rank);
void mat_mul_rc(double* colA, double* rowB, double* rcC);

using namespace std;

int main(int argc, char* argv[]){
	auto begin = chrono::high_resolution_clock::now();
	if(argc != 3){
		cout << "Usage: ./progname <threads> <array size>\n";
		return 1;
	}
	long thread;
	pthread_t* thread_handles;
	num_threads = atol(argv[1]);
	N = atol(argv[2]);
	mat_a = new double[N*N];
	mat_b = new double[N*N];
	mat_c = new double[N*N];
	thread_handles = (pthread_t*)malloc(num_threads * sizeof(pthread_t));
	
	srand(time(NULL));
	// two loops for better cache locality thru A and B
	for(size_t i = 0; i < N*N; i++){
		mat_a[i] = (double)(rand() / RAND_MAX);
	}
	for(size_t i = 0; i < N*N; i++){
		mat_b[i] = (double)(rand() / RAND_MAX);
	}
	
	for(thread = 0; thread < num_threads; thread++){
		pthread_create(&thread_handles[thread], NULL,
			mat_mul, (void*)thread);
	}
	
	for(thread = 0; thread < num_threads; thread++){
		pthread_join(thread_handles[thread], NULL);
	}
	free(thread_handles);
	delete[] mat_a; delete[] mat_b; delete[] mat_c;
	
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - begin);
	cout << "Num threads: " << setw(2) << num_threads << " Matrix size: " << setw(9) << N;
	cout << " Microseconds taken: " << setw(15) << duration.count() << endl;
	return 0;	
}

void* mat_mul(void* rank){
	long my_rank = (long) rank;
	long my_n = N / num_threads;
	long my_start = my_rank * my_n;
	long my_end = my_start + my_n;
	
	for(size_t i = my_start; i < my_end; i++){
		size_t row = i / N;
		size_t col = i % N;
		mat_mul_rc(mat_a + row, mat_b + col, mat_c + i);
	}

	return NULL;
}

void mat_mul_rc(double* a, double* b, double* c){
	double val = 0.0;
	for(size_t i = 0; i < N; i++){
		val += *(a + i) * *(b + N*i);
	}
	*c = val;
}

