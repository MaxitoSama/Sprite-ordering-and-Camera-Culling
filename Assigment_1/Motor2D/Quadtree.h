#ifndef __QUADTREE_H__
#define __QUADTREE_H__ 

#include "SDL/include/SDL_rect.h"
#include <vector>
#include <array>

using namespace std;

template<tdata>
class Quadtree
{
public:
	Quadtree(){}

	Quadtree(int level,SDL_Rect rect,Quadtree* parent=nullptr)
	{
		Level = level;
		Space = rect;
		Parent = parent;
	}

	~Quadtree()
	{}

	void split()
	{
		int x, y, w, h;
		
		x = this->Space.x;
		y = this->Space.y;
		w = (this->Space.w/2);
		h = (this->Space.h/2);

		Childs[0] = new Quadtree(this->Level + 1, { x,	y, w, h }, this);
		Childs[1] = new Quadtree(this->Level + 1, { x + w, y, w, h }, this);
		Childs[2] = new Quadtree(this->Level + 1, { x, y + h, w, h }, this);
		Childs[3] = new Quadtree(this->Level + 1, { x + w, y + h, w, h }, this);
	}

public:

	int					Level;
	
	SDL_Rect			Space;
	
	vector<tdata*>		Objects;

	array<Quadtree*,4>	Childs;

	Quadtree*			Parent = nullptr;
};




#endif // !__QUADTREE_H__

