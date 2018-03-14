#ifndef __QUADTREE_H__
#define __QUADTREE_H__ 

#define MAX_OBJECTS 5
#define MAX_LEVELS 5

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

		Level = 0;
	}

	Quadtree(SDL_Rect rect, int level)
	{
		Space = rect;

		for (int i = 0; i < Children.size(); i++)
		{
			Children[i] = nullptr;
		}
		Level = level;
	}

	~Quadtree()
	{
		Clear();
	}

	void Clear();
	void Split();
	int getIndex(const SDL_Rect& r);
	
	bool insert(ObjectToPrint* Object);
	bool CheckBoundaries(const SDL_Rect& r);

	vector<ObjectToPrint*> FillCollisionVector(vector<ObjectToPrint*> &ObjList, const SDL_Rect& camera);

public:
	
	int Level;

	SDL_Rect				Space;
	
	list<ObjectToPrint*>	Objects;

	array<Quadtree*,4>		Children;

};
#endif // !__QUADTREE_H__

