#ifndef __UISLIDER_H__
#define __UISLIDER_H__

#include "UIElements.h"

class SDL_Rect;
class SDL_Texture;

class UISlider :public UIElements
{
public:

	UISlider(int x, int y, UIElementType type, SDL_Rect* slider, SDL_Rect* button, j1Module* modul, int id, bool show = false);
	~UISlider();

	void Draw();
	void Action();

public:

	SDL_Rect*		slider = nullptr;
	SDL_Rect*		button = nullptr;
	SDL_Texture*	texture = nullptr;

	int				id;
	int				slider_end = 0;
	int				slider_begining = 0;

	iPoint			mouse_position;
	iPoint			button_point;
};

#endif //__UIIMAGE_H__