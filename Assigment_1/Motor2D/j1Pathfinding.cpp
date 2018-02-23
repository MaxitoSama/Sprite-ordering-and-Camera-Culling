#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Map.h"
#include "j1Entities.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
}

bool j1PathFinding::Start()
{
	PathTile = App->tex->Load("maps/PathTile.png");
	return true;
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");
	
	frontier.Clear();
	breadcrumbs.clear();
	visited.clear();

	return true;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetPath() const
{
	return &path;
}

void j1PathFinding::ResetPath()
{
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	
	memset(cost_so_far, 0, sizeof(uint) * COST_MAP * COST_MAP);
}

void j1PathFinding::Path(int x, int y, p2DynArray<iPoint>& path)
{
	path.Clear();
	iPoint goal = App->map->WorldToMap(x, y);
	iPoint curr = goal;
	p2List_item<iPoint>* item;

	item = breadcrumbs.end;
	path.PushBack(curr);

	while (item != breadcrumbs.start && visited.find(curr) != -1)
	{
		curr = breadcrumbs[visited.find(curr)];
		path.PushBack(curr);
		item = item->prev;
	}
	
	path.Flip();
}

void j1PathFinding::DrawPath(p2DynArray<iPoint>& path)
{
	iPoint point;

	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path[i].x, path[i].y);
		App->render->Blit(PathTile, pos.x, pos.y);
	}
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------

int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	ResetPath();

	int ret = 0;
	iPoint goal = App->map->WorldToMap(destination.x, destination.y);

	if (App->map->MovementCost(goal.x, goal.y) == -1 || App->map->MovementCost(goal.x, goal.y) == 0)
	{
		ret = -1;
	}

	if (ret != -1)
	{
		iPoint curr;

		frontier.Push(App->map->WorldToMap(origin.x, origin.y), 0);

		while (frontier.Count() != 0)
		{
			if (curr == goal)
			{
				break;
			}
			if (frontier.Pop(curr))
			{
				iPoint neighbors[8];
				neighbors[0].create(curr.x + 1, curr.y + 0);
				neighbors[1].create(curr.x + 0, curr.y + 1);
				neighbors[2].create(curr.x - 1, curr.y + 0);
				neighbors[3].create(curr.x + 0, curr.y - 1);
				neighbors[4].create(curr.x + 1, curr.y + 1);
				neighbors[5].create(curr.x - 1, curr.y - 1);
				neighbors[6].create(curr.x - 1, curr.y + 1);
				neighbors[7].create(curr.x + 1, curr.y - 1);


				for (uint i = 0; i < 8; ++i)
				{
					uint Distance = neighbors[i].DistanceTo(goal);

					if (App->map->MovementCost(neighbors[i].x, neighbors[i].y) > 0)
					{
						if (breadcrumbs.find(neighbors[i]) == -1 && visited.find(neighbors[i]) == -1)
						{
							cost_so_far[neighbors[i].x][neighbors[i].y] = Distance;
							frontier.Push(neighbors[i], Distance);							
							visited.add(neighbors[i]);
							breadcrumbs.add(curr);
						}
					}
				}
			}
		}
	}

	return ret;
}


