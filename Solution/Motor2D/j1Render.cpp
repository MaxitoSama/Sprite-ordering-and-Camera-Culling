#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Window.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"


#define VSYNC true

j1Render::j1Render() : j1Module()
{
	name.create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
j1Render::~j1Render()
{}

// Called before render is available
bool j1Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");
	bool ret = true;
	// load flags
	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(App->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		if (App->win->screen_surface->w == 1500)
		{
			camera.w = App->win->screen_surface->w-640;
			camera.h = App->win->screen_surface->h-640;
		}
		else
		{
			camera.w = App->win->screen_surface->w;
			camera.h = App->win->screen_surface->h;
		}
		
		camera.x = config.child("camera").attribute("camera_x").as_int();
		camera.y = config.child("camera").attribute("camera_y").as_int();
	}

	return ret;
}

// Called before the first frame
bool j1Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);

	Optimize = true;

	int height = App->map->data.height * 32;
	int width = App->map->data.width * 32;

	CullingQuadtree = new Quadtree({ 0,0,width,height},0);

	return true;
}

// Called each loop iteration
bool j1Render::PreUpdate()
{
	BROFILER_CATEGORY("PreUpdate Render", Profiler::Color::Magenta);
	SDL_RenderClear(renderer);
	return true;
}

bool j1Render::Update(float dt)
{
	BROFILER_CATEGORY("Update Render", Profiler::Color::Maroon);

	timer.Start();

	if (App->input->GetKey(SDL_SCANCODE_F1)==KEY_DOWN)
		Optimize = !Optimize;

	vector<ObjectToPrint*> PossibleCollision;
	SDL_Rect camera_aux = { -camera.x,-camera.y,camera.w,camera.h };

	for (int i = 0; i < Sprites.size(); i++)
	{
		CullingQuadtree->insert(Sprites[i]);
	}
	
	CullingQuadtree->FillCollisionVector(PossibleCollision, camera_aux);

	FillQueuefromVec(PossibleCollision);
	
	BlitFromQueue(SpriteOrderer);

	CullingQuadtree->Clear();
	LOG("time %f", timer.ReadMs());
	
	Sprites.clear();

	
	return true;
}

bool j1Render::PostUpdate()
{
	BROFILER_CATEGORY("PostUpdate Render", Profiler::Color::MediumAquaMarine);
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);

	LOG("Elementos de cola %d", SpriteOrderer.size());
	return true;
}

// Called before quitting
bool j1Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

// Load Game State
bool j1Render::Load(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();

	return true;
}

// Save Game State
bool j1Render::Save(pugi::xml_node& data) const
{
	pugi::xml_node cam = data.append_child("camera");

	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;

	return true;
}

void j1Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void j1Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void j1Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool j1Render::Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float img_scale, float speed, double angle, int pivot_x, int pivot_y) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	SDL_RendererFlip flag;

	if (img_scale<0)
	{
		flag = SDL_FLIP_HORIZONTAL;
		rect.w *= -img_scale;
		rect.h *= -img_scale;
	}
	else
	{
		flag = SDL_FLIP_NONE;
		rect.w *= img_scale;
		rect.h *= img_scale;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivot_x != INT_MAX && pivot_y != INT_MAX)
	{
		pivot.x = pivot_x;
		pivot.y = pivot_y;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, flag) != 0)
	{
 		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool j1Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = App->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}



//--------------------------------Camera Culling and Sprite Ordering--------------------------


