#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Colliders.h"
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

	if (App->entities->Slowmo)
	{
		Slowdt = dt / 4;
	}
	else
	{
		Slowdt = dt;
	}

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr || colliders[i]->type == COLLIDER_NONE)
		{
			continue;
		}

		c1 = colliders[i];

		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}

			c2 = colliders[k];

			if (c1->type == COLLIDER_WALL && c2->type == COLLIDER_PLAYER && c1->CheckFutureCrashColision(c2->rect, distance, App->entities->player->speed.x) == true
				&& (App->input->GetKey(SDL_SCANCODE_D )==KEY_REPEAT|| App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT))
			{
				App->entities->player->original_pos.x -= distance;
			}

			if (c1->type == COLLIDER_DEATH && c2->type == COLLIDER_PLAYER && c1->CheckCollision(c2->rect) == true)
			{
				App->entities->player->dead = true;
				App->entities->player->dead_by_fall = true;
				App->entities->Slowmo = false;
			}

			if (c1->type == COLLIDER_WIN && c2->type == COLLIDER_FEET && c1->CheckCollision(c2->rect) == true)
			{
				App->scene->ChangeScene(60,215);
			}
			
			if (c1->type == COLLIDER_WIN2 && c2->type == COLLIDER_FEET && c1->CheckCollision(c2->rect) == true)
			{
				App->scene->ChangeScene(60, 215);
				App->entities->player->score = 0;
			}
			
			for (uint i = 0; i < MAX_ENEMIES; ++i)
			{
				if (App->entities->entities[i] != nullptr)
				{
					if (c2 == App->entities->entities[i]->GetCollider() && c1->type == COLLIDER_WALL && c2->type == COLLIDER_ENEMY && c1->CheckFutureCrashColision(c2->rect, distance, App->entities->entities[i]->speed.x) == true)
					{
						App->entities->entities[i]->original_pos.x -= distance;
					}
					if (c2 == App->entities->entities[i]->GetCollider() && c1->type == COLLIDER_FLOOR && c2->type == COLLIDER_ENEMY && c1->CheckFutureFallColision(c2->rect, distance,Slowdt, App->entities->entities[i]->speed.y) == true)
					{
						App->entities->entities[i]->original_pos.y -= distance;
					}
					if (c2 == App->entities->entities[i]->GetColliderHead() && c1->type == COLLIDER_FEET && c2->type == COLLIDER_HEAD && c1->CheckFutureFallColision(c2->rect, distance, dt, App->entities->entities[i]->speed.y) == true)
					{
						if (!App->entities->player->dead)
						{
							App->entities->player->original_pos.y -= distance + 900 * dt;
							App->entities->entities[i]->collider->to_delete = true;
							App->entities->entities[i]->collider = nullptr;
							App->entities->entities[i]->collider_head->to_delete = true;
							App->entities->entities[i]->collider_head = nullptr;
							App->entities->entities[i]->alive = false;

							if (App->entities->entities[i]->GetType() == ZOMBIE)
							{
								App->entities->player->score += 500;
							}
							else
							{
								App->entities->player->score += 1000;
							}
						}
					}
					if (c2 == App->entities->entities[i]->GetCollider() && c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_COIN && c1->CheckCollision(c2->rect) == true)
					{
						App->entities->player->coins ++;
						EraseCollider(App->entities->entities[i]->collider);
						App->entities->entities[i]->alive = false;
					}
				}
			}

			if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY && c1->CheckFutureCrashColision(c2->rect, distance, App->entities->player->speed.x) == true && !App->entities->player->GOD)
			{
				App->entities->player->dead = true;
				App->entities->player->dead_by_entity = true;
				App->entities->Slowmo = false;
			}

			if ( App->entities->player != nullptr && c1->type == COLLIDER_FLOOR && c2->type == COLLIDER_FEET && c1->CheckFutureFallColision(c2->rect, distance, dt, App->entities->player->gravity) == true)
			{
				if(c2 == App->entities->player->collider_feet)
				{
					App->entities->player->original_pos.y -= distance;
					App->entities->player->dead = false;

					if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_IDLE)
					{
						App->entities->player->Jump = false;
						App->entities->player->fall = false;
					}
				}
			}
		}
	}

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

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (App->entities->entities[i] != nullptr)
			{
				App->pathfinding->DrawPath(App->entities->entities[i]->Enemypath);
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


