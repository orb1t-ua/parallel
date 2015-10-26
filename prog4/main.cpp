
#ifdef OMP_PARALLEL
#include <omp.h>
#endif

#include <iostream>
#include <iomanip>
#include <chrono>

#ifdef EXPORT
#include <fstream>
#endif

#include "lifematrix.h"

using namespace std;

int main(int argc, char* argv[]){
	auto begin = chrono::high_resolution_clock::now();
	if(argc != 5){
		cout << "Usage: ./progname <threads> <M> <N> <generations>\n";
		return 1;
	}
	int P = atoi(argv[1]);
	int X = atoi(argv[2]);
	int Y = atoi(argv[3]);
	int G = atoi(argv[4]);
	
	LifeMatrix lm[2];
	lm[0].init(X, Y);
	lm[1].init(X, Y);
	
	#ifdef EXPORT
	ofstream stream("animation.txt");
	P = 1;
	#endif
	
	for(int i = 0; i < G; i++){
		#pragma omp parallel for num_threads(P)
		for(int y = 0; y < Y; y++){
			for(int x = 0; x < X; x++){
				int j = i%2;
				int k = (i+1)%2;
				int neighbors = lm[j].getNumNeighbors(x, y);
				bool alive = lm[j].at(x, y);
				bool shouldLive = lm[j].shouldLive(alive, neighbors);
				lm[k].set(x, y, shouldLive);
				
				#ifdef EXPORT
				if(alive){
					stream << "1";
				}
				else{
					stream << "0";
				}
				#endif
			}
			#ifdef EXPORT
			stream << endl;
			#endif
		}
		#ifdef EXPORT
		stream << endl;
		#endif
	}
	
	#ifdef EXPORT
	stream.close();
	#endif
	
	lm[0].destroy();
	lm[1].destroy();
	
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::seconds>(end - begin);
	cout << "P: " << setw(3) << P << " M: " << setw(9) << X;
	cout << " N: " << setw(9) << Y << " G: " << setw(9) << G;
	cout << " seconds: " << setw(15) << duration.count() << endl;
	
	return 0;
}
