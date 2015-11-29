#include <omp.h>
#include "stdlib.h"
#include "stdio.h"
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;

long threads;
long N;
float *A, *B, *C;

void mat_mul(long i){
	float val = 0.0;
	float* a = A + (i / N);
	float* b = B + (i % N);
	for(long j = 0; j < N; j++){
		val += *(a + j) * *(b + (N*j));
	}
	*(C + i) = val;
}

int main(int argc, char* argv[]){
	auto begin = chrono::high_resolution_clock::now();
	if(argc != 3){
		cout << "Usage: ./progname <threads> <array size>\n";
		return 1;
	}
	threads = atol(argv[1]);
	N = atol(argv[2]);
	A = (float*)malloc(sizeof(float) * N * N);
	B = (float*)malloc(sizeof(float) * N * N);
	C = (float*)malloc(sizeof(float) * N * N);
	
	for(long i = 0; i < N*N; i++){
		*(A + i) = (float) i / (float)N;
		*(B + i) = (float) i / (float)N;
	}

    #pragma omp parallel for num_threads(threads)
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

