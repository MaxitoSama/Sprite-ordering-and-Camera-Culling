#include "Quadtree.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Colliders.h"


void Quadtree::Clear()
{
	//First we have to clear all the objects of the node
	list<ObjectToPrint*>::iterator item = Objects.begin();

	for (item;item!=Objects.end();item++)
	{
		if (*item == nullptr)
		{
			RELEASE(*item);
		}
	}
	
	Objects.clear();

	//Now we have to call all the children of the node and check if they have also childre 
	for (int i = 0; i < Children.size(); i++)
	{
		if (Children[i] != nullptr)
		{
			Children[i]->Clear();
			Children[i] = nullptr;
		}
	}

}

void Quadtree::Split()
{
	int x, y, w, h;

	x = this->Space.x;
	y = this->Space.y;
	w = (int)(this->Space.w / 2);
	h = (int)(this->Space.h / 2);

	Children[0] = new Quadtree({ x,	y, w, h },Level+1);
	Children[1] = new Quadtree({ x + w, y, w, h },Level+1);
	Children[2] = new Quadtree({ x, y + h, w, h }, Level + 1);
	Children[3] = new Quadtree({ x + w, y + h, w, h }, Level + 1);

}

// Index Quadrant
/*
---------
| 0 | 1 |
---------
| 2	| 3 |
---------
*/

int Quadtree::getIndex(const SDL_Rect& r)
{
	int index = -1;
	float MidWeight = Space.x+(Space.w/2);
	float MidHeight = Space.y+(Space.h/2);

	bool TopChildren = (r.y < MidHeight && r.y+r.h<MidHeight);
	bool BottomChildren = (r.y > MidHeight);

	if (r.x < MidWeight && r.x + r.w < MidWeight)
	{
		if (TopChildren)
		{
			index = 0;
		}
		else
		{
			index = 2;
		}
	}
	else if (r.x > MidWeight)
	{
		if (TopChildren)
		{
			index = 1;
		}
		else
		{
			index = 3;
		}
	}

	return index;
}

bool Quadtree::insert(ObjectToPrint* obj)
{
	////The objects is empty so we don't add it
	//if (obj == nullptr)
	//{
	//	return false;
	//}

	////The object it's not inside the Rectangle Space
	//if (CheckBoundaries(obj->rect) == false)
	//{
	//	return false;
	//}

	if (Children[0] != nullptr)
	{
		int index = getIndex(obj->rect);
		if (index = !- 1)
		{
			Children[index]->insert(obj);
		}
		return true;
	}

	Objects.push_back(obj);

	if (Objects.size() > MAX_OBJECTS && Level<MAX_LEVELS)
	{
		if (Children[0] == nullptr)
		{
			Split();
		}

		for (list<ObjectToPrint*>::iterator item = Objects.begin(); item != Objects.end(); item++)
		{
			int index = getIndex((*item)->rect);
			if (index != -1)
			{
				Children[index]->insert(*item);
				Objects.remove(*item);
			}
		}
	}

	return true;	
}

vector<ObjectToPrint*> Quadtree::FillCollisionList(vector<ObjectToPrint*> &ObjList, const SDL_Rect& camera)
{
	int index = getIndex(camera);
	if (index != -1 && Children[0] != nullptr)
	{
		Children[index]->FillCollisionList(ObjList, camera);
	}

	for (list<ObjectToPrint*>::iterator item = Objects.begin(); item != Objects.end(); item++)
	{
		ObjList.push_back(*item);
	}

	
	return ObjList;
}

bool Quadtree::CheckBoundaries(const SDL_Rect& r)
{
	if ((Space.x < r.x + r.w && Space.x + Space.w > r.x) || (Space.x < r.x + r.w  && Space.x + Space.w > r.x))
	{
		if (Space.y < r.y + r.h && Space.y + Space.h > r.y)
		{
			return true;
		}
	}
	return false;
}
