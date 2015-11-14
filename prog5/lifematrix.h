#ifndef LIFE_MATRIX_H
#define LIFE_MATRIX_H

class LifeMatrix{
	bool* data = nullptr;
	int width, height;
	inline void wrap(int& x, int& y){ x=(x+width)%width; y=(y+height)%height;}
public:
	void init(int w, int h);
	void alloc(int w, int h);
	void destroy();
	inline bool* operator()(const int x, const int y){int j=x; int k=y; wrap(j, k); return (data+j+k*width);}
	int nbors(int x, int y);
};

inline bool alive(bool state, int n) {return (n == 3) || (state && n == 2);};

#endif
