#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "Entity.h"
#include "p2Path.h"

class Enemy_Zombie : public Entity
{
public:

	Enemy_Zombie(int x, int y, ENTITY_TYPES type);
	~Enemy_Zombie();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	Animation anim;
	Animation dead;
	Animation walking;

private:

	bool	goback = false;
	bool	go_x = false;
	float	idle_speed = 0;
	float	path_speed = 0;

};

#endif
