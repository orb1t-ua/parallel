#include "mpi.h"

#include <iostream>
#include <iomanip>
#include <chrono>

#include "lifematrix.h"

using namespace std;

int main(int argc, char* argv[]){
	const auto begin = chrono::high_resolution_clock::now();
	int P=1, R=0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	MPI_Comm_rank(MPI_COMM_WORLD, &R);
	if(argc != 4 && 0 == R){
		cout << "Usage: ./progname <M> <N> <generations>\n";
		return 1;
	}
	const int X = atoi(argv[1]);
	const int Y = atoi(argv[2]);
	const int G = atoi(argv[3]);
	const int localY = Y / P;
	const int yStart = localY * R;
	const int yEnd   = (R == P-1) ? Y : yStart + localY;
	
	LifeMatrix lm[2];
	if(0 == R){
		cout << "M: " << X << " N: " << Y << " G: " << G << endl;
		lm[1].init(X, Y);
	}
	else{
		lm[1].alloc(X, Y);
	}
	lm[2].alloc(X, Y);
	cout << "Rank " << R << " starts at y= " << yStart << " and ends at: " << yEnd << endl;
	MPI_Barrier(MPI_COMM_WORLD);
	if(0 == R){
		cout << "Barrier completed" << endl;
	}
	MPI_Bcast(lm[0](0, 0), X*Y, MPI_C_BOOL, 0, MPI_COMM_WORLD);
	if(0 == R){
		cout << "Broadcast completed" << endl;
	}
	
	for(int i = 0; i < G; i++){
		// double buffer indices
		const int j = (i) % 2;
		const int k = (i+1) % 2;
		// game of life
		for(int y = yStart; y < yEnd; y++){
			for(int x = 0; x < X; x++){
				*lm[k](x, y) = lm[j].shouldLive(*lm[j](x, y), lm[j].getNumNeighbors(x, y));
			}
		}
		cout << "Rank " << R << " finished simulation on G " << i << endl;
		// interprocess communication
		if(0 == R){
			for(int r = 1; r < P; r++){
				MPI_Recv(lm[k](0, localY*r), localY, MPI_C_BOOL, r, 0, MPI_COMM_WORLD, NULL);
				cout << "Recv'd from " << r << endl;
			}
		}
		else {
			MPI_Send(lm[k](0, yStart), localY, MPI_C_BOOL, 0, 0, MPI_COMM_WORLD);
			cout << "Sent from " << R << " to 0\n";
		}
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(lm[k](0, 0), X*Y, MPI_C_BOOL, 0, MPI_COMM_WORLD);
		if(0 == R){
			cout << "G " << i << " finished\n";
		}
	}
	
	lm[0].destroy();
	lm[1].destroy();
	MPI_Finalize();
	
	const auto end = chrono::high_resolution_clock::now();
	const auto duration = chrono::duration_cast<chrono::seconds>(end - begin);
	if(0 == R){
		cout << "P: " << setw(3) << P << " M: " << setw(9) << X;
		cout << " N: " << setw(9) << Y << " G: " << setw(9) << G;
		cout << " seconds: " << setw(15) << duration.count() << endl;
	}
	
	return 0;
}
