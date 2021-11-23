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

	//Key
	doorKey = new DoorKey(1664, 2096);
	doorKey->Start();


	//GUI
	guiKeyRect = new SDL_Rect({ 16, 0, 16, 16 });
	guiKey = app->tex->Load("Assets/Items/DoorKey.png");


	//SOUNDS
	pickupSFX = app->audio->LoadFx("Assets/audio/fx/item_pickup.wav");
	keypickupSFX = app->audio->LoadFx("Assets/audio/fx/key_pickup.wav");
	playerjumpSFX = app->audio->LoadFx("Assets/audio/fx/8bit_jump.wav");
	playerwalkSFX = app->audio->LoadFx("Assets/audio/fx/walk_barefoot.wav");

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

	if (froggy != nullptr && app->input->GetKey(SDL_SCANCODE_C) != KEY_REPEAT)
	{
		app->render->camera.x = (froggy->position.x *-2) + 540 - froggy->bounds.w;
		app->render->camera.y = (froggy->position.y *-2) + 260 - froggy->bounds.h;
	}

	//if(key) std::cout << "key->" << "true" << std::endl;

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

	//Key
	if (key && doorKey != nullptr)
	{
		doorKey->CleanUp();
		delete doorKey;
		doorKey = nullptr;
	}
	else if (!key && doorKey != nullptr)
	{
		doorKey->Update(dt);
	}	

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

	//Key
	if (!key && doorKey != nullptr)
	{
		doorKey->PostUpdate();
	}
	


	//GUI
	if (key)
	{
		app->render->DrawTexture(guiKey, froggy->position.x + 3 , froggy->position.y - 16, guiKeyRect);
	}

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

	if (doorKey != nullptr)
	{
		doorKey->CleanUp();
		delete doorKey;
		doorKey = nullptr;
	}

	//Coinpool cleanup is done in map.cpp
	
	//GUI
	SDL_DestroyTexture(guiKey);
	guiKey = nullptr;

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
	
	if (doorKey != nullptr)
	{
		doorKey->CleanUp();
		delete doorKey;
	}
	doorKey = new DoorKey(1664, 2096);
	doorKey->Start();
	key = false;

	//froggy
	froggy->CleanUp();
	delete froggy;
	froggy = new Player();
	froggy->Start();

	return true;
}

void GameScene::OnCollision(Collider* c1, Collider* c2)
{
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

	if (doorKey != nullptr && doorKey->col == c1)
	{
		doorKey->OnCollision(c2);
	}

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
