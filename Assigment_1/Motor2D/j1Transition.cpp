#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Entities.h"
#include "j1Menu.h"
#include "j1Scene.h"
#include "j1Gui.h"
#include "j1Transition.h"
#include "j1Window.h"
#include "j1App.h"




j1Transition::j1Transition()
{
	name.create("transition");
	
}

j1Transition::~j1Transition()
{
}

bool j1Transition::Start()
{
	LOG("Preparing Fade Screen");

	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	screen = { 0, 0,App->win->screen_surface->w, App->win->screen_surface->h };
	return true;
}

bool j1Transition::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate Transition", Profiler::Color::WhiteSmoke);
	bool ret=true;

	if (current_step == fade_step::none)
	{
		ret == true;
	}
	else
	{
		switch (current_step)
		{
		case fade_step::fade_to_black:
		{
			if (transp <= 249)
			{
				transp += 6;

			}

			if (clock.ReadSec() >= 1.5 && free_gui)
			{
				free_gui = false;
				App->gui->startgame = true;
			}
			if (clock.ReadSec() >= 2)
			{
				to_disable->active = false;

				App->render->camera.x = 0;
				App->render->camera.y = 0;

				if (to_enable == App->scene)
				{
					App->menu->StartGame();
				}
				else
				{
					App->scene->GoToMenu();
				}

				to_enable->active = true;
				current_step = fade_step::fade_from_black;
			}
		} 
		break;

		case fade_step::fade_from_black:
		{
			if (transp >= 6)
			{
				transp -= 6;
			}
			else 
			{
				current_step = fade_step::none;
			}
		}
		break;
		}

		SDL_SetRenderDrawColor(App->render->renderer, 255, 255, 225, transp);
		SDL_RenderFillRect(App->render->renderer, &screen);
	}

	return ret;
}

bool j1Transition::Transition(j1Module* module_off, j1Module* module_on)
{
	bool ret=false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		transp = 0;
		free_gui = true;
		to_disable = module_off;
		to_enable = module_on;
		clock.Start();
		ret = true;
	}
	
	return ret;
}