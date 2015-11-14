#ifndef LIFE_MATRIX_H
#define LIFE_MATRIX_H

class LifeMatrix{
	bool* data = nullptr;
	int width, height;
public:
	void init(int w, int h);
	void alloc(int w, int h);
	void destroy();
	inline bool* operator()(const int x, const int y){int j=(x+width)%width; int k=(y+height)%height; return (data+j+k*width);}
	int nbors(int x, int y);
};

inline bool alive(bool state, int n) {return (n == 3) || (state && n == 2);};

#endif
