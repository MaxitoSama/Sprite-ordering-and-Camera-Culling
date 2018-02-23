#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1Colliders.h"
#include "j1Entities.h"

struct MapLayer 
{
	inline uint Get(int x, int y) const;

	p2SString		name;

	uint			width;
	uint			height;
	uint*			data = nullptr;
	uint			size = 0;

	~MapLayer();
};

struct TileCollider
{
	COLLIDER_TYPE		type;
	ENTITY_TYPES		entype;
	uint				tile_id;
	int					collider_x;
	int					collider_y;
	uint				collider_height;
	uint				collider_width;
};

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	p2SString				name;
	int						firstgid;
	int						margin;
	int						spacing;
	int						tile_width;
	int						tile_height;
	SDL_Texture*			texture=nullptr;
	int						tex_width;
	int						tex_height;
	int						num_tiles_width;
	int						num_tiles_height;
	int						offset_x;
	int						offset_y;
	p2List<TileCollider*>	colliders;
};

struct Image_Background
{
	p2SString				name;
	uint					width;
	uint					height;
	SDL_Texture*			Image = nullptr;
	float					Pvelocity;

	~Image_Background();
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int							width;
	int							height;
	int							tile_width;
	int							tile_height;
	SDL_Color					background_color;
	MapTypes					type;
	p2List<TileSet*>			tilesets;
	p2List<MapLayer*>			layers;
	p2List<Image_Background*>	backgrounds;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	//Translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	//Translates x,y coordinates from world positions to map positions
	iPoint WorldToMap(int x, int y) const;

	int MovementCost(int x, int y) const;
	
	// Draw colliders
	void Draw_Colliders();

	//Load Entities from Tiel
	void LoadEntities();

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadBackground(pugi::xml_node& Image_node, Image_Background* background);
	TileSet* j1Map::GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};
#endif // __j1MAP_H__