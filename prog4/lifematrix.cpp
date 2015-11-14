#include "lifematrix.h"

#include <random>
#include <time.h>

void LifeMatrix::init(int w, int h){
	if(data != nullptr)
		return;	
	width = w; height = h;
	data = new bool[width * height];
	srand(time(NULL));
	for(long i = 0; i < width * height; i++)
		*(data + i) = (bool)rand()%2;
}

void LifeMatrix::alloc(int w, int h){
	if(data != nullptr)
		return;	
	width = w; height = h;
	data = new bool[width * height];
}

void LifeMatrix::destroy(){
	delete[] data;
	data = nullptr;
}

int LifeMatrix::nbors(int x, int y){
	int i = (int)*(*this)(x-1, y-1);
	i += (int)*(*this)(x, y-1);
	i += (int)*(*this)(x+1, y-1);
	i += (int)*(*this)(x-1, y);
	i += (int)*(*this)(x+1, y);
	i += (int)*(*this)(x-1, y+1);
	i += (int)*(*this)(x, y+1);
	i += (int)*(*this)(x+1, y+1);
	return i;
}

