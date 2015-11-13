#ifndef LIFE_MATRIX_H
#define LIFE_MATRIX_H

class LifeMatrix{
	bool* data = nullptr;
	int width, height;
	void wrap(int& x, int& y);
public:
	void init(int w, int h);
	void alloc(int w, int h);
	void destroy();
	inline bool* operator()(const int x, const int y){int j = x; int k = y; wrap(j, k); return (data + j + k*width);}
	int getNumNeighbors(int x, int y);
	bool shouldLive(bool alive, int neighbors);
};

#endif