//This fucntions create a new element for the Queue with the info of the class ObjectToPrint
void j1Render::FillQueue(uint Priority,SDL_Texture* texture, int x, int y, const SDL_Rect* section, float scale, float speed, double angle, int pivot_x, int pivot_y)
{
	SDL_Rect aux_rect;

	aux_rect.x = x;
	aux_rect.y = y;

	if (section != NULL)
	{
		aux_rect.w = section->w;
		aux_rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &aux_rect.w, &aux_rect.h);
	}

	//TODO 2: Create a method that check wha tto put inside the queue and what not.

	if (Optimize)
	{
		if (CameraCollision(aux_rect))
		{
			ObjectToPrint* auxObject = new ObjectToPrint(Priority, texture, x, y, section, scale, speed, angle, pivot_x, pivot_y,aux_rect);
			SpriteOrderer.push(auxObject);
		}
	}
	else
	{
		ObjectToPrint* auxObject = new ObjectToPrint(Priority, texture, x, y, section, scale, speed, angle, pivot_x, pivot_y, aux_rect);
		SpriteOrderer.push(auxObject);
	}
}

//This function prints all the elements of the queue
bool j1Render::BlitFromQueue(priority_queue<ObjectToPrint*, vector<ObjectToPrint*>, OrderCrit>& Queue)const
{
	bool ret = true;
	int NumOfBlits = 0;

	while (Queue.empty()==false)
	{
		ObjectToPrint* first = Queue.top();
		
		uint scale = App->win->GetScale();

		SDL_Rect rect;
		rect.x = (int)(camera.x * first->speed) + first->x * scale;
		rect.y = (int)(camera.y * first->speed) + first->y * scale;

		if (first->section != NULL)
		{
			rect.w = first->section->w;
			rect.h = first->section->h;
		}
		else
		{
			SDL_QueryTexture(first->texture, NULL, NULL, &rect.w, &rect.h);
		}

		SDL_RendererFlip flag;

		if (first->scale < 0)
		{
			flag = SDL_FLIP_HORIZONTAL;
			rect.w *= -first->scale;
			rect.h *= -first->scale;
		}
		else
		{
			flag = SDL_FLIP_NONE;
			rect.w *= first->scale;
			rect.h *= first->scale;
		}

		SDL_Point* p = NULL;
		SDL_Point pivot;

		if (first->pivot_x != INT_MAX && first->pivot_y != INT_MAX)
		{
			pivot.x = first->pivot_x;
			pivot.y = first->pivot_y;
			p = &pivot;
		}

		if (SDL_RenderCopyEx(renderer, first->texture, first->section, &rect, first->angle, p, flag) != 0)
		{
			LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}
		

		RELEASE(first);

		Queue.pop();
		NumOfBlits++;
	}

	App->BlitsPerFrame = NumOfBlits;

	for (int i = 0; i < App->map->QueueRects.size(); i++)
	{
		RELEASE(App->map->QueueRects[i]);
	}

	App->map->QueueRects.clear();

	return ret;
}

//Check the collision with the camera.
bool j1Render::CameraCollision(const SDL_Rect& rect)const
{
	if ((rect.x < -camera.x + camera.w && rect.x + rect.w > -camera.x) || (rect.x < -camera.x + camera.w  && rect.x + rect.w > -camera.x))
	{
		if (rect.y < -camera.y + camera.h && rect.y + rect.h > -camera.y)
		{
			return true;
		}
	}
	return false;
}

//Fill the vector with the entities that will be inserted in the quadtree
void j1Render::FillVec(uint Priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section, float scale, float speed, double angle, int pivot_x, int pivot_y)
{
	SDL_Rect aux_rect;

	aux_rect.x = x;
	aux_rect.y = y;

	if (section != NULL)
	{
		aux_rect.w = section->w;
		aux_rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &aux_rect.w, &aux_rect.h);
	}

	ObjectToPrint* auxObject = new ObjectToPrint(Priority, texture, x, y, section, scale, speed, angle, pivot_x, pivot_y, aux_rect);
	Sprites.push_back(auxObject);
}

//Fills the queue from the vector of the quadtree
void j1Render::FillQueuefromVec(vector<ObjectToPrint*> sprites)
{
	for(int i=0;i<sprites.size();i++)
	{
		if (CameraCollision(sprites[i]->rect))
		{
			SpriteOrderer.push(sprites[i]);
		}
	}
}