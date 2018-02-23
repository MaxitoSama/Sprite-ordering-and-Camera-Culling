#include "Enemy_Plane.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Colliders.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Pathfinding.h"
#include "j1Entities.h"
#include "Player.h"
#include "j1Scene.h"

Enemy_Plane::Enemy_Plane(int x, int y, ENTITY_TYPES type): Entity(x, y,type)
{

	//Set animation steps, speed and loop
	anim.PushBack({ 49, 40, 639, 412 });
	//anim.PushBack({ 806, 40, 639, 412 });
	anim.speed = 18.0f;
	anim.loop = true;

	moving.PushBack({ 49, 40, 639, 412 });
	moving.PushBack({ 49, 37, 639, 412 });
	moving.PushBack({ 806, 40, 639, 412 });
	moving.PushBack({ 806, 43, 639, 412 });
	moving.speed = 18.0f;
	moving.loop = true;

	dead.PushBack({ 1539, 40, 639, 412 });
	dead.PushBack({ 1539, 40, 639, 412 });
	dead.PushBack({ 1539, 40, 639, 412 });
	dead.PushBack({ 1539, 40, 639, 412 });
	dead.speed = 18.0f;
	dead.loop = true;

	initial_pos.x = original_pos.x;
	initial_pos.y = original_pos.y;

	animation = &anim;

	collider = App->colliders->AddCollider({ x, y, (639*2)/5, (412*2)/6 }, COLLIDER_ENEMY, (j1Module*)App->entities);
	collider_head = App->colliders->AddCollider({ x, y, 200, 40}, COLLIDER_HEAD, (j1Module*)App->entities);
}

bool Enemy_Plane::Awake(pugi::xml_node& entity_config)
{
	LOG("Init Player config");
	pugi::xml_node plane = entity_config.child("plane");

	//Init Velocity var----------------------------------------------------
	speed.y = plane.child("speed").attribute("speed_y").as_float(10);
	speed.x = plane.child("speed").attribute("speed_x").as_float(0);
	idle_speed = plane.child("idle_speed").attribute("value").as_uint(0);
	path_speed.x = plane.child("path_speed").attribute("speed_x").as_float(1);
	path_speed.y = plane.child("path_speed").attribute("speed_y").as_float(1);

	scale = plane.child("scale").attribute("value").as_float(1);

	//Jump vars----------------------------------------------------
	right = plane.child("right").attribute("value").as_bool(false);
	left = plane.child("left").attribute("value").as_bool(false);
	going = plane.child("going").attribute("value").as_bool(false);
	go_x = plane.child("go_x").attribute("value").as_bool(false);
	go_y = plane.child("go_y").attribute("value").as_bool(false);
	goback= plane.child("goback").attribute("value").as_bool(false);
	alive = plane.child("alive").attribute("value").as_bool(true);
	die= plane.child("dead").attribute("value").as_bool(false);

	bool ret = true;

	return ret;
}

Enemy_Plane::~Enemy_Plane()
{
}

