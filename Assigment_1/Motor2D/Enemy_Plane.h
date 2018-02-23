#ifndef __PLANE_H__
#define __PLANE_H__

#include "Entity.h"


class Enemy_Plane : public Entity
{
public:

	Enemy_Plane(int x, int y, ENTITY_TYPES type);
	~Enemy_Plane();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	Animation anim;
	Animation dead;
	Animation moving;

private:

	float	idle_speed = 0;
	fPoint	path_speed = { 0,0 };

	bool	goback = false;
	bool	idle = true;
	bool	go_x = false;
	bool	go_y = false;

};

#endif // !__BALLOON_H__
