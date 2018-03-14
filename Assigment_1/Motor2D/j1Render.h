#ifndef __j1RENDER_H__
#define __j1RENDER_H__

#include <queue>
#include <vector>

#include "SDL/include/SDL.h"
#include "p2Point.h"
#include "j1Module.h"
#include "Quadtree.h"

using namespace std;

class ObjectToPrint
{
public:

	ObjectToPrint(uint priority,SDL_Texture* texture, int x, int y, const SDL_Rect* section, float scale, float speed, double angle, int pivot_x, int pivot_y, SDL_Rect rect) :
		priority(priority),texture(texture), x(x), y(y), section(section), scale(scale), speed(speed),angle(angle),pivot_x(pivot_x), pivot_y(pivot_y),rect(rect){}

	uint GetPriority()const
	{
		return priority;
	}

public:
	SDL_Texture*		texture;
	int					x;
	int					y;
	const SDL_Rect*		section;
	SDL_Rect			rect;
	float				scale;
	float				speed;
	double				angle;
	int					pivot_x;
	int					pivot_y;

	uint				priority;
};

struct OrderCrit
{
	bool operator()(const ObjectToPrint* Obj_1, const ObjectToPrint* Obj_2)const
	{
		return Obj_1->GetPriority() > Obj_2->GetPriority();
	}
};

class j1Render : public j1Module
{
public:

	j1Render();

	// Destructor
	virtual ~j1Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Load / Save
	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	// Blit
	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();
	void FillQueue(uint Priority,SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float scale = 1.0f, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);

	bool Blit(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float scale = 1.0f, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX) const;
	bool BlitFromQueue(priority_queue <ObjectToPrint*, vector<ObjectToPrint*>, OrderCrit>& Queue)const;
	bool DrawQuad(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool use_camera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool use_camera = true) const;
	bool CameraCollision(const SDL_Rect& rect)const;

	void FillVec(uint Priority, SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float scale = 1.0f, float speed = 1.0f, double angle = 0, int pivot_x = INT_MAX, int pivot_y = INT_MAX);
	void j1Render::FillQueuefromVec(vector<ObjectToPrint*> sprites);

	// Set background color
	void SetBackgroundColor(SDL_Color color);

public:

	SDL_Renderer*	renderer = nullptr;
	SDL_Rect		camera;
	SDL_Rect		viewport;

	SDL_Color		background;

	vector<ObjectToPrint*> Sprites;

	//Priority queue using the new template
	priority_queue <ObjectToPrint*,vector<ObjectToPrint*>,OrderCrit> SpriteOrderer;

private:

	bool	Optimize;
	int		reduce_camera;

	Quadtree* CullingQuadtree;
};

#endif // __j1RENDER_H__