#include "Coin.h"
#include "j1App.h"
#include "j1Audio.h"
#include "j1Textures.h"
#include "j1Colliders.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1Entities.h"
#include "Player.h"
#include "j1Scene.h"



Coin::Coin(int x, int y, ENTITY_TYPES type) : Entity(x, y, type)
{
	uint tex_w, tex_h;

	App->tex->GetSize(App->entities->sprites_coin, tex_w, tex_h);
	
	scale = 0.5f;

	collider = App->colliders->AddCollider({x,y,(int)(tex_w*scale),(int)(tex_h*scale)},COLLIDER_COIN, (j1Module*)App->entities);
}

Coin::~Coin()
{
}

bool Coin::Update(float dt)
{
	if (!alive)
	{
		App->audio->PlayFx(2);
		die = true;
	}
	return true;
}