#include "Scene.h"
#include "time.h"



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



	// L03: DONE: Load map
	app->map->Load(levelList[0]->file.GetString());

	gameScene->Start();

	// Load music

	app->audio->PlayMusic("Assets/audio/music/papaya.ogg");


	bgSelector();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	gameScene->PreUpdate();
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	drawBackground();
	// Draw map
	app->map->Draw();

	gameScene->Update(app->globalTime.getDeltaTime());

    // LOAD AND SAVE
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
	}
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || gameScene->pendingtoReload)
	{
		gameScene->pendingtoReload = false;
		gameScene->ReloadLevel();
		bgSelector();
	}
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		gameScene->froggy->godMode = !gameScene->froggy->godMode;
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
	/*SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
				   app->map->mapData.width, app->map->mapData.height,
				   app->map->mapData.tileWidth, app->map->mapData.tileHeight,
				   app->map->mapData.tilesets.count());
	app->win->SetTitle(title.GetString());*/
	

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


bool Scene::LoadState(pugi::xml_node& data)
{
	//Player
	gameScene->froggy->position.x = data.child("player").attribute("posX").as_int();
	gameScene->froggy->position.y = data.child("player").attribute("posY").as_int();
	gameScene->froggy->playerScore = data.child("player").attribute("score").as_int();

	//Level/Map related
	bgTex = app->tex->Load(data.child("background").attribute("value").as_string());
	gameScene->key = data.child("startScene").attribute("doorKey").as_bool();

	return true;
}

bool Scene::SaveState(pugi::xml_node& data) const
{
	data.child("player").attribute("posX") = gameScene->froggy->position.x;
	data.child("player").attribute("posY") = gameScene->froggy->position.y;
	data.child("player").attribute("score") = gameScene->froggy->playerScore;

	//Level/Map related
	data.child("background").attribute("value") = bg.GetString();
	data.child("startScene").attribute("doorKey") = gameScene->key;

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


void Scene::drawBackground()
{
	for (int x = 0; x <= 1000; x += 64)
	{
		for (int y = bgPivY; y <= 6000; y += 64)
		{
			SDL_Rect r = SDL_Rect({ 0,0,64,64 });

			app->render->DrawTexture(bgTex, x, y, &r, 0.01f, SDL_FLIP_NONE, (0, 0));

		}
	}

	if (bgPivY >= -64)
	{
		bgPivY = -128;
	}
	else
	{
		bgPivX += 1;
		if (bgPivX != 2)
		{
			bgPivY += 1;
		}
		else bgPivX = 0;
	}


}

void Scene::bgSelector()
{

	if (bgTex != NULL)
	{
		SDL_DestroyTexture(bgTex);
	}

	int num;
	num = rand() % 7;

	switch (num)
	{
	case Blue:
		bg = "Assets/Background/Blue.png";
		break;
	case Brown:
		bg = "Assets/Background/Brown.png";
		break;
	case Gray:
		bg = "Assets/Background/Gray.png";
		break;
	case Green:
		bg = "Assets/Background/Green.png";
		break;
	case Pink:
		bg = "Assets/Background/Pink.png";
		break;
	case Purple:
		bg = "Assets/Background/Purple.png";
		break;
	case Yellow:
		bg = "Assets/Background/Yellow.png";
	}
	bgTex = app->tex->Load(bg.GetString());
}
