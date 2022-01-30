#include "Scene.h"
#include "time.h"



Scene::Scene() : Module()
{
	name.Create("scene");
	menuScene = new MenuScene();
	gameScene = new GameScene();
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");

	highScoreI = config.child("highscore").attribute("value").as_int();

	bool ret = true;
	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//Scene managemenet
	switch (cScene)
	{
	case CScene::MENUSCENE:
		//app->audio->StopMusic();
		delete menuScene;
		menuScene = nullptr;
		menuScene = new MenuScene();
		menuScene->Start();
		break;
	case CScene::GAMESCENE:
	case CScene::GAMESCENELOAD:

		delete gameScene;
		gameScene = nullptr;
		gameScene = new GameScene();
		if (!app->map->generated)
		{
			levelList.add(new Level(1, "Level1.tmx"));
			levelList[0]->camX = 209;
			levelList[0]->camY = -2401;
			//Generates map
			if (app->map->Load(levelList[0]->file.GetString()) == true)
			{
				int w, h;
				uchar* data = NULL;

				if (app->map->CreateWalkabilityMap(w, h, &data)) app->SetMap(w, h, data);
				app->map->generated = true;
				RELEASE_ARRAY(data);
			}
		}

		gameScene->Start();

		// Load music
		app->audio->PlayMusic("Assets/audio/music/papaya.ogg");
		break;
	}

	bgSelector();
	if (buttonSFX == NULL)
	{
		buttonSFX = app->audio->LoadFx("Assets/audio/fx/button.wav");
	}

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	switch (cScene)
	{
		case CScene::MENUSCENE:

			if (!menuScene->sceneStarted) Start();

			menuScene->PreUpdate();
			break;
		case CScene::GAMESCENE:

			if (!gameScene->sceneStarted)
			{
				Start();
				gameScene->ReloadLevel();

			}
			gameScene->PreUpdate();
			break;

		case CScene::GAMESCENELOAD:

			if (!gameScene->sceneStarted)
			{
				Start();
				app->LoadGameRequest();
			}
			gameScene->PreUpdate();
			break;
	}

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{

	//Scene management
	switch (cScene)
	{
	case CScene::MENUSCENE:
		drawBackground();
		menuScene->Update(app->gameTime.getDeltaTime());
		break;
	case CScene::GAMESCENE:
	case CScene::GAMESCENELOAD:
		drawBackground();
		// Draw map
		app->map->Draw();
		gameScene->Update(app->gameTime.getDeltaTime());

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

		//GOD MODE ON/OFF
		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			gameScene->em.getPlayer()->godMode = !gameScene->em.getPlayer()->godMode;
		}
	break;
	}
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		if (app->FPS == 60)
		{
			app->FPS = 30;
		}
		else if (app->FPS == 30)
		{
			app->FPS = 60;
		}
		else
		{
			app->FPS = 60;
		}
	}

	//DEBUG
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{ //Shows all the debug information
		debugTiles = !debugTiles;
	}
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

	//Scene management
	switch (cScene)
	{
	case CScene::MENUSCENE:
		menuScene->PostUpdate();
		break;
	case CScene::GAMESCENE:
	case CScene::GAMESCENELOAD:
		gameScene->PostUpdate();
		break;
	}

	//ChangesScene if there is a petition
	changeScene();

	ret = !exitPetition;

	return ret;
}


bool Scene::LoadState(pugi::xml_node& data)
{
	gameScene->em.LoadState(data);

	//Player
	gameScene->em.getPlayer()->position.x = data.child("player").attribute("posX").as_int();
	gameScene->em.getPlayer()->position.y = data.child("player").attribute("posY").as_int();
	gameScene->em.getPlayer()->playerScore = data.child("player").attribute("score").as_int();
	gameScene->em.getPlayer()->health = data.child("player").attribute("health").as_int();
	gameScene->healthBar->setFrameFollow(&gameScene->em.getPlayer()->health);

	//TIMER
	gameScene->timeSave = data.child("playtime").attribute("value").as_float();

	//Level/Map related
	bgTex = app->tex->Load(data.child("background").attribute("value").as_string());
	gameScene->key = data.child("startScene").attribute("doorKey").as_bool(); //

	return true;
}

bool Scene::SaveState(pugi::xml_node& data) const
{
	data.child("player").attribute("posX") = gameScene->em.getPlayer()->position.x;
	data.child("player").attribute("posY") = gameScene->em.getPlayer()->position.y;
	data.child("player").attribute("score") = gameScene->em.getPlayer()->playerScore;
	data.child("player").attribute("health") = gameScene->em.getPlayer()->health;
	data.child("player").attribute("high_score") = highScoreI;

	data.child("raw").attribute("value") = false;

	//TIMER
	data.child("playtime").attribute("value") = gameScene->timeSave;

	//Level/Map related
	data.child("background").attribute("value") = bg.GetString();
	data.child("startScene").attribute("doorKey") = gameScene->key; //

	gameScene->em.SaveState(data);

	return true;
}


// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	if (levelList.count() != 0)
	{
		levelList.At(0)->data->file.Clear();
		levelList.clear();
	}

	if (gameScene->sceneStarted)
	{
		gameScene->CleanUp();
	}
	delete gameScene;
	gameScene = nullptr;

	if (menuScene->sceneStarted)
	{
		menuScene->CleanUp();
	}
	delete menuScene;
	menuScene = nullptr;

	bg.~SString();

	return true;
}

bool Scene::SaveSettings(pugi::xml_node& config)
{
	config.child("highscore").attribute("value") = highScoreI;

	return true;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == 1)
		{
			LOG("Click on button 1");
		}

		if (control->id == 2)
		{
			LOG("Click on button 2");
		}

	}
	//Other cases here

	default: break;
	}

	return true;
}

void Scene::drawBackground()
{
	for (int x = -64; x <= 768; x += 64)
	{
		for (int y = bgPivY; y <= 448; y += 64)
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

void Scene::changeScene(CScene scene)
{

	if (!changeScenePetition && scene != CScene::NONE)
	{
		changeScenePetition = true;
		nextScene = scene;
	}
	else if(changeScenePetition && scene == CScene::NONE)
	{
		switch (cScene)
		{
		case CScene::MENUSCENE:
			menuScene->CleanUp();
			break;
		case CScene::GAMESCENE:
		case CScene::GAMESCENELOAD:
			gameScene->CleanUp();
			break;
		}

		cScene = nextScene;
		changeScenePetition = false;
	}
}
