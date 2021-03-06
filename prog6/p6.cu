#include "cuda.h"
#include "cuda_runtime.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include <chrono>
#include <iostream>
#include <iomanip>

using namespace std;

#define CUDA_DEBUG {	\
		cudaError_t err = cudaGetLastError();	\
		if (err != cudaSuccess){ 	\
			printf("Error at %s %s: %s\n", __LINE__, __FILE__, cudaGetErrorString(err));	\
			exit(1);	\
		}	\
}	

__global__ void gpu_mat_mul(float* A, float* B, float* C, long width, long N)
{
  const long i = (blockIdx.x * blockDim.x + threadIdx.x);
  if(i >= N) return;
  float val = 0.0;
  float* a = A + (i / width);
  float* b = B + (i % width);
  for(long j = 0; j < width; j++){
  	val += *(a + j) * *(b + (width*j));
  }
  *(C + i) = val;
}

int main(int argc, char **argv){
	auto begin = chrono::high_resolution_clock::now();
	if(argc < 2){
		printf("usage: ./%s <size> \n", argv[0]);
		return 1;
	}
    
   	long size = atol(argv[1]);
   	long N = size*size;
   	size_t bytes = sizeof(float) * N;
   	
   	float* A_h = (float*)malloc(bytes);
   	float* B_h = (float*)malloc(bytes);
   	float* C_h = (float*)malloc(bytes);
   	
   	for(long i = 0; i < N; i++){
   		*(A_h + i) = (float)i / (float)N;
   		*(B_h + i) = (float)i / (float)N;
   	}
   	
	float *A_d, *B_d, *C_d;
	cudaMalloc((void**)&A_d, bytes);
	cudaMalloc((void**)&B_d, bytes);
	cudaMalloc((void**)&C_d, bytes);
	
	CUDA_DEBUG
	
	cudaMemcpy(A_d, A_h, bytes, cudaMemcpyHostToDevice);
	cudaMemcpy(B_d, B_h, bytes, cudaMemcpyHostToDevice);
	
	cudaDeviceSynchronize();
	
	CUDA_DEBUG
	
	free(A_h);
	free(B_h);
	
	long threadsPerBlock = 1024;
	long numBlocks = N / threadsPerBlock;
	numBlocks += (N % threadsPerBlock > 0) ?  1 : 0;
	
	gpu_mat_mul <<< numBlocks, threadsPerBlock >>> (A_d, B_d, C_d, size, N);
	cudaDeviceSynchronize();
	
	CUDA_DEBUG
	
	cudaMemcpy(C_h, C_d, bytes, cudaMemcpyDeviceToHost);
	
	cudaFree(C_d);
	cudaFree(B_d);
	cudaFree(A_d);
	
	CUDA_DEBUG
	
	free(C_h);
	
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(end - begin);
	cout << "Blocksize * Gridsize: " << setw(2) << threadsPerBlock * numBlocks << " Matrix size: " << setw(9) << size;
	cout << " Milliseconds taken: " << setw(15) << duration.count() << endl;
	return 0;
}
