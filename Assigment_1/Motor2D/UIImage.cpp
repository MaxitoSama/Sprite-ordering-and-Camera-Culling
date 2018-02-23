#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "UIImage.h"


UIImage::UIImage(int x, int y, UIElementType type, SDL_Rect* rect, j1Module* modul) :UIElements(x, y, type, modul)
{
	rectangle = rect;
	Elementrect = *rect;	
	
	rectangle->h;
	
	while (rectangle->h*scale <= App->win->screen_surface->h*0.135*0.8)
	{
		scale += 0.01f; 
	}
}


UIImage::~UIImage()
{
}

void UIImage::Draw()
{
	App->render->Blit(App->gui->GetAtlas(), position.x - App->render->camera.x, position.y - App->render->camera.y, rectangle, scale);
}
