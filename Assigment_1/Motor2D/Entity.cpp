#include "j1App.h"
#include "Entity.h"
#include "j1Entities.h"
#include "j1Colliders.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "Player.h"
#include "SDL/include/SDL_timer.h"
#include "j1Audio.h"

Entity::Entity(int x, int y, ENTITY_TYPES type) : position(x, y), original_pos(x, y),type(type)
{
}

bool Entity::Awake(pugi::xml_node& enemy)
{
	Awake(enemy);

	return true;
}

Entity::~Entity()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

void Entity::Draw(SDL_Texture* sprites, float direction, float dt)
{

	if (collider != nullptr)
	{
		
		if (collider->type == COLLIDER_PLAYER && App->entities->player->slide)
		{
			collider->SetPos((int)position.x, (int)position.y+40);
		}
		else
		{
			collider->SetPos((int)position.x+45, (int)position.y);
		}

		if (collider->type == COLLIDER_ENEMY && alive)
		{
			collider->SetPos((int)position.x+25, (int)position.y + 5);
		}
		if (collider->type == COLLIDER_COIN )
		{
			collider->SetPos((int)position.x, (int)position.y);
		}
	}

	if (collider_feet != nullptr)
	{
		collider_feet->SetPos((position.x + (263 / 4) - 54), position.y + (310 / 2) - 1);
	}

	if (collider_head != nullptr && alive)
	{
		collider_head->SetPos(position.x + 30, position.y-20);
	}
		
	if (animation != nullptr)
	{		
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame(dt)), direction, 1.0f);
	}
	
	if (type == COIN)
	{
		App->render->Blit(sprites, position.x, position.y,NULL, scale);
	}
}

const Collider* Entity::GetCollider() const
{
	return collider;
}

const Collider* Entity::GetColliderFloor() const
{
	return collider_feet;
}

const Collider* Entity::GetColliderHead() const
{
	return collider_head;
}

const ENTITY_TYPES Entity::GetType()const
{
	return type;
}