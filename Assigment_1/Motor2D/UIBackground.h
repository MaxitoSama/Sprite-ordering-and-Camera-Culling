#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "UIElements.h"

class UIBackground :public UIElements
{
public:

	UIBackground(int x, int y, UIElementType type, j1Module* modul);
	~UIBackground();

	void Draw();

public:

	int		h_background;
	float	scale = 0.1f;

};

#endif //__BACKGROUND_H__