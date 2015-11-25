#include "cuda.h"
#include "cuda_runtime.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;


__global__ void gpu_mat_mul(float *A, float* B, float* C, long N)
{
  long i = blockIdx.x * blockDim.x + threadIdx.x;
  if(i >= N) return;
  float val = 0.0;
  float* a = A + (i / N);
  float* b = B + (i % N);
  for(long j = 0; j < N; j++){
  	val += *(a + j) * *(b + (N*j));
  }
  *(C + i) = val;
}

int main(int argc, char **argv){
	auto begin = chrono::high_resolution_clock::now();
	if(argc < 2){
		printf("usage: ./%s <Size> \n", argv[0]);
		return 1;
	}
    cudaSetDevice(0);
   	long size = atol(argv[1]);
   	long N = size*size;
   	size_t bytes = sizeof(float) * N;
   	
   	float* A_h = (float*)malloc(bytes);
   	float* B_h = (float*)malloc(bytes);
   	float* C_h = (float*)malloc(bytes);
   	
	float *A_d, *B_d, *C_d;
	cudaMalloc((void**)&A_d, bytes);
	cudaMalloc((void**)&B_d, bytes);
	cudaMalloc((void**)&C_d, bytes);
	
	memset(A_h, 1.0, bytes);
	memset(B_h, 1.0, bytes);
	
	cudaMemcpy(A_d, A_h, bytes, cudaMemcpyHostToDevice);
	cudaMemcpy(B_d, B_h, bytes, cudaMemcpyHostToDevice);
	free(B_h);
	free(A_h);
	
	int blockSize;   // The launch configurator returned block size 
	int minGridSize; // The minimum grid size needed to achieve the 
		             // maximum occupancy for a full device launch 
	int gridSize;    // The actual grid size needed, based on input size 
	cudaOccupancyMaxPotentialBlockSize( &minGridSize, &blockSize, gpu_mat_mul, 0, N);
	// Round up according to array size 
	gridSize = (N + blockSize - 1) / blockSize;
	gpu_mat_mul <<< gridSize, blockSize >>> (A_d, B_d, C_d, N);
	
	cudaMemcpy(C_h, C_d, bytes, cudaMemcpyDeviceToHost);
	
	cudaFree(C_d);
	cudaFree(B_d);
	cudaFree(A_d);
	
	free(C_h);

	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);
	cout << "Number of threads: " << setw(2) << blockSize * gridSize << " Matrix size: " << setw(9) << N;
	cout << " Microseconds taken: " << setw(15) << duration.count() << endl;
	return 0;
}
