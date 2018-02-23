#ifndef __TRANSITION_H__
#define	__TRANSITION_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1Transition:public j1Module
{
public:
	j1Transition();
	~j1Transition();

	bool Start();
	bool PostUpdate();
	bool Transition(j1Module* module_off, j1Module* module_on);

private:
	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	j1Timer clock;

	Uint32 start_time = 0;
	Uint32 total_time = 0;

	SDL_Rect screen;

	uint transp;

	bool free_gui=false;

	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;
};

#endif // __TRANSITION_H__