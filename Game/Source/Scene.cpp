#include "Scene.h"


Scene::Scene() : Module()
{
	name.Create("scene");
	gameScene = new GameScene();
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	levelList.add(new Level(1, "Level1.tmx"));
	levelList[0]->camX = 209;
	levelList[0]->camY = -2401;

	gameScene->Start();

	// L03: DONE: Load map
	app->map->Load(levelList[0]->file.GetString());

	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// Draw map
	app->map->Draw();

	gameScene->Update(dt);

    // LOAD AND SAVE
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}

	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 2;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 2;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 2;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 2;	

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());

	app->win->SetTitle(title.GetString());

	return true;
}

void Scene::OnCollision(Collider* c1, Collider* c2)
{
	gameScene->OnCollision(c1, c2);
}

void Scene::WillCollision(Collider* c1, Collider* c2)
{
	gameScene->WillCollision(c1, c2);
}


// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	gameScene->PostUpdate();

	

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}


bool Scene::LoadState(pugi::xml_node&)
{
	return true;
}

bool Scene::SaveState(pugi::xml_node&) const
{

	return true;
}



// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	levelList.clear();


	gameScene->CleanUp();
	delete gameScene;
	gameScene = nullptr;

	return true;
}
