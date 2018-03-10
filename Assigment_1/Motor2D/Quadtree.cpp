#include "Quadtree.h"
#include "j1Render.h"


void Quadtree::Clear()
{

}

void Quadtree::Split()
{
	int x, y, w, h;

	x = this->Space.x;
	y = this->Space.y;
	w = (this->Space.w / 2);
	h = (this->Space.h / 2);

	Childs[0] = new Quadtree(this->Level + 1, { x,	y, w, h }, this);
	Childs[1] = new Quadtree(this->Level + 1, { x + w, y, w, h }, this);
	Childs[2] = new Quadtree(this->Level + 1, { x, y + h, w, h }, this);
	Childs[3] = new Quadtree(this->Level + 1, { x + w, y + h, w, h }, this);

}

bool Quadtree::insert(ObjectToPrint* Object)
{
	//The objects is empty so we don't need it
	if (Object == nullptr)
	{
		return false;
	}

	//The object it's not inside the Rectangle Space
	if (CheckBoundaries(Object->rectangle) == false)
	{
		return false;
	}

	//If it's inside the Space, check if we have to split or not
	if (Objects.size() < MAX_OBJECTS)
	{
		Objects.push_back(Object);
		return true;
	}
	else
	{
		Split();

		bool ret = false;

		//Do the same process for the childs
		for (int i = 0; i < Childs.size() && ret == false; i++)
		{
			ret = Childs[i]->insert(Object);
		}
	}
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
