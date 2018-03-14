#ifndef __HOUSE__H_
#define __HOUSE__H_

#include "j1Entities.h"
#include "Entity.h"

class House:public Entity
{
public:
	House(int x, int y, ENTITY_TYPES type);
	~House();

	bool Awake(pugi::xml_node&);
	bool Update(float dt);

private:

	uint image_size_h;
	uint image_size_w;

};

#endif // !__HOUSE__H_
