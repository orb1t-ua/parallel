#ifndef LIFE_MATRIX_H
#define LIFE_MATRIX_H

struct Offset{
	int i, bit;
};

class LifeMatrix{
	bool* data = nullptr;
	int width, height;
	void wrap(int& x, int& y);
public:
	void init(int w, int h);
	void destroy();
	bool at(int x, int y);
	void set(int x, int y, bool val);
	int getNumNeighbors(int x, int y);
	bool shouldLive(bool alive, int neighbors);
	void reset();
};

#endif
