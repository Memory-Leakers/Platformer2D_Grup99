
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"


#include "Defs.h"
#include "Log.h"
#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

int Properties::GetProperty(const char* value, int defaultValue) const
{
	ListItem<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return defaultValue;
}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    folder.Create(config.child("folder").child_value());

    return ret;
}

// Draw the map (all requried layers)
void Map::Draw()
{
	if (mapLoaded == false) return;

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	ListItem<Collider*>* colItem;
	colItem = mapData.col.start;

	while (mapLayerItem != NULL) {

		if (mapLayerItem->data->properties.GetProperty("Draw") == 1) 
		{
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);

					if (gid > 0) {
						TileSet* tileset = GetTilesetFromTileId(gid);

						SDL_Rect r = tileset->GetTileRect(gid);
						iPoint pos = MapToWorld(x, y);

						//TEXTURE
						app->render->DrawTexture(tileset->texture,
							pos.x,
							pos.y,
							&r);
					}
					
				}
			}
		}

		if (mapLayerItem->data->properties.GetProperty("Navigation") == 1 && app->scene->gameScene->debugTiles)
		{
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);

					if (gid > 0) {
						TileSet* tileset = GetTilesetFromTileId(gid);

						SDL_Rect r = tileset->GetTileRect(gid);
						iPoint pos = MapToWorld(x, y);

						app->render->DrawTexture(tileset->texture,
							pos.x,
							pos.y,
							&r);
					}
				}
			}
		}

		mapLayerItem = mapLayerItem->next;
	}
}

iPoint Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	if (mapData.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x * mapData.tileWidth;
		ret.y = y * mapData.tileHeight;
	}
	else if (mapData.type == MAPTYPE_ISOMETRIC)
	{
		ret.x = (x - y) * (mapData.tileWidth / 2);
		ret.y = (x + y) * (mapData.tileHeight / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

iPoint Map::WorldToMap(int x, int y) const
{
	iPoint ret(0, 0);

	if (mapData.type == MAPTYPE_ORTHOGONAL)
	{
		ret.x = x / mapData.tileWidth;
		ret.y = y / mapData.tileHeight;
	}
	else if (mapData.type == MAPTYPE_ISOMETRIC)
	{

		float half_width = mapData.tileWidth * 0.5f;
		float half_height = mapData.tileHeight * 0.5f;
		ret.x = int((x / half_width + y / half_height) / 2);
		ret.y = int((y / half_height - (x / half_width)) / 2);
	}
	else
	{
		LOG("Unknown map type");
		ret.x = x; ret.y = y;
	}

	return ret;
}

TileSet* Map::GetTilesetFromTileId(int id) const
{
	ListItem<TileSet*>* item = mapData.tilesets.start;
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

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int id) const
{
	SDL_Rect rect = { 0 };

	int relativeId = id - firstgid;
	rect.w = tileWidth;
	rect.h = tileHeight;
	rect.x = margin + ((rect.w + spacing) * (relativeId % columns));
	rect.y = margin + ((rect.h + spacing) * (relativeId / columns));
	
	return rect;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // Remove all tilesets
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}

	// Remove all layers
	ListItem<MapLayer*>* item2;
	item2 = mapData.layers.start;

	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	mapData.layers.clear();
	mapData.tilesets.clear();

	delete item;
	item = nullptr;
	delete item2;
	item2 = nullptr;


    return true;
}

bool Map::Load(const char* filename)
{
    bool ret = true;
    SString tmp("%s%s", folder.GetString(), filename);

	pugi::xml_document mapFile; 
    pugi::xml_parse_result result = mapFile.load_file(tmp.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", filename, result.description());
        ret = false;
    }

	// Load general info
    if(ret == true)
    {
		ret = LoadMap(mapFile);
	}

    // remember to support more any number of tilesets!
	if (ret == true)
	{
		ret = LoadTileSets(mapFile.child("map"));
	}

	// Load layer info
	if (ret == true)
	{
		ret = LoadAllLayers(mapFile.child("map"));
	}

	if (ret == true)
	{
		LoadMapEntities();
	}

    mapLoaded = ret;

	tmp.Clear();

    return ret;
}

// L03: TODO: Load map general properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
	bool ret = true;
	pugi::xml_node map = mapFile.child("map");

	if (map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		// L03: DONE 3: Load map general properties
		mapData.height = map.attribute("height").as_int();
		mapData.width = map.attribute("width").as_int();
		mapData.tileHeight = map.attribute("tileheight").as_int();
		mapData.tileWidth = map.attribute("tilewidth").as_int();

		// L05: DONE 1: Add formula to go from isometric map to world coordinates
		mapData.type = MAPTYPE_UNKNOWN;
		if (strcmp(map.attribute("orientation").as_string(), "isometric") == 0)
		{
			mapData.type = MAPTYPE_ISOMETRIC;
		}
		if (strcmp(map.attribute("orientation").as_string(), "orthogonal") == 0)
		{
			mapData.type = MAPTYPE_ORTHOGONAL;
		}
	}

	return ret;
}

// L03: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSets(pugi::xml_node mapFile) {

	bool ret = true;

	pugi::xml_node tileset;
	for (tileset = mapFile.child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();
		if (ret == true) ret = LoadTilesetDetails(tileset, set);
		if (ret == true) ret = LoadTilesetImage(tileset, set);
		mapData.tilesets.add(set);
	}

	return ret;
}

// L03: DONE 4: Load Tileset attributes
bool Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;

	// L03: DONE 4: Load Tileset attributes
	set->name.Create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tileWidth = tileset_node.attribute("tilewidth").as_int();
	set->tileHeight = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	set->tilecount = tileset_node.attribute("tilecount").as_int();
	set->columns = tileset_node.attribute("columns").as_int();

	return ret;
}

