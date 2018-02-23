#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "UIElements.h"
#include "UIImage.h"
#include "p2Animation.h"

class Console :public UIElements
{
public:

	Console(int x, int y, UIElementType type, SDL_Rect* rect, j1Module* modul, const char* text);
	~Console();

	void Draw();
	void InsertComand();
	void Draw_commands();
	void exec_command(const char* command);

	iPoint CalcPosBar();

private:

	SDL_Rect*				rect;

	SDL_Texture*			comand;

	Animation				bar;

	iPoint					Bpos;

	uint					com_length;

	const char*				text;
	
	p2List<SDL_Texture*>	last_comands;

	

};

#endif // !1
