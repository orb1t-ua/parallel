#include "lifematrix.h"

#include <random>
#include <time.h>

void LifeMatrix::init(int w, int h){
	if(data != nullptr){
		return;	
	}
	width = w; height = h;
	data = new bool[width * height];
	srand(time(NULL));
	for(long i = 0; i < width * height; i++){
		*(data + i) = rand()%2 == 0;
	}
}

void LifeMatrix::alloc(int w, int h){
	if(data != nullptr){
		return;	
	}
	width = w; height = h;
	data = new bool[width * height];
}

void LifeMatrix::destroy(){
	delete[] data;
	data = nullptr;
}

void LifeMatrix::wrap(int& x, int& y){
    x = (x+width)%width;
    y = (y+height)%height;
}

int LifeMatrix::nbors(int x, int y){
	int i = 0;
	for(int j = y-1; j < y+1; j++){
		for(int k = x-1; k < x+1; k++){
			i = *(*this)(k, j) ? i + 1 : i;
		}
	}
	return i - 1;
}

