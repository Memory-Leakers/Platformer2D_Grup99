#include "App.h"
#include "Defs.h"
#include "Log.h"
#include "Optick/include/optick.h"

#include <iostream>
#include <sstream>

#define INVALID_WALK_CODE 243 

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	win = new Window();
	input = new Input();	
	tex = new Textures();
	audio = new Audio();
	map = new Map();
	col = new ModuleCollisions();
	scene = new Scene();
	font = new ModuleFonts();
	guiManager = new GuiManager();
	


	render = new Render();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(audio);
	AddModule(tex);
	AddModule(col);
	AddModule(scene);
	AddModule(map);
	AddModule(guiManager);
	AddModule(font);
	
	


	// Render last to swap buffer
	AddModule(render);

	//TIMERS APP
	ptimer = new PerformanceTimer();
	frameDuration = new PerformanceTimer();

	//Genearate savefiles
	GenerateSaveFile();

}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}
	RELEASE_ARRAY(mapo);
	modules.clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool App::Awake()
{
	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	bool ret = false;

	// L01: DONE 3: Load config from XML
	config = LoadConfig(configFile);

	if (config.empty() == false)
	{
		ret = true;
		configApp = config.child("app");

		// L01: DONE 4: Read the title from the config file
		title.Create(configApp.child("title").child_value());
		organization.Create(configApp.child("organization").child_value());
		app->win->SetTitle(title.GetString());
	}
	
	if (ret == true)
	{
		ListItem<Module*>* item;
		item = modules.start;

		while ((item != NULL) && (ret == true))
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	globalTime.Update();

	if (globalTime.getDeltaTime() >= 1.0f / FPS && app->render->VSync)
	{
		PrepareUpdate();
		gameTime.Update();

		if (input->GetWindowEvent(WE_QUIT) == true)
			ret = false;

		if (ret == true)
			ret = PreUpdate();

		if (ret == true)
			ret = DoUpdate();

		if (ret == true)
			ret = PostUpdate();

		FinishUpdate();
		globalTime.Reset();
	}
	else if (app->render->VSync == false)
	{
		PrepareUpdate();
		gameTime.Update();

		if (input->GetWindowEvent(WE_QUIT) == true)
			ret = false;

		if (ret == true)
			ret = PreUpdate();

		if (ret == true)
			ret = DoUpdate();

		if (ret == true)
			ret = PostUpdate();

		FinishUpdate();
	}
	else
	{
		gameTime.Update();
	}
	
	
	return ret;
}

// Load config from XML file
// NOTE: Function has been redesigned to avoid storing additional variables on the class
pugi::xml_node App::LoadConfig(pugi::xml_document& configFile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = configFile.load_file(CONFIG_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	else ret = configFile.child("config");

	return ret;
}

pugi::xml_node App::LoadPlayer(pugi::xml_document& playerfile) const
{

	pugi::xml_node ret;

	pugi::xml_parse_result result = playerfile.load_file(PLAYER_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", PLAYER_FILENAME, result.description());
	else ret = playerfile.child("player_state");

	return ret;
}

pugi::xml_node App::LoadWalkingEnemy(pugi::xml_document& walkingenemyfile) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = walkingenemyfile.load_file(ENEMY_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", ENEMY_FILENAME, result.description());
	else ret = walkingenemyfile.child("enemy_state");

	return ret;
	
}

bool App::IsWalkable(const iPoint& pos) const
{
	//Determinar bien la condicion
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

uchar App::GetTileAt(const iPoint& pos) const
{
	//Determinar bien la condicion
	//check boundaries
	if (CheckBoundaries(pos))
	{
		return mapo[(pos.y * map->mapData.width) + pos.x];
	}
	return INVALID_WALK_CODE;
}

void App::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;
	//RELEASE_ARRAY(mapo);
	
	mapo = new uchar[width * height];
	memcpy(mapo, data, width * height);
	
}

bool App::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	frameCount++;
	lastSecFrameCount++;
	

	// L08: DONE 4: Calculate the dt: differential time since last frame
	dt = frameDuration->ReadMs();
	frameDuration->Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	OPTICK_EVENT();
	// L02: DONE 1: This is a good place to call Load / Save methods
	if (loadGameRequested == true) LoadGame();
	if (saveGameRequested == true) SaveGame();

	float secondsSinceStartup = startupTime.getExecuteTime(true);

	lastSecFrameTime.Update();
	if (lastSecFrameTime.getDeltaTime() > 1) {
		lastSecFrameTime.Reset();
		framesPerSecond = lastSecFrameCount;
		lastSecFrameCount = 0;
		averageFps = (averageFps + framesPerSecond) / 2;
	}

	static char title[256];
	sprintf_s(title, 256, "Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %.0fs Frame Count: %I64u VSync: %s",
		averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount, render->VSync ? "ON" : "OFF");

	// L08: DONE 2: Use SDL_Delay to make sure you get your capped framerate
	float delay = float(maxFrameRate) - frameDuration->ReadMs();
	//LOG("F: %f Delay:%f", frameDuration->ReadMs(), delay);

	// L08: DONE 3: Measure accurately the amount of time SDL_Delay() actually waits compared to what was expected
	PerformanceTimer* delayt = new PerformanceTimer();
	delayt->Start();
	if (maxFrameRate > 0 && delay > 0) SDL_Delay(delay);
	//LOG("Expected %f milliseconds and the real delay is % f", delay, delayt->ReadMs());

	app->win->SetTitle(title);
	delete delayt;
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;

	OPTICK_EVENT();
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	OPTICK_EVENT();
	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}
	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;

	pugi::xml_document* configFile = new pugi::xml_document();
	configFile->load_file(CONFIG_FILENAME);

	pugi::xml_parse_result result = configFile->load_file(CONFIG_FILENAME);

	ListItem<Module*>* item;
	item = modules.end;
	while (item != NULL && ret == true)
	{
		ret = item->data->SaveSettings(configFile->child("config").child(item->data->name.GetString()));
		item = item->prev;
	}
	item = modules.end;
	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}
	if (mapo != nullptr)
	{
		RELEASE_ARRAY(mapo);
	}
	configFile->save_file(CONFIG_FILENAME);

	delete item;
	item = nullptr;
	delete configFile;
	configFile = nullptr;

	return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void App::LoadGameRequest()
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist
	loadGameRequested = true;
}

