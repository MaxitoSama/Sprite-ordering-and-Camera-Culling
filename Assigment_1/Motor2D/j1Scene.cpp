#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Colliders.h"
#include "j1Pathfinding.h"
#include "j1Menu.h"
#include "j1Scene.h"
#include "j1Transition.h"
#include "j1Entities.h"
#include "Player.h"
#include "j1Gui.h"
#include "UIButton.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;
	Map_1 = true;
	Map_2 = false;

	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	//Camera Movement--------------------------------------
	
	win_height = App->win->screen_surface->h;
	win_width = App->win->screen_surface->w;
	
	if (!App->menu->active)
	{
		timer.Start();
		if (Map_1)
		{
			App->map->Load("Map_1.tmx");
			App->audio->PlayMusic("audio/music/map1_music.ogg");
			App->map->Draw_Colliders();
			App->map->LoadEntities();
			App->colliders->AddCollider({ 25400,0,50,380 }, COLLIDER_WIN, this);

			map_height = App->map->data.height * App->map->data.tile_height;
			map_width = App->map->data.width * App->map->data.tile_width;
			limit_y = map_height - win_height;
			limit_x = map_width - win_width / 2;

			//enemies
			App->entities->AddEnemy(ZOMBIE, 1300, 600);
			App->entities->AddEnemy(PLANE, 2000, 250);
			App->entities->AddEnemy(ZOMBIE, 10900, 600);
			App->entities->AddEnemy(ZOMBIE, 9812, 668);
			App->entities->AddEnemy(ZOMBIE, 21754, 827);
		
			App->colliders->AddCollider({ 11104,636,608,64 }, COLLIDER_DEATH);
			App->colliders->AddCollider({ 22112,829,288,32 }, COLLIDER_DEATH);
		}

		if (Map_2)
		{
			App->map->Load("Map_2.tmx");
			App->audio->PlayMusic("audio/music/map1_music.ogg");
			App->map->Draw_Colliders();
			App->map->LoadEntities();
			
			map_height = App->map->data.height * App->map->data.tile_height;
			map_width = App->map->data.width * App->map->data.tile_width;
			limit_y = map_height - win_height;
			limit_x = map_width - win_width / 2;

			App->colliders->AddCollider({ 25400,0,50,310 }, COLLIDER_WIN2, this);
			App->colliders->AddCollider({ 21087,828,320,32 }, COLLIDER_DEATH);

			App->entities->AddEnemy(ZOMBIE, 5500, 828);
			App->entities->AddEnemy(ZOMBIE, 19350, 825);
			App->entities->AddEnemy(ZOMBIE, 19461, 825);

			App->entities->AddEnemy(PLANE, 500, 50);
		}

		if (LoadUI)
		{

			display_score = { 173, 3149, 397, 133 };
			display_coins = { 187,2976,383,132 };
			live_icon = { 683,2985,83,79 };
			rect_clock = { 853,2984,215,76 };

			rect_button_exit = { 2149, 1409, 178, 182 };
			rect_button_back = { 3094,101,179,182 };
			rect_exit_pause = { 3093,537,178,181 };

			LoadUI = false;
		
			App->gui->AddElementImage(0.008*App->win->screen_surface->w, 0.05*App->win->screen_surface->h, IMAGE, &display_coins, this);
			Coins = App->gui->AddElementText(0.10*App->win->screen_surface->w, 0.075*App->win->screen_surface->h, TEXT, 1, 255, 255, 0, this, coins, true, true);

			App->gui->AddElementImage(0.003*App->win->screen_surface->w, 0.15*App->win->screen_surface->h, IMAGE, &display_score, this);
			Score = App->gui->AddElementText(0.10*App->win->screen_surface->w, 0.175*App->win->screen_surface->h, TEXT, 1, 255, 255, 0, this, score_string, true, true);

			App->gui->AddElementImage(0.77*App->win->screen_surface->w, 0.05*App->win->screen_surface->h, IMAGE, &live_icon, this);
			Lives = App->gui->AddElementText(0.87*App->win->screen_surface->w, 0.07*App->win->screen_surface->h, TEXT, 1, 0, 0, 0, this, lives, true, true);

			App->gui->AddElementImage(App->win->screen_surface->w/2-(rect_clock.w/2*1.6), 0.04*App->win->screen_surface->h, IMAGE, &rect_clock, this);
			Clock = App->gui->AddElementText(App->win->screen_surface->w *0.48 , 0.065*App->win->screen_surface->h, TEXT, 3, 0, 0, 0, this, time, true, true);
			
			exit_button = App->gui->AddElementButton(0.1*win_width,0.38*win_height, BUTTON, &rect_button_exit, this, nullptr, false);
			exit_text = App->gui->AddElementText(0.17*win_width, 0.42*win_height, TEXT, 1, 255, 255, 0, this, "Exit", false);
			

			back_menu_button = App->gui->AddElementButton(0.1*win_width, 0.23*win_height, BUTTON, &rect_button_back, this, nullptr, false);
			back_menu_text= App->gui->AddElementText(0.17*win_width, 0.27*win_height, TEXT, 1, 255, 255, 0, this, "Menu", false);

	
			exit_pause = App->gui->AddElementButton(0.43*win_width, 0.4*win_height, BUTTON, &rect_exit_pause, this, nullptr, false);

			pause_buttons.add(exit_button);
			pause_buttons.add(back_menu_button);
			pause_buttons.add(back_menu_text);
			pause_buttons.add(exit_text);
			pause_buttons.add(exit_pause);

			pause_window = App->gui->AddElementWindow(0.25*win_width, 0.2*win_height, WINDOWS, this, &pause_buttons, { 1086,250,768,623 }, false);
		}
		App->GamePaused = false;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate_Scene1 ", Profiler::Color::Orchid)
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Update_Scene1 ", Profiler::Color::MediumOrchid)

 	win_position_y = App->render->camera.y*-1;

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		App->SaveGame();
	}

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		if (Map_1)
		{
			StartCurrentScene();
		}
		else
		{
			ChangeScene(60, 215);
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		StartCurrentScene();

	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		ChangeScene(60,215);

	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (App->Cap_on)
		{
			App->Cap_on = false;
		}
		else
		{
			App->Cap_on = true;
		}
	}	

	if (App->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		if (App->GamePaused)
		{
			pause_window->show = false;
			App->GamePaused = false;
		}
		else
		{
			pause_window->show = true;
			App->GamePaused = true;
		}
	}


	if (App->entities->player->original_pos.x > win_width / 2 && App->entities->player->original_pos.x <= limit_x)
	{
		App->render->camera.x = App->entities->player->win_width / 2 - App->entities->player->original_pos.x;
	}
	
	if (!App->win->fullscreen_window && App->entities->player->original_pos.y > win_height / 2 && win_position_y+win_height <map_height)
	{
		if (App->entities->player->original_pos.y + win_height / 2 < map_height)
		{
			App->render->camera.y = -App->entities->player->original_pos.y + win_height / 2;
		}
	}

	App->map->Draw();

	sprintf_s(score_string,256,"%d",App->entities->player->score);
	sprintf_s(coins, 256, "%d", App->entities->player->coins);
	sprintf_s(lives, 256, "x0%d", App->entities->player->lives);
	sprintf_s(time, 128, "%i", (int)timer.ReadSec());

	if (App->entities->player->lives < 1 || timer.ReadSec()>=100)
	{
		App->gui->startgame = true;
		App->GamePaused = true;
		App->transit->Transition(this, App->menu);
		App->entities->player->lives = 3;
		App->entities->player->score = 0;
		App->entities->player->coins=0;
	}
	
	if (App->entities->player->coins == 20)
	{
		App->entities->player->lives++;
		App->entities->player->coins = 0;
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("PosUpdate_Scene1 ", Profiler::Color::DarkOrchid)
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		exit = false;

	return exit;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	App->entities->CleanUp();
	App->colliders->CleanUp();
	App->map->CleanUp();
	App->pathfinding->CleanUp();

	return true;
}

