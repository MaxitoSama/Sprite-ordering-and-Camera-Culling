#ifndef __QUADTREE_H__
#define __QUADTREE_H__ 

#define MAX_OBJECTS 10

#include "SDL/include/SDL_rect.h"
#include <vector>
#include <array>
#include <list>

class ObjectToPrint;
struct Collider;

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
	
	bool insert(Collider* Object);
	bool CheckBoundaries(const SDL_Rect& r);

	list<Collider*> FillCollisionList(list<Collider*> &CollidersList,Collider* collider);

public:
	
	SDL_Rect			Space;
	
	list<Collider*>		Objects;

	array<Quadtree*,4>	Children;

};




#endif // !__QUADTREE_H__

