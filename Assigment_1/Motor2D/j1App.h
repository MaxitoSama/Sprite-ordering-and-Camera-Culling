#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "p2DynArray.h"
#include "j1Module.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2Defs.h"
#include "j1PerfTimer.h"
#include "j1Timer.h"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Menu;
class j1Scene;
class j1Transition;
class j1Colliders;
class j1Map;
class j1Entities;
class j1PathFinding;
class j1Fonts;
class j1Gui;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load 
	bool LoadGameNow();
	//Save
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win=nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Menu*				menu = nullptr;
	j1Scene*			scene = nullptr;
	j1Transition*		transit = nullptr;
	j1Colliders*		colliders = nullptr;
	j1Map*				map = nullptr;
	j1Entities*			entities = nullptr;
	j1PathFinding*		pathfinding = nullptr;
	j1Fonts*			font = nullptr;
	j1Gui*				gui = nullptr;


	float				dt;

	bool				Cap_on = false;
	bool				GamePaused = false;

	p2SString			load_game;
	mutable p2SString	save_game;
	

private:

	p2List<j1Module*>	modules;
	uint32				frames;
	
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;

	mutable bool		want_to_save;
	bool				want_to_load;

	j1PerfTimer			ptimer;
	uint32				frame_count=0;
	j1Timer				startup_time;
	j1Timer				frame_time;
	j1Timer				last_sec_frame_time;
	uint32				last_sec_frame_count = 0;
	uint32				prev_last_sec_frame_count = 0;
	uint16_t			framerate_cap;
	int					capped_ms = -1;
};

extern j1App* App; 

#endif