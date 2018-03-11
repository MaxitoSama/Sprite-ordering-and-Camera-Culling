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
		Space = { 0,0,0,0 };

		for (int i = 0; i < Children.size(); i++)
		{
			Children[i] = nullptr;
		}
	}

	Quadtree(SDL_Rect rect)
	{
		Space = rect;

		for (int i = 0; i < Children.size(); i++)
		{
			Children[i] = nullptr;
		}
	}

	~Quadtree()
	{
		Clear();
	}

	void Clear();
	void Split();
	void FillCameraQueue();
	
	bool insert(ObjectToPrint* Object);
	bool CheckBoundaries(const SDL_Rect& r);
	

public:
	
	SDL_Rect				Space;
	
	vector<ObjectToPrint*>	Objects;

	array<Quadtree*,4>		Children;

};




#endif // !__QUADTREE_H__

