#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Colliders.h"
#include "j1Entities.h"
#include "j1Textures.h"
#include "j1Menu.h"
#include "j1Scene.h"
#include "Entity.h"

//Include all enemies
#include "Enemy_Zombie.h"
#include "Enemy_Plane.h"
#include "Player.h"
#include "Coin.h"

#define SPAWN_MARGIN 2000

j1Entities::j1Entities()
{
	name.create("entity");
	
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		entities[i] = nullptr;
	}
}

// Destructor
j1Entities::~j1Entities()
{
}

bool j1Entities::Awake(pugi::xml_node& config)
{
	config_file.load_file("config.xml");
	entity_config = config_file.child("config").child("entity");

	return true;
}

bool j1Entities::Start()
{
	LOG("loading enemies");

	// Create a prototype for each enemy available so we can copy them around
	sprites_zombie = App->tex->Load("assets/enemies/zombie/zombie.png");
	sprites_plane = App->tex->Load("assets/enemies/plane/plane.png");
	sprites_player = App->tex->Load("assets/character/character.png");
	sprites_coin = App->tex->Load("assets/entities/coin.png");

	if (!App->menu->active)
	{
		if (player == nullptr)
		{
			player = new Player(10, 100, PLAYER);
			player->Awake(entity_config);
			player->Start();
		}
		else
		{
			player->Start();
		}

	}
	

	return true;
}

bool j1Entities::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate Entities", Profiler::Color::Orange);
	
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPES::NO_TYPE)
		{
			if (-queue[i].y < App->render->camera.y + SPAWN_MARGIN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENTITY_TYPES::NO_TYPE;
				LOG("Spawning enemy at %d", queue[i].y * SCREEN_SIZE);
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Entities::Update(float dt)
{
	BROFILER_CATEGORY("Update Entities", Profiler::Color::OrangeRed);

	if (App->scene->active)
	{
		if (Slowmo)
		{
			Slowmo_dt = dt / 4;
		}
		else
		{
			Slowmo_dt = dt;
		}

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (entities[i] != nullptr)
			{
				entities[i]->Update(Slowmo_dt);
			}
		}

		if (player != nullptr)
		{
			player->Update(dt);
			player->Draw(sprites_player, player->scale, dt);
		}

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (entities[i] != nullptr && (entities[i]->collider != nullptr))
			{
				if (entities[i]->GetType() == ZOMBIE)
				{
					entities[i]->Draw(sprites_zombie, entities[i]->scale, Slowmo_dt);
				}
				if (entities[i]->GetType() == PLANE)
				{
					entities[i]->Draw(sprites_plane, entities[i]->scale, Slowmo_dt);
				}
				if (entities[i]->GetType() == COIN)
				{
					entities[i]->Draw(sprites_coin, entities[i]->scale, Slowmo_dt);
				}
			}
		}

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (entities[i] != nullptr && entities[i]->die)
			{
				entities[i]->position.x = -2000;
				entities[i]->position.y = 0;
				entities[i]->original_pos.x = -2000;
				entities[i]->original_pos.y = 0;
				entities[i]->die = false;
				entities[i]->alive = true;
			}
		}

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (entities[i] != nullptr && (entities[i]->collider == nullptr))
			{
				if (entities[i]->GetType() == ZOMBIE)
				{
					entities[i]->Draw(sprites_zombie, entities[i]->scale, Slowmo_dt);
				}
				if (entities[i]->GetType() == PLANE)
				{
					entities[i]->Draw(sprites_plane, entities[i]->scale, Slowmo_dt);
				}
			}
		}
	}
	return true;
}

bool j1Entities::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate Entities", Profiler::Color::DarkOrange);
	
	return true;
}

bool j1Entities::CleanUp()
{
	LOG("Freeing all enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (entities[i] != nullptr)
		{
			delete entities[i];
			entities[i] = nullptr;
		}
		if (queue[i].type != NO_TYPE)
		{
			queue[i].type = NO_TYPE;
		}
	}
	
	App->tex->UnLoad(sprites_coin);
	App->tex->UnLoad(sprites_plane);
	App->tex->UnLoad(sprites_player);
	App->tex->UnLoad(sprites_zombie);

	return true;
}

bool j1Entities::AddEnemy(ENTITY_TYPES type, int x, int y, int wave, int id)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Entities::SpawnEnemy(const EnemyInfo& info)
{

	uint i = 0;
	for (; entities[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENTITY_TYPES::ZOMBIE:
			entities[i] = new Enemy_Zombie(info.x, info.y,info.type);
			entities[i]->Awake(entity_config);
			break;
		case ENTITY_TYPES::PLANE:
			entities[i] = new Enemy_Plane(info.x, info.y,info.type);
			entities[i]->Awake(entity_config);
			break;
		case ENTITY_TYPES::PLAYER:
			entities[i] = new Player(info.x, info.y, info.type);
			break;
		case ENTITY_TYPES::COIN:
			entities[i] = new Coin(info.x, info.y, info.type);
			break;
		}
	}
}

bool j1Entities::Load(pugi::xml_node& data)
{
	pugi::xml_node zombies = data.child("enemy_zombie").first_child();
	pugi::xml_node planes=data.child("enemy_plane").first_child();

	if (player != nullptr)
	{
		player->Load(data);
	}

	for (uint i = 0; i < MAX_ENEMIES; i++)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == ZOMBIE)
			{
				entities[i]->Load(zombies);
				zombies=zombies.next_sibling();
			}
			if (entities[i]->type == PLANE)
			{
				entities[i]->Load(planes);
				planes = planes.next_sibling();
			}
		}
	}
	
	return true;
}

bool j1Entities::Save(pugi::xml_node& data) const
{
	if (player != nullptr)
	{
		player->Save(data);
	}

	pugi::xml_node zombies = data.append_child("enemy_zombie");
	pugi::xml_node planes = data.append_child("enemy_plane");

	for (uint i = 0; i < MAX_ENEMIES; i++)
	{
		if (entities[i] != nullptr)
		{
			if (entities[i]->type == ZOMBIE)
			{
				entities[i]->Save(zombies);
			}
			if (entities[i]->type == PLANE)
			{
				entities[i]->Save(planes);
			}
		}
	}

	return true;
}