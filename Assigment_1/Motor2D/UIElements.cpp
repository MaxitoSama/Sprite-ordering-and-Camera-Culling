#include "j1App.h"
#include "UIElements.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "j1Render.h"
#include "j1Fonts.h"



UIElements::UIElements(int x, int y, UIElementType type, j1Module* modul) :position(x, y), type(type), callback(modul)
{
}

UIElements::~UIElements()
{
}

bool UIElements::CheckMouse()const
{
	bool ret = false;
	int x, y;

	App->input->GetMousePosition(x, y);

	x = x - App->render->camera.x;
	y = y - App->render->camera.y;

	if (x>Elementrect.x && x<Elementrect.x + Elementrect.w)
	{
		if (y > Elementrect.y && y <Elementrect.y + Elementrect.h)
		{
			ret = true;
		}
	}

	return ret;
}