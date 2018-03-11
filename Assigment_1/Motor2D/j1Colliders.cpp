#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Colliders.h"
#include "j1Map.h"
#include "j1Entities.h"
#include "Entity.h"
#include "Player.h"
#include "j1Pathfinding.h"
#include "j1Scene.h"

j1Colliders::j1Colliders() : j1Module()
{
	name.create("colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}
}

// Destructor
j1Colliders::~j1Colliders()
{
}

bool j1Colliders::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool j1Colliders::Start()
{

	int map_width = App->map->data.width*App->map->data.tile_width;
	int map_height = App->map->data.height*App->map->data.tile_height;

	ColQuadtree=new Quadtree({ 0,0,map_width,map_height });

	return true;
}

bool j1Colliders::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate Colliders ", Profiler::Color::Red)
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

// Called before render is available
bool j1Colliders::Update(float dt)
{
	BROFILER_CATEGORY("Update Colliders ", Profiler::Color::PaleVioletRed)

	
	Collider* c1=nullptr;
	Collider* c2=nullptr;

	ColQuadtree->Clear();

	if (App->entities->Slowmo)
	{
		Slowdt = dt / 4;
	}
	else
	{
		Slowdt = dt;
	}

	numberCollisions_1 = 0;

	//for (uint i = 0; i < MAX_COLLIDERS; ++i)
	//{
	//	// skip empty colliders
	//	if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE)
	//	{
	//		continue;
	//	}

	//	c1 = colliders[i];

	//	for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
	//	{
	//		// skip empty colliders
	//		if (colliders[k] == nullptr)
	//		{
	//			continue;
	//		}

	//		c2 = colliders[k];
	//		
	//		for (uint i = 0; i < MAX_ENEMIES; ++i)
	//		{
	//			if (App->entities->entities[i] != nullptr)
	//			{
	//				if (c2 == App->entities->entities[i]->GetCollider() && c1->type == COLLIDER_FLOOR && c2->type == COLLIDER_ENEMY && c1->CheckFutureFallColision(c2->rect, distance,Slowdt, App->entities->entities[i]->speed.y) == true)
	//				{
	//					App->entities->entities[i]->original_pos.y -= distance;
	//				}
	//				if (c2 == App->entities->entities[i]->GetCollider() && c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WALL && c2->CheckFutureCrashColision(c1->rect, distance, App->entities->player->speed.x) == true)
	//				{
	//					App->entities->player->original_pos.x -= distance;
	//				}
	//			}
	//		}

	//		if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY && c1->CheckFutureCrashColision(c2->rect, distance, App->entities->player->speed.x) == true && !App->entities->player->GOD)
	//		{
	//			App->entities->player->dead = true;
	//			App->entities->player->dead_by_entity = true;
	//			App->entities->Slowmo = false;
	//		}

	//		if ( App->entities->player != nullptr && c1->type == COLLIDER_FLOOR && c2->type == COLLIDER_FEET && c1->CheckFutureFallColision(c2->rect, distance, dt, App->entities->player->gravity) == true)
	//		{
	//			if(c2 == App->entities->player->collider_feet)
	//			{
	//				App->entities->player->original_pos.y -= distance;
	//				App->entities->player->dead = false;

	//				if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
	//				{
	//					App->entities->player->Jump = false;
	//					App->entities->player->fall = false;
	//				}
	//			}
	//		}

	//		numberCollisions_1++;
	//	}
	//}

	CollisionQuadtree();

	DebugDraw();

	return true;
}

void j1Colliders::DebugDraw()
{
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		LOG("YOu can see the colliders");
		debug = !debug;
	}

	if (debug == false) {
		return;
	}
	else {

		Uint8 alpha = 255;
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == nullptr)
				continue;

			switch (colliders[i]->type)
			{
			case COLLIDER_NONE: // white
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha,false);
				break;
			case COLLIDER_WALL: // blue
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha,false);
				break;
			case COLLIDER_PLAYER: // green
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha,false);
				break;
			case COLLIDER_DEATH: // pink
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha, false);
				break;
			case COLLIDER_FLOOR: // light blue
				App->render->DrawQuad(colliders[i]->rect, 135, 206, 250, alpha, false);
				break;
			case COLLIDER_FEET: // light blue
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 0, alpha, false);
				break;
			case COLLIDER_WIN: // light blue
				App->render->DrawQuad(colliders[i]->rect, 0, 206, 250, alpha, false);
				break;
			case COLLIDER_WIN2: // light blue
				App->render->DrawQuad(colliders[i]->rect, 0, 206, 250, alpha, false);
				break;
			case COLLIDER_ENEMY: // light blue
				App->render->DrawQuad(colliders[i]->rect, 100, 206, 250, alpha, false);
				break;
			case COLLIDER_HEAD: // black
				App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha, false);
				break;
			case COLLIDER_COIN: // black
				App->render->DrawQuad(colliders[i]->rect, 0, 100, 0, alpha, false);
				break;
			}
		}
	}
}

