#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

#include "UIElements.h"

class SDL_Texture;

class UITextBox : public UIElements
{
public:

	UITextBox(int x, int y, UIElementType type, const char* text, j1Module* modul);
	~UITextBox();

	void Draw();
	void Action();

private:

	const char*		HidenText = nullptr;
	SDL_Texture*	hidentexture = nullptr;

	bool			hide = false;
	uint			sizeTx;
	uint			sizeTy;

};

#endif //__TEXTBOX_H__