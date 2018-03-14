#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "UIElements.h"

class UIWindow : public UIElements
{
public:

	UIWindow(int x, int y, UIElementType type, j1Module* modul, p2List<UIElements*>* elementslist, SDL_Rect rect,bool _show);
	~UIWindow();

	void Draw();
	void Action();

private:

	p2List<UIElements*>*	windowelements=NULL;
	SDL_Rect				window_rect;

	iPoint					mousestart;
	iPoint					mouseend = { 0,0 };
};

#endif //__WINDOW_H__
