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

void LifeMatrix::destroy(){
	delete[] data;
	data = nullptr;
}

void LifeMatrix::reset(){
	for(long i = 0; i < width * height; i++){
		*(data + i) = rand()%2 == 0;
	}
}

void LifeMatrix::wrap(int& x, int& y){
	while(x < 0){
		x += width;
	}
	while(x >= width){
		x -= width;
	}
	while(y < 0){
		y += height;
	}
	while(y >= height){
		y -= height;
	}
}

bool LifeMatrix::at(int x, int y){
	wrap(x, y);
	return *(data + width*y + x);
}

void LifeMatrix::set(int x, int y, bool val){
	wrap(x, y);
	*(data + width*y + x) = val;
}

int LifeMatrix::getNumNeighbors(int x, int y){
	int i = 0;
	for(int j = y-1; j < y+1; j++){
		for(int k = x-1; k < x+1; k++){
			i = at(k, j) ? i + 1 : i;
		}
	}
	return i - 1;
}

bool LifeMatrix::shouldLive(bool alive, int neighbors){
	return (neighbors == 3) || (alive && neighbors == 2);
}
