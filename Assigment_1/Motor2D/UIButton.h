#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "UIElements.h"

class SDL_Rect;
class SDL_Texture;

class UIButton : public UIElements
{
public:

	UIButton(int x, int y, UIElementType type, SDL_Rect* RecTex, const char* text, j1Module* modul,bool show=true);
	~UIButton();

	void Draw();
	void Action();

private:

	const char*		buttontext = nullptr;

	SDL_Texture*	ButtonTex=nullptr;
	SDL_Texture*	ButtonText = nullptr;

	uint sizeTx;
	uint sizeTy;

	SDL_Rect*		RectTexture = nullptr;

	bool			light = false;
};

#endif //__BUTTONS_H__

