#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "UISlider.h"
#include "j1Input.h"
#include "j1Audio.h"


UISlider::UISlider(int x, int y, UIElementType type, SDL_Rect* slider, SDL_Rect* button, j1Module* modul, int id, bool show) :UIElements(x, y, type, modul)
{
	this->show = show;
	this->slider = slider;
	this->button = button;
	Elementrect = *button;
	this->id = id;

	button->h;
	while (button->h*scale <= App->win->screen_surface->h*0.0952)
	{
		scale += 0.01f;//scale 0.5f
	}

	collider_size_x = button->w;
	collider_size_y = button->h;

	button_point.x = position.x - App->render->camera.x - button->w / 2;
	button_point.y = position.y - App->render->camera.y + 55 - button->h / 2;
}

UISlider::~UISlider()
{

}

void UISlider::Draw()
{
	if (show)
	{
		int rect_x;
		int rect_y;

		slider_end = position.x + slider->w*0.5;
		slider_begining = position.x - slider->w * 0.50;

		rect_x = button_point.x+ (int)(collider_size_x*scale / 2);
		rect_y = button_point.y+ (int)(collider_size_y*scale)/3;

		Elementrect = { rect_x-45,rect_y-40,(int)(collider_size_x*scale),(int)(collider_size_y*scale) };

		//slider
		App->render->Blit(App->gui->window, position.x - App->render->camera.x - slider->w / 2, position.y - App->render->camera.y - slider->h / 2, slider, scale*2);
		
		//button
		App->render->Blit(App->gui->GetAtlas(), button_point.x, button_point.y, button, scale);

		if (debug == true)
		{
			App->render->DrawQuad(Elementrect, 255, 0, 255, 80);
		}
		Action();
	}

}

void UISlider::Action()
{
	if (action)
	{
		App->input->GetMousePosition(mouse_position.x, mouse_position.y);
		if (mouse_position.x < slider_end)
		{
			if (mouse_position.x > slider_begining)
			{
				button_point.x = mouse_position.x - (int)(collider_size_x*scale / 2);
				if (id == 1) //sound
				{
					int volume = 128;
					volume = (button_point.x - slider_begining) * 150 / (slider_end- slider_begining);
					App->audio->fx_volume = volume;
				}
				if (id == 2) //music
				{
					int volume = 128;
					volume = (button_point.x - slider_begining) * 150 / (slider_end - slider_begining);
					App->audio->music_volume = volume;
				}
			}
		}
	}
}
