#include "j1App.h"
#include "j1Render.h"
#include "j1Gui.h"
#include "UIBackground.h"
#include "j1Window.h"


UIBackground::UIBackground(int x, int y, UIElementType type, j1Module* modul) :UIElements(x, y, type, modul)
{
}

UIBackground::~UIBackground()
{
}

void UIBackground::Draw()
{
	//Adapts the background to the screen
	h_background = 907;//size of the background
	while (h_background*scale <= App->win->screen_surface->h)
	{
		scale += 0.01f;
	}

	//prints background
	App->render->Blit(App->gui->GetBackground(), position.x, position.y, {},scale);
}