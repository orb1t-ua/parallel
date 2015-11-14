#include "mpi.h"

#include <iostream>
#include <iomanip>
#include <chrono>

#include "lifematrix.h"

#ifdef DEBUG
#define D(x) x
#else
#define D(x) 
#endif

using namespace std;

int main(int argc, char* argv[]){
	const auto begin = chrono::high_resolution_clock::now();
	int P=1, R=0;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &P);
	MPI_Comm_rank(MPI_COMM_WORLD, &R);
	if(argc != 4){
		if(0==R)
			cout << "Usage: mpiexec -n <P> " << argv[0] << " <M> <N> <generations>\n";
		return 1;
	}
	const int X = atoi(argv[1]);
	const int Y = atoi(argv[2]);
	const int G = atoi(argv[3]);
	int localY = Y / P;
	const int yStart = localY * R;
	const int yEnd   = (R == P-1) ? Y : yStart + localY;
	localY = (R == P-1) ? yEnd - yStart : localY;
	
	LifeMatrix lm[2];
	if(0 == R){
		lm[0].init(X, Y);
		lm[1].alloc(X, Y);
	}
	else{
		lm[0].alloc(X, localY+2);
		lm[1].alloc(X, localY+2);
	}
	
#define BCAST \
	if(0==R){	\
		for(int r=1; r<P-1; r++){	\
			MPI_Send(lm[0](0, r*localY - 1), (localY+2)*X, MPI_BYTE, r, 0, MPI_COMM_WORLD);	\
		}	\
		MPI_Send(lm[0](0, (P-1)*localY - 1), (localY+1)*X, MPI_BYTE, P-1, 0, MPI_COMM_WORLD);	\
		MPI_Send(lm[0](0, 0), X, MPI_BYTE, P-1, 0, MPI_COMM_WORLD);	\
	}	\
	else {	\
		if(R != (P-1))	\
			MPI_Recv(lm[0](0, 0), X*(localY+2), MPI_BYTE, 0, 0, MPI_COMM_WORLD, NULL);	\
		else {	\
			MPI_Recv(lm[0](0, 0), X*(localY+1), MPI_BYTE, 0, 0, MPI_COMM_WORLD, NULL);	\
			MPI_Recv(lm[0](0, localY+1), X, MPI_BYTE, 0, 0, MPI_COMM_WORLD, NULL);	\
		}	\
	}

#define GATHER \
	if(0==R){ \
		for(int r=1; r<P-1; r++){	\
			MPI_Recv(lm[k](0, localY*r), localY, MPI_BYTE, r, 0, MPI_COMM_WORLD, NULL);\
		}	\
		MPI_Recv(lm[k](0, localY*(P-1)), Y - (localY*(P-1)), MPI_BYTE, P-1, 0, MPI_COMM_WORLD, NULL);	\
	}	\
	else {	\
		MPI_Send(lm[k](0, 1), localY, MPI_BYTE, 0, 0, MPI_COMM_WORLD);	\
	}	
	
	for(int i = 0; i < G; i++){
		// double buffer indices
		const int j = (i) % 2;
		const int k = (i+1) % 2;
		BCAST
		// game of life
		for(int y = yStart; y < yEnd; y++){
			for(int x = 0; x < X; x++){
				*lm[k](x, y) = alive(*lm[j](x, y), lm[j].nbors(x, y));
			}
		}
		GATHER
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
