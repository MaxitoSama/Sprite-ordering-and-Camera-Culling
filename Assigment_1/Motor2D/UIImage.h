#ifndef __UIIMAGE_H__
#define __UIIMAGE_H__

#include "UIElements.h"

class SDL_Rect;
class SDL_Texture;

class UIImage :public UIElements
{
public:

	UIImage(int x, int y, UIElementType type, SDL_Rect* rect, j1Module* modul);
	~UIImage();

	void Draw();

public:

	SDL_Rect*		rectangle = nullptr;
	SDL_Texture*	texture = nullptr;

};

#endif //__UIIMAGE_H__