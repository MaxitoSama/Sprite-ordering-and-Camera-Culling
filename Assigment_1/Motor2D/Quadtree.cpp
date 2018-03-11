#include "Quadtree.h"
#include "j1App.h"
#include "j1Render.h"


void Quadtree::Clear()
{
	//First we have to clear all the objects of the node
	for (int i = 0; i < Objects.size(); i++)
	{
		if (Objects[i] = nullptr)
		{
			RELEASE(Objects[i]);
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

bool Quadtree::insert(ObjectToPrint* Object)
{
	//The objects is empty so we don't add it
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
		if (Children[0] == nullptr)
		{
			Split();
		}

		bool ret = false;

		//Do the same process for the childs
		for (int i = 0; i < Children.size() && ret == false; i++)
		{
			ret = Children[i]->insert(Object);
		}
	}
}

void Quadtree::FillCameraQueue()
{
	for (int i = 0; i < Children.size(); i++)
	{
		//Check if the current node has children and if it's true then goes ther and does the same
		if (Children[i] != nullptr)
		{
			Children[i]->FillCameraQueue();
		}
		else //If the node hasn't children then fill the priority Queue
		{
			if (App->render->CameraCollision(this->Space))
			{
				for (int j = 0; j < Objects.size(); j++)
				{
					App->render->FillQueue_v_2(Objects[j]);
				}
			}
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
