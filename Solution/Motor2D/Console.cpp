#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Gui.h"
#include "UIBackground.h"
#include "j1Window.h"
#include "Console.h"


Console::Console(int x, int y, UIElementType type, SDL_Rect* _rect, j1Module* modul, const char* _text) :UIElements(x, y, type, modul)
{
	rect = _rect;

	bar.PushBack({ 3000,0,2,20 });
	bar.PushBack({ 3000,0,0,0 });
	bar.loop = true;
	bar.speed =5.0f;

	Bpos.x = x+3;
	Bpos.y = y+480;

	text = _text;
	comand = App->font->Print(text, { 255,255,255 }, App->gui->fonts[4]);
}


Console::~Console()
{
}

void Console::Draw()
{
	if (show)
	{
		App->render->Blit(App->gui->console, position.x - App->render->camera.x, position.y - App->render->camera.y, rect);
		App->render->Blit(App->gui->console, Bpos.x - App->render->camera.x, Bpos.y - App->render->camera.y, (&bar.GetCurrentFrame(App->dt)));
		
		if (text[0] != '\0' )
		{
			comand = App->font->Print(text, { 255,255,255 }, App->gui->fonts[4]);
			App->render->Blit(comand, 1 - App->render->camera.x, Bpos.y - App->render->camera.y);
			SDL_DestroyTexture(comand);
		}

		Bpos.x=CalcPosBar().x;
		
		Draw_commands();
	}
}

void Console::Draw_commands()
{
	com_length = 0;

	for (uint i = 0; i < last_comands.count(); i++)
	{
		uint x, y;
		x = y = 0;

		App->render->Blit(last_comands[i], position.x+3, position.y + com_length + 1);

		App->tex->GetSize(last_comands[i], x, y);
		com_length = com_length + y;

		if (com_length > 480)
		{
			uint auxX, auxY;
			auxX = auxY = 0;
			App->tex->GetSize(last_comands.start->data, auxX, auxY);
			com_length = com_length - auxY;
			p2List_item<SDL_Texture*>* item = last_comands.start;
			last_comands.del(item);
		}
	}
}


iPoint Console::CalcPosBar()
{
	iPoint recalculated;

	App->font->CalcSize(text, recalculated.x, recalculated.y, App->gui->fonts[4]);
	
	return recalculated;
}

void Console::InsertComand()
{
	last_comands.add(App->font->Print(text, { 255,255,255 }, App->gui->fonts[4]));
}

void Console::exec_command(const char* command)
{
	if (strcmp(command, "exit")==0)
	{
	}
	if (strcmp(command, "help") == 0)
	{
		last_comands.add(App->font->Print("buah tete do you need help?", { 0,0,0 }, App->gui->fonts[4]));
		last_comands.add(App->font->Print(" exit-> close app", { 0,0,0 }, App->gui->fonts[4]));
	}
}

