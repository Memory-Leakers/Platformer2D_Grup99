#ifndef __APP_H__
#define __APP_H__

#include "Defs.h"
#include "Log.h"
#include "List.h"
#include "Module.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "Map.h"
#include "ModuleCollisions.h"
#include "ModuleFonts.h"
#include "Timer.h"
#include "PerformanceTimer.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"SaveFiles/config.xml"
#define SAVE_STATE_FILENAME "SaveFiles/save_game.xml"
#define PLAYER_FILENAME "SaveFiles/player.xml"
#define ENEMY_FILENAME "SaveFiles/GameObjects/enemy.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
//class GameScene;
class Map;
class ModuleCollisions;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest();
	void SaveGameRequest() const;

	pugi::xml_node LoadPlayer(pugi::xml_document&) const;

	pugi::xml_node LoadWalkingEnemy(pugi::xml_document&) const;

	bool IsWalkable(const iPoint& pos) const;

	uchar GetTileAt(const iPoint& pos) const;

	void SetMap(uint width, uint height, uchar* data);

	bool CheckBoundaries(const iPoint& pos) const;
	

private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	//GameScene* gameScene;
	Map* map;
	ModuleCollisions* col;
	ModuleFonts* font;


	Timer globalTime;
	Timer gameTime;
	
	uchar* mapo;

	int FPS = 60;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	//pugi::xml_document configFile;
	//pugi::xml_node config;
	//pugi::xml_node configApp;

	// L07: DONE 4: Calculate some timing measures
	// required variables are provided:
	PerformanceTimer* ptimer;
	PerformanceTimer* frameDuration;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;
	float dt = 0.0f;

	uint32 maxFrameRate = 0;


	uint frames;

	mutable bool saveGameRequested;
	bool loadGameRequested;

	// size of the map
	uint width;
	uint height;
};

extern App* app;

#endif	// __APP_H__