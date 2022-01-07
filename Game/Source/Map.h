#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"
#include "List.h"
#include "Point.h"
#include "Collider.h"
#include "Coin.h"
#include "HealthPack.h"
#include "Trap.h"

#include "PugiXml\src\pugixml.hpp"

struct TileSet
{
	SString	name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;

	SDL_Texture* texture;
	int	texWidth;
	int	texHeight;
	int	tilecount;
	int	columns;

	SDL_Rect GetTileRect(int id) const;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct Properties
{
	struct Property
	{
		SString name;
		int value;
	};
	
	~Properties()
	{
		ListItem<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int GetProperty(const char* name, int default_value = 0) const;

	List<Property*> list;
};

struct MapLayer
{
	SString	name;
	int width;
	int height;
	uint* data;

	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	SDL_Color backgroundColor;
	MapTypes type;
	List<TileSet*> tilesets;

	List<MapLayer*> layers;

	List<Collider*> col;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake(pugi::xml_node& conf);

    // Called each loop iteration
    void Draw();

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y) const;

	void LoadMapEntities();

	bool SaveState(pugi::xml_node&) const override;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:
	bool LoadMap(pugi::xml_node mapFile);
	bool LoadTileSets(pugi::xml_node mapFile);
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);

	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:
	MapData mapData;

private:

    SString folder;
    bool mapLoaded;
};

#endif // __MAP_H__