#include <omp.h>
#include "stdlib.h"
#include "stdio.h"

#include <chrono>
#include <time.h>
#include <random>
#include <iostream>
#include <iomanip>


long threads;
long N;
double *A, *B, *C;

void mat_mul(long i);

using namespace std;

int main(int argc, char* argv[]){
	auto begin = chrono::high_resolution_clock::now();
	if(argc != 3){
		cout << "Usage: ./progname <threads> <array size>\n";
		return 1;
	}
	threads = atol(argv[1]);
	N = atol(argv[2]);
	A = (double*)malloc(sizeof(double) * N * N);
	B = (double*)malloc(sizeof(double) * N * N);
	C = (double*)malloc(sizeof(double) * N * N);
	
	srand(time(NULL));
	// two loops for better cache locality thru A and B
	for(long i = 0; i < N*N; i++){
		*(A + i) = (double)rand();
	}
	for(long i = 0; i < N*N; i++){
		*(B + i) = (double)rand();
	}

    #   pragma omp parallel for num_threads(threads)
	for(long i = 0; i < N*N; i++){
		mat_mul(i);
	}
	
	free(A); free(B); free(C);
	
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);
	cout << "Number of threads: " << setw(2) << threads << " Matrix size: " << setw(9) << N;
	cout << " Milliseconds taken: " << setw(15) << duration.count() << endl;
	return 0;	
}

void mat_mul(long i){
	double val = 0.0;
	double* a = A + (i / N);
	double* b = B + (i % N);
	for(long j = 0; j < N; j++){
		val += *(a + j) * *(b + (N*j));
	}
	*(C + i) = val;
}