// ---------------------------------------
void App::SaveGameRequest() const
{
	// NOTE: We should check if SAVE_STATE_FILENAME actually exist and... should we overwriten
	saveGameRequested = true;
}

/*LOAD GAME*/
bool App::LoadGame()
{
	bool ret = true;

	//app->scene->LoadState();
	pugi::xml_document* doc = new pugi::xml_document();
	doc->load_file(SAVE_STATE_FILENAME);

	pugi::xml_parse_result result = doc->load_file(SAVE_STATE_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_STATE_FILENAME, result.description());
	else ret = doc->child("game_state");

	ListItem<Module*>* item;
	item = modules.start;

	cout << "First->" << doc->child("game_state").child("scene").child("player").attribute("posX").as_int() << endl;

	while (item != NULL && ret == true)
	{
		if(item->data->LoadState(doc->child("game_state").child(item->data->name.GetString())))
		{
			LOG("%s load correctly", item->data->name.GetString());
		}
		else
		{
			LOG("%s wasn't able to be loaded", item->data->name.GetString());
		}
		item = item->next;
	}
	loadGameRequested = false;

	//Clean
	delete doc;
	doc = nullptr;
	delete item;
	item = nullptr;

	return ret;
}

/*SAVE GAME*/
bool App::SaveGame() const
{
	bool ret = true;

	pugi::xml_document* doc = new pugi::xml_document();
	doc->load_file(SAVE_STATE_FILENAME);

	pugi::xml_parse_result result = doc->load_file(SAVE_STATE_FILENAME);

	if (result == NULL) LOG("Could not load xml file: %s. pugi error: %s", SAVE_STATE_FILENAME, result.description());
	else ret = doc->child("game_state");

	ListItem<Module*>* item;
	item = modules.start;
	while (item != NULL && ret == true)
	{
		if (item->data->SaveState(doc->child("game_state").child(item->data->name.GetString())))
		{
			LOG("%s saved correctly", item->data->name.GetString());
		}
		else
		{
			LOG("%s wasn't able to be saved", item->data->name.GetString());
		}
		item = item->next;
	}
	doc->save_file(SAVE_STATE_FILENAME);

	saveGameRequested = false;

	//Clean
	delete doc;
	doc = nullptr;
	delete item;
	item = nullptr;

	return ret;
}

void App::GenerateSaveFile()
{
	bool ret = true;

	pugi::xml_document* doc = new pugi::xml_document();
	doc->load_file(SAVE_STATE_FILENAME);

	pugi::xml_parse_result result = doc->load_file(SAVE_STATE_FILENAME);

	if (result == NULL)
	{
		LOG("Generating a new raw save file");
		pugi::xml_node node = doc->append_child("game_state");
		node.append_child("input");
		node.append_child("window");
		node.append_child("textures");
		node.append_child("audio");
		pugi::xml_node s_node = node.append_child("scene");
		node.append_child("map");

		s_node.append_child("raw").append_attribute("value") = true;


		pugi::xml_node p_node = s_node.append_child("player");
		p_node.append_attribute("posX");
		p_node.append_attribute("posY");
		p_node.append_attribute("score");
		p_node.append_attribute("high_score");
		p_node.append_attribute("health");

		s_node.append_child("playtime").append_attribute("value");

		pugi::xml_node ss_node = s_node.append_child("startScene");
		ss_node.append_attribute("value");
		ss_node.append_attribute("doorKey");

		s_node.append_child("background").append_attribute("value");
		s_node.append_child("pickables");
		s_node.append_child("interactables");
		s_node.append_child("enemies");

		doc->save_file(SAVE_STATE_FILENAME);
	}
	
	delete doc;
	doc = nullptr;
}


