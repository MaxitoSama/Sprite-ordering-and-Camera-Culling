#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "UITextBox.h"

UITextBox::UITextBox(int x, int y, UIElementType type, const char* text, j1Module* modul) : UIElements(x, y, type, modul)
{

	if (text != nullptr)
	{
		HidenText = text;
		hidentexture = App->font->Print(text, { 70,70,70 }, App->gui->fonts[1]);
	}

	App->tex->GetSize(App->gui->textbox, collider_size_x, collider_size_y);
	App->tex->GetSize(hidentexture, sizeTx, sizeTy);

	scale = 2.0f;

	int rect_x = position.x - (collider_size_x / 2)*scale;
	int rect_y = position.y;

	Elementrect = { rect_x,rect_y,(int)collider_size_x*(int)scale,(int)collider_size_y*(int)scale };

}

UITextBox::~UITextBox()
{
}

void UITextBox::Draw()
{
	App->render->Blit(App->gui->textbox, position.x - App->render->camera.x - collider_size_x / 2 * scale, position.y - App->render->camera.y,NULL, scale);

	if (debug == true)
	{
		App->render->DrawQuad(Elementrect, 255, 255, 0, 80);
	}

	if (hide == false)
	{
		App->render->Blit(hidentexture, position.x - App->render->camera.x - sizeTx / 1.5f, position.y - App->render->camera.y + sizeTy / 2,NULL, 1.5f);
	}
}

void UITextBox::Action()
{
	bool inside = false;
	int x, y;

	App->input->GetMousePosition(x, y);

	if (x>Elementrect.x && x<Elementrect.x + Elementrect.w)
	{
		if (y > Elementrect.y && y <Elementrect.y + Elementrect.h)
		{
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				hide = true;
			}
		}
	}
	if (x<Elementrect.x || x>Elementrect.x + Elementrect.w || y < Elementrect.y || y > Elementrect.y + Elementrect.h)
	{
		if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			hide = false;
		}
	}
}