// L03: DONE 4: Load Tileset image
bool Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
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
		// L03: DONE 4: Load Tileset image
		SString tmp("%s%s", folder.GetString(), image.attribute("source").as_string());
		set->texture = app->tex->Load(tmp.GetString());
		tmp.Clear();
	}

	

	return ret;
}

// L04: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	//Load the attributes
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_int();
	layer->height = node.attribute("height").as_int();

	//L06: TODO 6 Call Load Propoerties
	LoadProperties(node, layer->properties);

	//Reserve the memory for the tile array
	layer->data = new uint[layer->width * layer->height];
	memset(layer->data, 0, layer->width * layer->height);

	//Iterate over all the tiles and assign the values
	pugi::xml_node tile;
	int i = 0;
	for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
	{
		layer->data[i] = tile.attribute("gid").as_int();

		i++;
	}

	return ret;
}

// L04: DONE 4: Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
	bool ret = true;
	for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
	{
		//Load the layer
		MapLayer* mapLayer = new MapLayer();
		ret = LoadLayer(layerNode, mapLayer);

		//add the layer to the map
		mapData.layers.add(mapLayer);
	}

	return ret;
}

// L06: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property *p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_int();

		properties.list.add(p);
	}
	
	return ret;
}

void Map::LoadMapEntities ()
{
	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = mapData.layers.start;

	while (mapLayerItem != NULL)
	{
		if (mapLayerItem->data->properties.GetProperty("Navigation"))
		{
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);

					switch(gid) 
					{
						case 246://FRUITS
							app->scene->gameScene->em.addEntity(new Coin(x * 16 - 8, y * 16 - 8));
							break;
						case 250: //SPIKE TOP
							app->scene->gameScene->em.addEntity(new Trap(x * 16, y * 16));
							break;
						case 251: //SPIKE BOTTOM
							app->scene->gameScene->em.addEntity(new Trap(x * 16, y * 16, InteractablesId::SPIKETRAP_BOTTOM));
							break;
						case 252: //SPIKE RIGHT
							app->scene->gameScene->em.addEntity(new Trap(x * 16, y * 16, InteractablesId::SPIKETRAP_RIGHT));
							break;
						case 253: //SPIKE LEFT
							app->scene->gameScene->em.addEntity(new Trap(x * 16, y * 16, InteractablesId::SPIKETRAP_LEFT));
							break;
						case 255: //WALKING ENEMY
							app->scene->gameScene->em.addEntity(new WalkingEnemy(x * 16, y * 16 - 14));
							break;
						case 256: //FLYING ENEMY
							app->scene->gameScene->em.addEntity(new FlyingEnemy(x * 16, y * 16));
							break;
						case 257: //HEARTH
							app->scene->gameScene->em.addEntity(new HealthPack(x * 16, y * 16));
							break;
					}
				}
			}
		}

		mapLayerItem = mapLayerItem->next;
	}

	delete mapLayerItem;
	mapLayerItem = nullptr;
}

bool Map::SaveState(pugi::xml_node& data) const
{

	//SAVE FRUITS TO SAVE_FILE
	//ListItem<Pickable*>* fruitItem;
	//fruitItem = mapData.fruits.start;

	//while (data.child("fruits").child("f"))
	//{
	//	data.child("fruits").remove_child("f");
	//}

	//while (fruitItem != NULL)
	//{
	//	if (fruitItem->data != nullptr)
	//	{
	//		if (!fruitItem->data->dead)
	//		{
	//			pugi::xml_node f = data.child("fruits").append_child("f");
	//			f.append_attribute("posX") = fruitItem->data->position.x;
	//			f.append_attribute("posY") = fruitItem->data->position.y;
	//			f.append_attribute("id") = (int) fruitItem->data->pickable_id;
	//		}
	//	}
	//	
	//	fruitItem = fruitItem->next;
	//}

	//delete fruitItem;
	//fruitItem = nullptr;

	////SAVE ENEMIES TO SAVE_FILE

	//while (data.child("enemies").child("e"))
	//{
	//	data.child("enemies").remove_child("e");
	//}

	//ListItem<Enemy*>* enemyItem;
	//enemyItem = mapData.enemies.start;

	//while (enemyItem != NULL)
	//{
	//	if (enemyItem->data != nullptr)
	//	{
	//		pugi::xml_node e = data.child("enemies").append_child("e");
	//		e.append_attribute("posX") = enemyItem->data->position.x;
	//		e.append_attribute("posY") = enemyItem->data->position.y;
	//		e.append_attribute("id") = enemyItem->data->enemy_id;
	//	}
	//	enemyItem = enemyItem->next;
	//}

	//delete enemyItem;
	//enemyItem = nullptr;


	return true;
}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
	bool ret = false;
	ListItem<MapLayer*>* item;
	item = mapData.layers.start;

	for (item = mapData.layers.start; item != NULL; item = item->next)
	{
		MapLayer* layer = item->data;

		if (layer->properties.GetProperty("walkable", 0) == 0)
			continue;

		uchar* map = new uchar[layer->width * layer->height];
		memset(map, 1, layer->width * layer->height);

		for (int y = 0; y < mapData.height; ++y)
		{
			for (int x = 0; x < mapData.width; ++x)
			{
				if(layer->Get(x,y) == 254)
				{
					map[(y * layer->width) + x] = 1;
				}
				else 
				{
					map[(y * layer->width) + x] = 0;

				}
			}
		}

		*buffer = map;
		width = mapData.width;
		height = mapData.height;
		ret = true;

		break;
	}

	return ret;
}