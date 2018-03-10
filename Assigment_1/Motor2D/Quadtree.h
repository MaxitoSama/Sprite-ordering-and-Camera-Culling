#ifndef __QUADTREE_H__
#define __QUADTREE_H__ 

#define MAX_OBJECTS 10

#include "SDL/include/SDL_rect.h"
#include <vector>
#include <array>

class ObjectToPrint;

using namespace std;

class Quadtree
{
public:
	Quadtree()
	{
		Level = 0;
		Space = { 0,0,0,0 };
		Parent = nullptr;

		for (int i = 0; i < Childs.size(); i++)
		{
			Childs[i] = nullptr;
		}
	}

	Quadtree(int level,SDL_Rect rect,Quadtree* parent=nullptr)
	{
		Level = level;
		Space = rect;
		Parent = parent;

		for (int i = 0; i < Childs.size(); i++)
		{
			Childs[i] = nullptr;
		}
	}

	~Quadtree()
	{
		Clear();
	}

	void Clear();
	void Split();
	
	bool insert(ObjectToPrint* Object);
	bool CheckBoundaries(const SDL_Rect& r);
	

public:

	int					Level;
	
	SDL_Rect			Space;
	
	vector<ObjectToPrint*>		Objects;

	array<Quadtree*,4>	Childs;

	Quadtree*			Parent = nullptr;
};




#endif // !__QUADTREE_H__

