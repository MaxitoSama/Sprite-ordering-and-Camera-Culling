#ifndef __COIN_H__
#define __COIN_H__

#include "j1Entities.h"
#include "Entity.h"

class Coin: public Entity
{
public:
	
	Coin(int x, int y, ENTITY_TYPES type);
	~Coin();

	bool Update(float dt);

public:

	Collider* collider_coin=nullptr;

};

#endif //__COIN_H__

