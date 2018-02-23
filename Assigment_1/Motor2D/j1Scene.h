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

	//Change to scene 1
	void ChangeScene(int x, int y);

	void StartCurrentScene();
	bool GUIEvent(UIEvents eventType, UIElements* element);
	void GoToMenu();

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
	bool				LoadUI = true;

	GuiText*			text = nullptr;
	j1Timer				timer;

	UIElements*			QuitButton = nullptr;
	UIElements*			Score = nullptr;
	UIElements*			Lives = nullptr;
	UIElements*			Coins = nullptr;
	UIElements*			button_back = nullptr;
	UIElements*			pause_window = nullptr;
	UIElements*			back_menu_button = nullptr;
	UIElements*			back_menu_text = nullptr;
	UIElements*			exit_button = nullptr;
	UIElements*			exit_text = nullptr;
	UIElements*			exit_pause = nullptr;
	UIElements*			clockframe = nullptr;
	UIElements*			Clock = nullptr;


	SDL_Rect			button_play;
	SDL_Rect			display_score;
	SDL_Rect			live_icon;
	SDL_Rect			display_coins;
	SDL_Rect			rect_button_back;
	SDL_Rect			rect_button_exit;
	SDL_Rect			rect_clock;
	SDL_Rect			rect_exit_pause;

	int					map_height;
	int					map_width;
	int					win_height ;
	int					win_width;
	int					win_position_y;
	int					limit_y;
	int					limit_x;

};

#endif // __j1SCENE_H__