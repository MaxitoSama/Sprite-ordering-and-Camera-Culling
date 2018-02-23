#ifndef __ModuleCollision_H__
#define __ModuleCollision_H__

#define MAX_COLLIDERS 1000

#include "j1Module.h"

enum COLLIDER_TYPE
{
	COLLIDER_NONE = -1,
	COLLIDER_WALL,
	COLLIDER_FLOOR,
	COLLIDER_PLAYER,
	COLLIDER_DEATH,
	COLLIDER_FEET,
	COLLIDER_WIN,
	COLLIDER_WIN2,
	COLLIDER_ENEMY,
	COLLIDER_HEAD,
	COLLIDER_COIN,
	COLLIDER_MAX
};

struct Collider
{
	SDL_Rect		rect;
	bool			to_delete = false;
	COLLIDER_TYPE	type;
	j1Module*		callback = nullptr;

	
	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) :rect(rectangle),type(type),	callback(callback){}

	void SetPos(int x, int y)
	{
		rect.x = x;
		rect.y = y;
	}

	bool CheckCollision(const SDL_Rect& r) const;
	bool CheckFutureFallColision(const SDL_Rect& r,float& distance,float dt, float speed);
	bool CheckFutureCrashColision(const SDL_Rect& r, float& distance, float speed);
};

class j1Colliders : public j1Module
{
public:

	j1Colliders();
	~j1Colliders();

	bool Awake();
	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	bool EraseCollider(Collider* collider);
	void DebugDraw();

	bool checkColisionList(Collider* enemCollider);
	
private:

	Collider*	colliders[MAX_COLLIDERS];
	bool		debug = false;
	float		distance = 0;
	float		Slowdt;

};

#endif // __ModuleCollision_H__