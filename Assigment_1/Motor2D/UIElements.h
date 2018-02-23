#ifndef __UIELEMENTS_H__
#define __UIELEMENTS_H__

#include "j1Gui.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

enum UIElementType
{
	BACKGROUND,
	TEXT,
	IMAGE,
	BUTTON,
	TEXTBOX,
	WINDOWS,
	SLIDER,
	CONSOLE
};

class UIElements :public j1Gui
{
public:

	UIElements(int x, int y, UIElementType type, j1Module* modul);
	~UIElements();

	virtual void Draw() {};
	virtual void Action() {};
	virtual void InsertComand() {};
	virtual void exec_command(const char* command) {};

	bool CheckMouse()const;

public:

	SDL_Rect		Elementrect;
	fPoint			position;
	UIElementType	type;

	j1Module*		callback = nullptr;

	uint			collider_size_x;
	uint			collider_size_y;

	bool			mousein = false;
	bool			mouseout = true;
	bool			action = false;
	bool			debug = false;
	bool			show = false;
	bool			isWindowElement = false;
	bool			tab = false;
	bool			tabed = false;

	SDL_Texture*	ElementTexture = nullptr;

	float			scale=0;

};

#endif// __UIELEMENTS_H__