#include "House.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Entities.h"
#include "Player.h"
#include "j1Colliders.h"




House::House(int x, int y, ENTITY_TYPES type):Entity(x, y, type)
{
	scale = 1.0f;

	App->tex->GetSize(App->entities->sprites_house, image_size_w, image_size_h);

	priority = 3;

	collider = App->colliders->AddCollider({(int)position.x,(int)position.y+110,(int)image_size_w,(int)image_size_h-220}, COLLIDER_WALL, (j1Module*)App->entities);

}


House::~House()
{
}

bool House::Awake(pugi::xml_node&)
{
	return true;
}

bool House::Update(float dt)
{
	//TODO 1: Create a method that prints the house befor the player or after the player

	if (App->entities->player->position.y < position.y + image_size_h / 2)
	{
		priority = App->entities->player->priority + 1;
	}
	else
	{
		priority = App->entities->player->priority - 1;
	}
	return true;
}