bool Enemy_Plane::Update(float dt)
{
	if (alive)
	{
		iPoint enemyposition = { (int)original_pos.x,(int)original_pos.y };

		position = original_pos;

		if (abs((int)App->entities->player->original_pos.x - (int)original_pos.x) <= 600 && !going)
		{
			going = true;
			go_x = true;
			go_y = true;
			goback = false;
			idle = false;

			iPoint player_position = { (int)App->entities->player->original_pos.x, (int)App->entities->player->original_pos.y + 50 };
			App->pathfinding->CreatePath(enemyposition, player_position);
			App->pathfinding->Path(App->entities->player->original_pos.x, App->entities->player->original_pos.y, Enemypath);
			Timepath = SDL_GetTicks() + 100;
		}

		if (!going && !goback && abs((int)App->entities->player->original_pos.x - (int)original_pos.x) > 600 && idle)
		{
			animation = &moving;

			if (original_pos.x < (float)initial_pos.x + 200 && right == true)
			{
				speed.x = idle_speed * dt;
				original_pos.x += speed.x;
				scale = 0.4;
				if (original_pos.x >= (float)initial_pos.x + 200)
				{
					left = true;
					right = false;
				}
			}
			else if (original_pos.x > (float)initial_pos.x - 200 && left == true)
			{
				speed.x = -idle_speed * dt;
				original_pos.x += speed.x;
				scale = -0.4;
				if (original_pos.x <= (float)initial_pos.x - 200)
				{
					left = false;
					right = true;
				}
			}
		}

		else
		{

			if (enemyposition.x != (int)App->entities->player->original_pos.x && enemyposition.y != (int)App->entities->player->original_pos.y)
			{
				iPoint PositiontoGo = App->map->MapToWorld(Enemypath[pathcounter].x, Enemypath[pathcounter].y);

				if (App->entities->player->original_pos.x < original_pos.x && going && !goback)
				{
					scale = -0.4;
				}
				if (App->entities->player->original_pos.x >= original_pos.x && going && !goback)
				{
					scale = 0.4;
				}
				if (App->entities->player->original_pos.x < initial_pos.x && !going && goback)
				{
					scale = 0.4;
				}
				if (App->entities->player->original_pos.x > initial_pos.x && !going && goback)
				{
					scale = -0.4;
				}

				if (go_x)
				{
					if (PositiontoGo.x < (int)original_pos.x)
					{
						speed.x = -path_speed.x * dt;
						original_pos.x += speed.x;
						if (PositiontoGo.x >= (int)original_pos.x)
						{
							go_x = false;
						}
					}
					else
					{
						speed.x = path_speed.x * dt;
						original_pos.x += speed.x;
						if (PositiontoGo.x <= (int)original_pos.x)
						{
							go_x = false;
						}
					}
				}

				if (go_y)
				{
					if (PositiontoGo.y < (int)original_pos.y)
					{
						speed.y = -path_speed.y * dt;
						original_pos.y += speed.y;
						if (PositiontoGo.y >= (int)original_pos.y)
						{
							go_y = false;
						}
					}
					else
					{
						speed.y = path_speed.y * dt;
						original_pos.y += speed.y;
						if (PositiontoGo.y <= (int)original_pos.y)
						{
							go_y = false;
						}
					}
				}

				if (!go_y && !go_x)
				{
					pathcounter++;
					if (pathcounter < Enemypath.Count())
					{
						go_x = true;
						go_y = true;
					}
					else
					{
						if (going)
						{
							going = false;
						}
						if (goback)
						{
							goback = false;
							idle = true;
						}
						pathcounter = 0;
					}
				}
			}
		}
		
		if (abs((int)App->entities->player->original_pos.x - (int)original_pos.x) > 600 && !goback && !idle && original_pos.x > -500)
		{
			pathcounter = 0;
			going = false;
			goback = true;
			idle = false;
			go_x = true;
			go_y = true;

			iPoint start = { (int)initial_pos.x, (int)initial_pos.y };
			App->pathfinding->CreatePath(enemyposition, start);
			App->pathfinding->Path(start.x, start.y, Enemypath);
		}
		if (original_pos.x < -500)
		{
			original_pos.x = -2000;
		}

		if (SDL_GetTicks() >= Timepath && (going))
		{
			going = false;
			goback = false;
		}
	}
	else
	{
		animation = &dead;
		if (dead.Finished() == true)
		{
			die = true;
			dead.Reset();
		}
	}

	//LOG("plane position %f", original_pos.x);

	return true;
}

bool Enemy_Plane::Load(pugi::xml_node& data)
{
	original_pos.x = data.attribute("x").as_float();
	original_pos.y= data.attribute("y").as_float();

	if (collider == nullptr)
	{
		collider = App->colliders->AddCollider({ (int)position.x, (int)position.y - 100, (639 * 2) / 5, (412 * 2) / 6 }, COLLIDER_ENEMY, (j1Module*)App->entities);
	}
	if (collider_head == nullptr)
	{
		collider_head = App->colliders->AddCollider({ (int)position.x, (int)position.y, 200, 40 }, COLLIDER_HEAD, (j1Module*)App->entities);
	}

	return true;
}

bool Enemy_Plane::Save(pugi::xml_node& data) const
{
	pugi::xml_node zombieposition = data.append_child("position");

	zombieposition.append_attribute("x") = original_pos.x;
	zombieposition.append_attribute("y") = original_pos.y;

	return true;
}
