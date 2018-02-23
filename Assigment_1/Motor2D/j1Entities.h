#ifndef __J1ENEMIES_H__
#define __J1ENEMIES_H__

#include "j1Module.h"

#define MAX_ENEMIES 100
#define SCREEN_SIZE 1
#define SCREEN_WIDTH 224
#define SCREEN_HEIGHT 320

class Entity;
class Player;
struct SDL_Texture;

enum ENTITY_TYPES
{
	NO_TYPE,
	PLAYER,
	ZOMBIE,
	PLANE,
	COIN
};

struct EnemyInfo
{
	ENTITY_TYPES type = ENTITY_TYPES::NO_TYPE;
	int x, y;
};

class j1Entities : public j1Module
{
public:

	j1Entities();
	~j1Entities();

	bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	bool AddEnemy(ENTITY_TYPES type, int x, int y, int wave = 1, int id = 0);

private:

	void SpawnEnemy(const EnemyInfo& info);

public:

	Entity*				entities[MAX_ENEMIES];
	Player*				player = nullptr;
	
	pugi::xml_node		entity_config;
	pugi::xml_document	config_file;
	
	SDL_Texture*		sprites_zombie = nullptr;
	SDL_Texture*		sprites_plane = nullptr;
	SDL_Texture*		sprites_player = nullptr;
	SDL_Texture*		sprites_coin = nullptr;

	bool				draw_underlayed = false;
	bool				Slowmo = false;

	uint				SlowAnimationCap = 1;

	float				Slowmo_dt = 0;

private:
	
	EnemyInfo			queue[MAX_ENEMIES];

};

#endif // __J1ENEMIES_H__