bool j1Colliders::checkColisionList(Collider * enemCollider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		if (colliders[i] == nullptr)
			continue;
		if (enemCollider->CheckCollision(colliders[i]->rect))
			return true;
	}
	return false;
}

bool j1Colliders::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Colliders::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

bool j1Colliders::EraseCollider(Collider* collider)
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == collider)
		{
			delete colliders[i];
			colliders[i] = nullptr;
			return true;
		}
	}

	return false;
}

void j1Colliders::CollisionQuadtree()
{
	Collider* col=nullptr;
	numberCollisions_2 = 0;

	for (int i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE)
		{
			continue;
		}

		ColQuadtree->insert(colliders[i]);
	}

	for (int i = 0; i < MAX_COLLIDERS; i++)
	{
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE)
		{
			continue;
		}

		col = colliders[i];

		ColQuadtree->FillCollisionList(ListToCollide, col);

		for (list<Collider*>::iterator item = ListToCollide.begin(); item != ListToCollide.end(); item++)
		{
			if (col != *item)
			{
				if (App->entities->entities[i] != nullptr)
				{
					if (*item == App->entities->entities[i]->GetCollider() && col->type == COLLIDER_FLOOR && (*item)->type == COLLIDER_ENEMY && col->CheckFutureFallColision((*item)->rect, distance, Slowdt, App->entities->entities[i]->speed.y) == true)
					{
						App->entities->entities[i]->original_pos.y -= distance;
					}
					if (*item == App->entities->entities[i]->GetCollider() && col->type == COLLIDER_PLAYER && (*item)->type == COLLIDER_WALL && (*item)->CheckFutureCrashColision(col->rect, distance, App->entities->player->speed.x) == true)
					{
						App->entities->player->original_pos.x -= distance;
					}
				}
			}
			numberCollisions_2++;
		}

		/*for (list<Collider*>::iterator item = ListToCollide.begin(); item != ListToCollide.end(); item++)
		{
			RELEASE(*item);
		}*/

		ListToCollide.clear();
	}
}

// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r)const
{
	if ((rect.x < r.x + r.w && rect.x + rect.w > r.x) || (rect.x < r.x + r.w  && rect.x + rect.w > r.x))
	{
		if (rect.y < r.y + r.h && rect.y + rect.h > r.y)
		{
			return true;
		}
	}
	return false;
}

bool Collider::CheckFutureFallColision(const SDL_Rect& r, float& distance, float dt,float speed)
{
	speed = speed*dt;

	if (rect.x < r.x + r.w && rect.x + rect.w > r.x)
	{
		if (rect.y < r.y + r.h + speed && rect.y && rect.y + rect.h > r.y)
		{
			distance = r.y + r.h + speed - rect.y;
			return true;
		}
	}
	return false;
}


bool Collider::CheckFutureCrashColision(const SDL_Rect& r, float& distance, float speed)
{
	if (speed >0)
	{
		if (rect.x  < r.x + r.w + speed && rect.x + rect.w > r.x)
		{
			if (rect.y < r.y + r.h  && rect.y + rect.h > r.y)
			{
				distance = r.x + r.w + speed - rect.x;
				return true;
			}
		}
	}
	if (speed <0)
	{
		if (rect.x < r.x + r.w && rect.x + rect.w > r.x+speed)
		{
			if (rect.y < r.y + r.h  && rect.y + rect.h > r.y)
			{
				distance = r.x-(rect.x + rect.w -speed);
				return true;
			}
		}
	}
	
	return false;
}