//go to the begining
void j1Scene::StartCurrentScene()
{
	App->entities->player->original_pos.x = 60;
	App->entities->player->original_pos.y = 215;
	App->render->camera.x = 0;
	App->render->camera.y = 0;
}

//Change to scene 2
void j1Scene::ChangeScene(int x, int y)
{
	CleanUp();
	
	if (Map_1 && App->entities->player->Curr_map==1)
	{
		Map_1 = false;
		Map_2 = true;

		App->scene->Start();
		App->pathfinding->Start();
		App->entities->player->original_pos.y = y;
		App->entities->player->original_pos.x = x;
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->entities->player->Curr_map = 2;
		App->entities->Start();
	}
	else
	{
		Map_1 = true;
		Map_2 = false;

		App->scene->Start();
		App->pathfinding->Start();
		App->entities->player->original_pos.y = y;
		App->entities->player->original_pos.x = x;
		App->render->camera.x = 0;
		App->render->camera.y = 0;
		App->entities->player->Curr_map = 1;
		App->entities->Start();
	}
}

bool j1Scene::GUIEvent(UIEvents eventType, UIElements* element)
{
	switch (eventType)
	{
	case MOUSE_ENTER:
		if (element->type == BUTTON && element->show)
		{
			element->Action();
		}
		break;
	case MOUSE_LEAVE:
		if (element->type == BUTTON && element->show)
		{
			element->Action();
			element->action = false;
		}
		break;

	case MOUSE_CLICK:
		if (element->type == BUTTON && element->show)
		{
			element->action = true;
		}
		break;

	case MOUSE_STOP_CLICK:
		if (element->type == BUTTON && element->show == true)
		{
			App->audio->PlayFx(3);
			element->action = false;
		}
		if (element == exit_button && element->show)
		{
			exit = false;
		}
		if (element == back_menu_button && element->show)
		{
			App->gui->startgame = true;
			App->transit->Transition(this, App->menu);
		}

		if (element == exit_pause && element->show)
		{
			pause_window->show = false;
			App->GamePaused = false;
		}
		break;

	default:
		break;
	}
	return true;
}
void j1Scene::GoToMenu()
{
	LoadUI = true;
	pause_buttons.clear();

	App->scene->CleanUp();
	App->menu->Start();
}
