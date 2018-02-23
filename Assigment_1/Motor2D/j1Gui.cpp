#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Fonts.h"
#include "j1Input.h"
#include "j1Gui.h"
#include "UIElements.h"
#include "Console.h"
#include "UIBackground.h"
#include "UIText.h"
#include "UIImage.h"
#include "UIButton.h"
#include "UITextBox.h"
#include "UIWindow.h"
#include "UISlider.h"
#include "j1Menu.h"
#include "j1Scene.h"


j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());

	background = App->tex->Load("gui/Background.png");
	window = App->tex->Load("gui/windows.png");
	console = App->tex->Load("gui/Console.png");

	fonts.PushBack(App->font->Load("fonts/ninja_naruto/njnaruto.ttf", 60));
	fonts.PushBack(App->font->Load("fonts/ninja_naruto/njnaruto.ttf", 50));
	fonts.PushBack(App->font->Load("fonts/ninja_naruto/SKURRI.ttf", 20));
	fonts.PushBack(App->font->Load("fonts/ninja_naruto/clock.ttf", 60));
	fonts.PushBack(App->font->Load("fonts/Consola/PIXEARG_.ttf",14));
	
	return true;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	return true;
}

bool j1Gui::PostUpdate()
{
	p2List_item<UIElements*>* element = elements.start;
	p2List_item<UIElements*>* element2 = elements.start;
	

	while (element != nullptr)
	{
		element->data->Draw();

		if (element->data->CheckMouse() == true)
		{
			if (element->data->mousein == false)
			{
				element->data->callback->GUIEvent(MOUSE_ENTER, element->data);
				element->data->mousein = true;
				element->data->mouseout = false;
			}
			if (App->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				element->data->callback->GUIEvent(MOUSE_CLICK, element->data);
			}
			if (App->input->GetMouseButtonDown(1) == KEY_UP)
			{
				element->data->callback->GUIEvent(MOUSE_STOP_CLICK, element->data);
			}

		}
		
		if (element != nullptr)
		{
			if (element->data->CheckMouse() == false && element->data->mouseout == false)
			{
				element->data->callback->GUIEvent(MOUSE_LEAVE, element->data);
				element->data->mousein = false;
				element->data->mouseout = true;
			}

			element = element->next;
		}
	}


	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		while (element2 != nullptr)
		{
			if (!element2->data->debug)
			{
				element2->data->debug = true;
			}
			else
			{
				element2->data->debug = false;
			}

			element2 = element2->next;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		tabing = true;
		tab_num++;

		if (tab_num == elements.count())
		{
			tab_num = 0;
		}

		if (elements[tab_num]->type == BUTTON)
		{
			if (last_tab != -1)
			{
				elements[last_tab]->callback->GUIEvent(MOUSE_LEAVE, elements[last_tab]);
			}
			
			last_tab = tab_num;
			
			elements[tab_num]->callback->GUIEvent(MOUSE_ENTER, elements[tab_num]);
		}
		else
		{
			while (elements[tab_num]->type != BUTTON || elements[tab_num]->show==false)
			{
				tab_num++;
				
				if (tab_num == elements.count())
				{
					tab_num = 0;
					break;
				}
			}
			
			if (last_tab != -1)
			{
				elements[last_tab]->callback->GUIEvent(MOUSE_LEAVE, elements[last_tab]);
			}

			last_tab = tab_num;

			elements[tab_num]->callback->GUIEvent(MOUSE_ENTER, elements[tab_num]);
		}		

		LOG("TAB pressed");
	}

	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN && last_tab != -1)
	{
		elements[tab_num]->callback->GUIEvent(MOUSE_CLICK, elements[tab_num]);
	}
	if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP && last_tab != -1)
	{
		elements[tab_num]->callback->GUIEvent(MOUSE_STOP_CLICK, elements[tab_num]);
		tab_num = 0;
	}

	if (startgame)
	{
		startgame = false;
		CleanUp();
	}
	return true;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	p2List_item<UIElements*>* item;
	item = elements.start;

	while (item != nullptr)
	{
		DeleteElements(item->data);
		item=item->next;
	}

	elements.clear();

	if (!active)
	{
		App->tex->UnLoad(background);
		App->tex->UnLoad(atlas);
	}
	
	return true;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}

SDL_Texture* j1Gui::GetBackground() const
{
	return background;
}

// class Gui ---------------------------------------------------

UIElements* j1Gui::AddConsole(int x, int y, UIElementType type, SDL_Rect* rect, j1Module* modul, const char* text)
{
	UIElements* element_created;
	element_created = new Console(x, y, type, rect, modul,text);
	elements.add(element_created);
	
	return element_created;
}

void j1Gui::AddBackground(int x, int y, UIElementType type, j1Module* modul)
{
	UIElements* element_created;
	element_created = new UIBackground(x, y, type, modul);
	elements.add(element_created);
}

UIElements* j1Gui::AddElementText(int x, int y, UIElementType type,uint font, int _r, int _g, int _b, j1Module* modul, const char* text,bool actualize, bool show)
{
	UIElements* element_created;
	element_created = new UIText(x, y, type, text,font,_r,_g,_b, modul,actualize,show);
	elements.add(element_created);

	return element_created;
}

void j1Gui::AddElementImage(int x, int y, UIElementType type, SDL_Rect* rect, j1Module* modul)
{
	UIElements* element_created;
	element_created = new UIImage(x, y, type, rect, modul);
	elements.add(element_created);
}

UIElements* j1Gui::AddElementButton(int x, int y, UIElementType type, SDL_Rect* RecTex, j1Module* modul, const char* text, bool show)
{
	UIElements* element_created;
	element_created = new UIButton(x, y, type, RecTex, text, modul,show);
	elements.add(element_created);

	return element_created;
}

UIElements* j1Gui::AddElementTextBox(int x, int y, UIElementType type, j1Module* modul, const char* text)
{
	UIElements* element_created;
	element_created = new UITextBox(x, y, type, text, modul);
	elements.add(element_created);

	return element_created;
}

UIElements* j1Gui::AddElementWindow(int x, int y, UIElementType type, j1Module* modul, p2List<UIElements*>* elementslist,SDL_Rect rect,bool show)
{
	UIElements* element_created;
	element_created = new UIWindow(x, y, type, modul, elementslist,rect,show);
	elements.add(element_created);

	return element_created;
}

UIElements* j1Gui::AddElementSlider(int x, int y, UIElementType type, SDL_Rect* slider, SDL_Rect* button, j1Module* modul, int id, bool show)
{
	UIElements* element_created;
	element_created = new UISlider(x, y, type, slider, button, modul, id, show);
	elements.add(element_created);
	return element_created;
}


void j1Gui::DeleteElements(UIElements* element)
{
	RELEASE(element);
}
