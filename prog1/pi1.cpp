#include <pthread.h>
#include "stdlib.h"
#include "stdio.h"

#include <chrono>
#include <iostream>
#include <iomanip>

long num_threads;
long iterations;

double sum;

using namespace std;

int main(int argc, char* argv[]){
	if(argc != 2){
		cout << "Usage: ./progname <iterations>\n";
		return 1;
	}
	num_threads = 1;
	iterations = atol(argv[1]);
	
	sum = 0.0;
	
	auto begin = chrono::high_resolution_clock::now();

	double factor = 1.0;
	for(long i = 0; i < iterations; i++, factor = -factor){
		sum += factor / (2.0 * i + 1);
	}
	sum *= 4.0;

	auto end = chrono::high_resolution_clock::now();
	
	auto duration = chrono::duration_cast<chrono::nanoseconds>(end - begin);
	
	cout << "pi1.cpp: num threads: " << setw(2) << num_threads << " iterations: " << setw(9) << iterations;
	cout << " nanoseconds taken: " << setw(15) << duration.count() << " final sum: " << setw(12) << sum << endl;

	return 0;	
}

