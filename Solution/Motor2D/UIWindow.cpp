#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "UIWindow.h"


UIWindow::UIWindow(int x, int y, UIElementType type, j1Module* modul, p2List<UIElements*>* elementslist, SDL_Rect rect,bool _show) :UIElements(x, y, type, modul)
{
	window_rect = rect;
	show = _show;

	collider_size_x = window_rect.w;
	collider_size_y = window_rect.h;

	windowelements = elementslist;

	p2List_item<UIElements*>* item = windowelements->start;

	while (item != nullptr)
	{
		item->data->position.x = item->data->position.x + position.x;
		item->data->position.y = item->data->position.y + position.y;

		item = item->next;
	}

	scale = 1.0f;
}


UIWindow::~UIWindow()
{
}

void UIWindow::Draw()
{
	if (show)
	{
		int rect_x = position.x - App->render->camera.x;
		int rect_y = position.y - App->render->camera.y;

		Elementrect = { rect_x,rect_y,(int)collider_size_x*(int)scale,(int)collider_size_y*(int)scale };

		App->render->Blit(App->gui->window, position.x - App->render->camera.x, position.y - App->render->camera.y,&window_rect, 1.0f);

		if (debug)
		{
			App->render->DrawQuad(Elementrect, 100, 200, 50, 80, true);
		}

		p2List_item<UIElements*>* item = windowelements->start;

		while (item != nullptr)
		{
			item->data->show = true;
			item->data->Draw();

			item = item->next;
		}

		if (action)
		{
			Action();
		}

		App->input->GetMousePosition(mouseend.x, mouseend.y);
	}
	else
	{
		p2List_item<UIElements*>* item = windowelements->start;
		while (item != nullptr)
		{
			item->data->show = false;
			item = item->next;
		}
	}
}

void UIWindow::Action()
{

	App->input->GetMousePosition(mousestart.x, mousestart.y);

	if (mousestart.x != mouseend.x || mousestart.y != mouseend.y)
	{
		position.x += mousestart.x - mouseend.x;
		position.y += mousestart.y - mouseend.y;

		p2List_item<UIElements*>* item = windowelements->start;

		while (item != nullptr)
		{
			item->data->position.x += mousestart.x - mouseend.x;
			item->data->position.y += mousestart.y - mouseend.y;

			item = item->next;
		}
	}

}