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

	ListItem<Coin*>* fruitItem;
	fruitItem = app->map->mapData.fruits.start;

	while (fruitItem != NULL)
	{
		if (fruitItem->data != nullptr)
		{
			fruitItem->data->Start();
		}
		fruitItem = fruitItem->next;
	}


	//Trophy
	trophy = new Trophy(2784, 1792);
	trophy->Start();

	//Clean
	delete fruitItem;
	fruitItem = nullptr;

	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;
	
	

	froggy->PreUpdate();

	//SET CAM ON FROGGY

	if (froggy != nullptr)
	{
		app->render->camera.x = (froggy->position.x *-2) + 540 - froggy->bounds.w;
		app->render->camera.y = (froggy->position.y *-2) + 260 - froggy->bounds.h;
	}

	return ret;
}

bool GameScene::Update(float dt)
{
	bool ret = true;

	froggy->Update(dt);

	ListItem<Coin*>* fruitItem;
	//fruitItem = fruitPool->start;
	fruitItem = app->map->mapData.fruits.start;

	while (fruitItem != NULL)
	{
		if (fruitItem->data != nullptr)
		{
			if (!fruitItem->data->pendingToDelete)
			{
				fruitItem->data->Update(dt);
			}
		}
		fruitItem = fruitItem->next;
	}

	//DEBUG
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{
		debugTiles = !debugTiles;
	}

	//Trophy
	trophy->Update(dt);

	//Clean
	delete fruitItem;
	fruitItem = nullptr;

	return ret;
}

bool GameScene::PostUpdate()
{
	bool ret = true;

	froggy->PostUpdate();

	ListItem<Coin*>* fruitItem;
	//fruitItem = fruitPool->start;
	fruitItem = app->map->mapData.fruits.start;

	while (fruitItem != NULL)
	{
		if (fruitItem->data != nullptr)
		{
			if (!fruitItem->data->pendingToDelete)
			{
				fruitItem->data->PostUpdate();
			}
		}
		fruitItem = fruitItem->next;
	}

	//Trophy
	trophy->PostUpdate();

	//Clean
	delete fruitItem;
	fruitItem = nullptr;

	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	froggy->CleanUp();
	delete froggy;
	froggy = nullptr;

	trophy->CleanUp();
	delete trophy;
	trophy = nullptr;

	//Coinpool cleanup is done in map.cpp

	return true;
}

bool GameScene::ReloadLevel()
{

	app->map->UnloadFruits();
	app->map->LoadFruits();
	ListItem<Coin*>* fruitItem;
	fruitItem = app->map->mapData.fruits.start;

	while (fruitItem != NULL)
	{
		if (fruitItem->data != nullptr)
		{
			fruitItem->data->Start();
		}
		fruitItem = fruitItem->next;
	}
	

	//froggy
	froggy->CleanUp();
	delete froggy;
	froggy = new Player();
	froggy->Start();

	return true;
}

void GameScene::OnCollision(Collider* c1, Collider* c2)
{

	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->OnCollision(c2);
	}

	ListItem<Coin*>* fruitItem;
	//fruitItem = fruitPool->start;
	fruitItem = app->map->mapData.fruits.start;

	while (fruitItem != NULL)
	{
		if (fruitItem->data != nullptr && fruitItem->data->col == c1)
		{
			fruitItem->data->OnCollision(c2);
		}
		fruitItem = fruitItem->next;
	}
	
	if (trophy != nullptr && trophy->col == c1)
	{
		trophy->OnCollision(c2);
	}

	/*
	for (int i = 0; i < MAX_ENEMY; ++i)
	{
		if (enemy[i] != nullptr && enemy[i]->getCollider() == c1)
		{
			enemy[i]->OnCollision(c2);
		}
	}
	*/
	//Clean
	delete fruitItem;
	fruitItem = nullptr;

}

void GameScene::WillCollision(Collider* c1, Collider* c2)
{

	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->WillCollision();
	}
}
