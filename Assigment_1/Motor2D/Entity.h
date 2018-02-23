#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "j1Entities.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "p2animation.h"
#include "p2Log.h"
#include "SDL/include/SDL_timer.h"
#include "SDL_mixer\include\SDL_mixer.h"
#include "p2Path.h"

#pragma comment( lib, "SDL_mixer/libx86/SDL2_mixer.lib" )

struct SDL_Texture;
struct Collider;

class Entity:public j1Entities
{
public:

	Entity(int x, int y,ENTITY_TYPES type);
	virtual ~Entity();

	const Collider*		GetCollider() const;
	const Collider*		GetColliderFloor() const;
	const Collider*		GetColliderHead() const;
	const ENTITY_TYPES	GetType()const;

	bool				Awake(pugi::xml_node&);
	void				Draw(SDL_Texture* sprites, float direction,float dt);

	virtual bool		Update(float dt)=0;

public:
	
	ENTITY_TYPES	type;

	fPoint			position = { 0.0f,0.0f };
	fPoint			original_pos = { 0.0f,0.0f };
	fPoint			speed = { 0.0f,0.0f }; 
	fPoint			initial_pos = { 0.0f,0.0f };

	float			scale = 0;
	
	bool			left = false;
	bool			right = true;
	bool			alive = true;
	bool			die = false;
	bool			going = false;

	uint			Timepath = 0;
	uint			pathcounter = 0;

	Collider*		collider = nullptr;
	Collider*		collider_feet = nullptr;
	Collider*		collider_head = nullptr;

	p2DynArray<iPoint>	Enemypath = NULL;

protected:

	Animation*		animation = nullptr;
};

#endif // __ENEMY_H__