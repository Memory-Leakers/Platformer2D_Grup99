#include "GameScene.h"
#include "App.h"

GameScene::GameScene()
{
	
}

GameScene::~GameScene()
{

}

bool GameScene::Awake()
{
	//LOG("Loading Game Scene");
	bool ret = true;

	return ret;
}

bool GameScene::Start()
{
	bool ret = true;

	froggy = new Player();
	froggy->Start();



	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool GameScene::Update(float dt)
{
	bool ret = true;

	froggy->Update(dt);

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		debugTiles = !debugTiles;
	}


	return ret;
}

bool GameScene::PostUpdate()
{
	bool ret = true;

	froggy->PostUpdate();

	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	froggy->CleanUp();
	delete froggy;
	froggy = nullptr;
	

	return true;
}
