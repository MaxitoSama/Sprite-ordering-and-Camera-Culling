#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2List.h"
#include "p2Log.h"
#include "j1Module.h"
#include "p2DynArray.h"


#define CURSOR_WIDTH 2

class SDL_Texture;
class SDL_Rect;
class UIElements;
enum UIElementType;
class _TTF_Font;

enum UIEvents
{
	MOUSE_ENTER,
	MOUSE_LEAVE,
	MOUSE_CLICK,
	MOUSE_STOP_CLICK
};

class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool GUIEvent(UIEvents eventType, UIElements* element) { return true; }

	// Gui creation functions
	void AddBackground(int x, int y, UIElementType type, j1Module* modul);
	UIElements* AddElementText(int x, int y, UIElementType type, uint font, int _r, int _g, int _b, j1Module* modul, const char* text = nullptr,bool actualize=false, bool show=true);
	UIElements* AddElementTextBox(int x, int y, UIElementType type, j1Module* modul, const char* text = nullptr);
	void AddElementImage(int x, int y, UIElementType type, SDL_Rect* rect, j1Module* modul);
	UIElements* AddElementButton(int x, int y, UIElementType type, SDL_Rect* RecTex, j1Module* modul, const char* text = nullptr, bool show = true);
	UIElements* AddElementWindow(int x, int y, UIElementType type, j1Module* modul, p2List<UIElements*>* elementslist, SDL_Rect rect,bool show=true);
	UIElements* AddElementSlider(int x, int y, UIElementType type, SDL_Rect* slider, SDL_Rect* button, j1Module* modul, int id, bool show = false);
	
	UIElements* AddConsole(int x, int y, UIElementType type, SDL_Rect* slider, j1Module* modul, const char* text);

	void DeleteElements(UIElements* element);

	SDL_Texture* GetAtlas() const;
	SDL_Texture* GetBackground() const;

public:

	p2List<UIElements*> elements;

	SDL_Texture* background = nullptr;
	SDL_Texture* window = nullptr;
	SDL_Texture* textbox = nullptr;
	SDL_Texture* console = nullptr;

	p2DynArray<_TTF_Font*> fonts;
	p2DynArray<SDL_Texture*> buttons;

	bool startgame = false;

private:

	SDL_Texture* atlas = nullptr;
	
	p2SString atlas_file_name;
	
	p2List_item<UIElements*>* tab_element=nullptr;
	p2List_item<UIElements*>* last_tab_element = nullptr;

	uint tab_num=-1;
	uint last_tab = -1;

	bool tabing = false;
};

#endif // __j1GUI_H__