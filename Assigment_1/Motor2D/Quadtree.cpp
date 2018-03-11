#include "Quadtree.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Colliders.h"


void Quadtree::Clear()
{
	//First we have to clear all the objects of the node
	list<Collider*>::iterator item = Objects.begin();

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
	w = (this->Space.w / 2);
	h = (this->Space.h / 2);

	Children[0] = new Quadtree({ x,	y, w, h });
	Children[1] = new Quadtree({ x + w, y, w, h });
	Children[2] = new Quadtree({ x, y + h, w, h });
	Children[3] = new Quadtree({ x + w, y + h, w, h });

}

bool Quadtree::insert(Collider* collider)
{
	//The objects is empty so we don't add it
	if (collider == nullptr)
	{
		return false;
	}

	//The object it's not inside the Rectangle Space
	if (CheckBoundaries(collider->rect) == false)
	{
		return false;
	}

	//If it's inside the Space, check if we have to split or not
	if (Objects.size() < MAX_OBJECTS)
	{
		Objects.push_back(collider);
		return true;
	}
	else
	{
		//Check if the space has children and if not, create the children
		if (Children[0] == nullptr)
		{
			Split();
		}

		bool ret = false;

		//Do the same process for the childs
		for (int i = 0; i < Children.size() && ret == false; i++)
		{
			Objects.remove(collider);
			ret = Children[i]->insert(collider);
		}
	}
}

list<Collider*> Quadtree::FillCollisionList(list<Collider*> &CollidersList, Collider* collider)
{
	bool ret = false;

	for (list<Collider*>::iterator item = Objects.begin(); item != Objects.end(); item++)
	{
		if (collider == *item)
		{
			ret = true;
			break;
		}
	}

	if (ret == true)
	{
		for (list<Collider*>::iterator item = Objects.begin(); item != Objects.end(); item++)
		{
			CollidersList.push_back(*item);
		}
	}
	else
	{
		if (Children[0] != nullptr)
		{
			for (int i = 0; i < Children.size(); i++)
			{
				Children[i]->FillCollisionList(CollidersList, collider);
			}
		}
	}

	return CollidersList;
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
