#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "p2DynArray.h"
#include "p2Point.h"

struct SDL_Texture;
class GuiImage;
class GuiText;
class GuiWindw;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
		
	p2DynArray<iPoint>	path_test;
	p2List<UIElements*> pause_buttons;

	char				score_string[256];
	char				coins[256];
	char				lives[8];
	char				time[128];

	bool				Map_1;
	bool				Map_2;


private:

	bool				exit=true;

	int					map_height;
	int					map_width;
	int					win_height ;
	int					win_width;
	int					win_position_y;
	int					limit_y;
	int					limit_x;

};

#endif // __j1SCENE_H__