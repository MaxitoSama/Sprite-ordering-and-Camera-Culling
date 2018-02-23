#include "p2Defs.h"
#include "p2DynArray.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Colliders.h"
#include "j1Pathfinding.h"
#include "j1Map.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}



//draw the map and background
void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	uint tile_indx;
	uint layer_indx;
	uint background_indx=0;
	
	for (background_indx = 0; background_indx < data.backgrounds.count(); background_indx++)
	{
		App->render->Blit(data.backgrounds[background_indx]->Image, -App->render->camera.x, -App->render->camera.y, NULL, 1.0f);
	}

	p2List_item<MapLayer*>* item = data.layers.start;

	for (; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int tile_id = layer->Get(x, y);
				if (tile_id > 0)
				{
					TileSet* tileset = GetTilesetFromTileId(tile_id);

					SDL_Rect r = tileset->GetTileRect(tile_id);
					iPoint pos = MapToWorld(x, y);

					App->render->Blit(tileset->texture, pos.x, pos.y, &r);
				}
			}
		}
	}
}

//draw the colliders
void j1Map::Draw_Colliders()
{
	int counter_x = 0;
	int counter_y = 0;
	
	uint tile_indx=0;
	uint layer_indx;

	//draw the horizontal colliders together
	for (layer_indx = 0; layer_indx < data.layers.count(); layer_indx++)
	{

		for (int j = 0; j < data.height; j++)
		{
			for (int i = 0; i < data.width; i++)
			{
				uint id = data.layers[layer_indx]->Get(i, j);
				if (id != 0)
				{
					if (data.layers[layer_indx]->Get(i+1,j) == id)
					{
						counter_x++;
						continue;
					}

					else
					{
						for (uint indx = 0; indx < data.tilesets[tile_indx]->colliders.count(); indx++)
						{
							uint collider_num = data.tilesets[tile_indx]->colliders[indx]->tile_id;
							if (id - data.tilesets[tile_indx]->firstgid == collider_num && data.tilesets[tile_indx]->colliders[indx]->type==COLLIDER_FLOOR)
							{
								int x = MapToWorld(i - counter_x, j- counter_y).x;
								int y = MapToWorld(i - counter_x, j- counter_y).y;
								SDL_Rect collider_rec = { x,y,data.tile_width*(counter_x +1),data.tile_height*(counter_y+1) };
								App->colliders->AddCollider(collider_rec, data.tilesets[tile_indx]->colliders[indx]->type);
							}
						}
						counter_x = 0;
					}
				}
			}

		}

		//draw the vertical colliders together
		for (int i = 0; i < data.width; i++)
		{
			for (int j = 0; j < data.height; j++)
			{
				uint id = data.layers[layer_indx]->Get(i, j);
				if (id != 0)
				{
					if (data.layers[layer_indx]->Get(i, j + 1) == id)
					{
						counter_y++;
						continue;
					}
					else
					{
						for (uint indx = 0; indx < data.tilesets[tile_indx]->colliders.count(); indx++)
						{
							uint collider_num = data.tilesets[tile_indx]->colliders[indx]->tile_id;
							if (id - data.tilesets[tile_indx]->firstgid == collider_num && data.tilesets[tile_indx]->colliders[indx]->type == COLLIDER_WALL && 
								data.tilesets[tile_indx]->colliders[indx]->entype != COIN)
							{
								int x = MapToWorld(i , j - counter_y).x;
								int y = MapToWorld(i , j - counter_y).y;
								SDL_Rect collider_rec = { x,y,data.tile_width,data.tile_height*(counter_y +1)};
								App->colliders->AddCollider(collider_rec, data.tilesets[tile_indx]->colliders[indx]->type);
							}
						}
						counter_y = 0;
					}
				}
			}
		}
	}

	App->colliders->AddCollider({ 0,1080,1000000,10 }, COLLIDER_DEATH);
}

void j1Map::LoadEntities()
{
	int counter_x = 0;
	int counter_y = 0;

	uint tile_indx = 0;
	uint layer_indx;

	//draw the horizontal colliders together
	for (layer_indx = 0; layer_indx < data.layers.count(); layer_indx++)
	{
		for (int j = 0; j < data.height; j++)
		{
			for (int i = 0; i < data.width; i++)
			{
				uint id = data.layers[layer_indx]->Get(i, j);
				if (id != 0)
				{

					for (uint indx = 0; indx < data.tilesets[tile_indx]->colliders.count(); indx++)
					{
						uint collider_num = data.tilesets[tile_indx]->colliders[indx]->tile_id;
						if (id - data.tilesets[tile_indx]->firstgid == collider_num && data.tilesets[tile_indx]->colliders[indx]->entype == COIN)
						{
							int x = MapToWorld(i,j).x;
							int y = MapToWorld(i,j).y;
							App->entities->AddEnemy(COIN, x, y);
						}
					}

				}
			}
		}

	}
}


TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

iPoint j1Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}

int j1Map::MovementCost(int x, int y) const
{
	int ret = -1;

	if (x >= 0 && x < data.width && y >= 0 && y < data.height)
	{
		int id = data.layers.start->next->next->next->next->data->Get(x, y);

		if (id == 0)
			ret = 3;
		else
			ret = 0;
	}
	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	p2List_item<MapLayer*>* item_l;
	p2List_item<Image_Background*>* item_I;

	item = data.tilesets.start;
	item_l = data.layers.start;
	item_I = data.backgrounds.start;

	while (item != NULL)
	{
		App->tex->UnLoad(item->data->texture);
		RELEASE(item->data);
		item = item->next;
	}
	
	while (item_l != NULL)
	{
		RELEASE(item_l->data);
		item_l = item_l->next;
	}

	while (item_I != NULL)
	{
		App->tex->UnLoad(item_I->data->Image);
		item_I = item_I->next;
	}

	data.tilesets.clear();

	data.layers.clear();

	data.backgrounds.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if (ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for (tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if (ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if (ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	//Load all backgrounds images ------------------------------------------
	pugi::xml_node background;
	for (background = map_file.child("map").child("imagelayer"); background && ret; background = background.next_sibling("imagelayer"))
	{
		Image_Background* set = new Image_Background();

		if (ret == true)
		{
			ret = LoadBackground(background, set);
		}

		data.backgrounds.add(set);
	}

	// Load layer info ----------------------------------------------
	pugi::xml_node layer;

	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(layer, set);
		}

		data.layers.add(set);
	}

	if (ret == true)
	{
		/*LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);*/

		p2List_item<TileSet*>* item = data.tilesets.start;
		while (item != NULL)
		{
			TileSet* s = item->data;
			/*LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);*/
			item = item->next;
		}

		p2List_item<MapLayer*>* item_layer = data.layers.start;
		while (item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			/*LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);*/
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if (bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if (v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if (orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if (orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if (orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();

	for (pugi::xml_node tile = tileset_node.child("tile"); tile; tile = tile.next_sibling())
	{
		TileCollider* aux_collider=new TileCollider();

		aux_collider->tile_id = tile.attribute("id").as_uint();
		aux_collider->collider_x= tile.child("objectgroup").child("object").attribute("x").as_int();
		aux_collider->collider_y = tile.child("objectgroup").child("object").attribute("y").as_int();
		aux_collider->collider_height = tile.child("objectgroup").child("object").attribute("height").as_uint();
		aux_collider->collider_width = tile.child("objectgroup").child("object").attribute("width").as_uint();

		p2SString type(tile.child("objectgroup").child("object").attribute("type").as_string());

		if (type == "COLLIDER_NONE")
		{
			aux_collider->type = COLLIDER_NONE;
		}
		else if (type == "COLLIDER_WALL")
		{
			aux_collider->type = COLLIDER_WALL;
		}
		else if (type == "COLLIDER_FLOOR")
		{
			aux_collider->type = COLLIDER_FLOOR;
		}
		else if (type == "COIN")
		{
			aux_collider->entype = COIN;
		}
		else
		{
			aux_collider->type = COLLIDER_NONE;
		}

		set->colliders.add(aux_collider);
	}
	
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if (set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if (set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	uint i = 0;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint(0);
	layer->height = node.attribute("height").as_uint(0);
	layer->size = layer->width*layer->height;

	layer->data = new uint[layer->size];
	memset(layer->data, 0, sizeof(uint)*layer->size);


	for (pugi::xml_node aux = node.child("data").child("tile"); aux; aux = aux.next_sibling("tile"))
	{
		layer->data[i++] = aux.attribute("gid").as_uint();
	}

	return true;
}

bool j1Map::LoadBackground(pugi::xml_node& Image_node, Image_Background* background)
{
	bool ret = true;
	pugi::xml_node image = Image_node.child("image");

	background->name = Image_node.attribute("name").as_string();
	background->width = Image_node.child("image").attribute("width").as_uint();
	background->height = Image_node.child("image").attribute("height").as_uint();
	//background->Pvelocity =App->player->speed *(-1);

	if (image == NULL)
	{
		LOG("Error parsing Image_Background xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		background->Image = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(background->Image, NULL, NULL, &w, &h);

		if (background->width <= 0)
		{
			background->width = w;
		}

		background->height = image.attribute("height").as_int();

		if (background->height <= 0)
		{
			background->height = h;
		}
	}

	return ret;
}

MapLayer::~MapLayer()
{
	delete[] data;
}

inline uint MapLayer::Get(int x, int y) const
{
	return data[(y*width) + x];
}

Image_Background::~Image_Background()
{
	delete Image;
}