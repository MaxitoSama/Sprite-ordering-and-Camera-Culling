#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "j1Scene.h"
#include "UIText.h"


UIText::UIText(int x, int y, UIElementType type, const char* text, uint fonttype,  int _r, int _g, int _b, j1Module* modul, bool actualize, bool _show) :UIElements(x, y, type, modul)
{
	show = _show;
	actualizable = actualize;

	string = text;
	font = fonttype;

	r = _r;
	g = _g;
	b = _b;

	texture = App->font->Print(string, { r,g,b }, App->gui->fonts[font]);

	if (font == 0 || font == 1)
	{
		BlackBackground = App->font->Print(string, { 0,0,0 }, App->gui->fonts[font]);
	}


	App->tex->GetSize(texture, collider_size_x, collider_size_y);

	scale = 2.0f;

	int rect_x = position.x - (collider_size_x / 2);
	int rect_y = position.y;

	
}

UIText::~UIText()
{
}

void UIText::Draw()
{
	if (show)
	{
		Elementrect = { (int)(position.x - App->render->camera.x), (int)(position.y - App->render->camera.y),(int)collider_size_x,(int)collider_size_y };

		if (!actualizable)
		{
			if(BlackBackground!=nullptr)
			{
				App->render->Blit(BlackBackground, position.x - App->render->camera.x + 2, position.y - App->render->camera.y + 2);
			}
			App->render->Blit(texture, position.x - App->render->camera.x , position.y - App->render->camera.y);
		}
		else
		{
			texture = App->font->Print(string, { 255,255,0 }, App->gui->fonts[font]);
			BlackBackground = App->font->Print(string, { 0,0,0 }, App->gui->fonts[font]);
			
			if (BlackBackground != nullptr)
			{
				App->render->Blit(BlackBackground, position.x - App->render->camera.x + 2, position.y - App->render->camera.y + 2);
			}
			App->render->Blit(texture, position.x - App->render->camera.x, position.y - App->render->camera.y);
			SDL_DestroyTexture(texture);
			SDL_DestroyTexture(BlackBackground);
		}		

		if (debug == true)
		{
			App->render->DrawQuad(Elementrect, 0, 255, 255, 80);
		}
	}
}