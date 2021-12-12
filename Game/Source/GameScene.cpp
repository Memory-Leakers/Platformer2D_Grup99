#include "GameScene.h"
#include "App.h"
#include "DynArray.h"

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
	//Enemy
	
	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{
		enemyItem->data->Start();

		enemyItem = enemyItem->next;
	}

	delete enemyItem;
	enemyItem = nullptr;
	
	 
	//enemies.add(new WalkingEnemy());
	//enemies.At(enemies.count() - 1)->data->Start();
	//peppa = new WalkingEnemy();
	//peppa->Start();
	
  //Fruit
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

	//Traps
	ListItem<Trap*>* trapItem;
	trapItem = app->map->mapData.trap.start;

	while (trapItem != NULL)
	{
		if (trapItem->data != nullptr)
		{
			trapItem->data->Start();
		}
		trapItem = trapItem->next;
	}

	//Trophy
	trophy = new Trophy(2640, 752);
	trophy->Start();

	//Checkpoint
	checkpoint = new Checkpoint(1712, 352);
	checkpoint->Start();

	//Key
	doorKey = new DoorKey(1520, 1056);
	doorKey->Start();


	//GUI
	guiKeyRect = new SDL_Rect({ 16, 0, 16, 16 });
	guiKey = app->tex->Load("Assets/Items/DoorKey.png");

	healthBar = new GUI("Assets/Menu/Ingame/healthBar.png", { 5, 5 }, 4, {0, 0, 58, 16 }, 16);
	healthBar->texScale = 1.65f;
	healthBar->frameOnX = false;
	healthBar->setFrameFollow(&froggy->health);
	healthBar->Start();


	//SOUNDS
	pickupSFX = app->audio->LoadFx("Assets/audio/fx/item_pickup.wav");
	keypickupSFX = app->audio->LoadFx("Assets/audio/fx/key_pickup.wav");
	playerjumpSFX = app->audio->LoadFx("Assets/audio/fx/8bit_jump.wav");
	playerwalkSFX = app->audio->LoadFx("Assets/audio/fx/walk_barefoot.wav");

	//Clean
	delete fruitItem;
	fruitItem = nullptr;

	delete trapItem;
	trapItem = nullptr;

	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;

	//Player (Froggy)
	froggy->PreUpdate();

	//Checkpoint
	checkpoint->PreUpdate();
	
	//ENEMIES
	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{
		if (enemyItem->data != nullptr)
		{
			if (enemyItem->data->death)
			{
				enemyItem->data->CleanUp();
				delete enemyItem->data;
				enemyItem->data = nullptr;
			}
			else
			{
				enemyItem->data->PreUpdate();
			}
		}

		enemyItem = enemyItem->next;
	}

	delete enemyItem;
	enemyItem = nullptr;

	//SET CAM ON FROGGY

	if (froggy != nullptr && app->input->GetKey(SDL_SCANCODE_C) != KEY_REPEAT)
	{
		int scale = app->win->GetScale();

		app->render->camera.x = (froggy->position.x *  -scale) + app->render->camera.w / 2 - froggy->bounds.w;
		app->render->camera.y = (froggy->position.y * -scale) + app->render->camera.h / 2 - froggy->bounds.h;
	}

	return ret;
}

bool GameScene::Update(float dt)
{
	bool ret = true;

	froggy->Update(dt);

	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{
		if (enemyItem->data != nullptr)
		{
			enemyItem->data->Update(dt);
		}
		enemyItem = enemyItem->next;
	}

	delete enemyItem;
	enemyItem = nullptr;
	/*if (peppa != nullptr)
	{
		peppa->Update(dt);
	}*/
	

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

	//Checkpoint
	checkpoint->Update(dt);

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

	//GUI
	healthBar->Update();


	//Clean
	delete fruitItem;
	fruitItem = nullptr;

	return ret;
}

bool GameScene::PostUpdate()
{
	bool ret = true;

	froggy->PostUpdate();

	//Fruits
	ListItem<Coin*>* fruitItem;
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

	//Traps
	ListItem<Trap*>* trapItem;
	trapItem = app->map->mapData.trap.start;

	while (trapItem != NULL)
	{
		if (trapItem->data != nullptr)
		{
			trapItem->data->PostUpdate();
		}
		trapItem = trapItem->next;
	}

	//Enemy
	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{
		if (enemyItem->data != nullptr)
		{
			enemyItem->data->PostUpdate();
		}
		enemyItem = enemyItem->next;
	}
	delete enemyItem;
	enemyItem = nullptr;

	/*if (peppa != nullptr)
	{
		peppa->PostUpdate();
	}*/
	//Trophy
	trophy->PostUpdate();

	//Checkpoint
	checkpoint->PostUpdate();

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

	healthBar->PostUpdate();

	//Clean
	delete fruitItem;
	fruitItem = nullptr;

	delete trapItem;
	trapItem = nullptr;

	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	//Player (Froggy)
	froggy->CleanUp();
	delete froggy;
	froggy = nullptr;

	//Trophy
	trophy->CleanUp();
	delete trophy;
	trophy = nullptr;

	//Checkpoint
	checkpoint->CleanUp();
	delete checkpoint;
	checkpoint = nullptr;

	if (doorKey != nullptr)
	{
		doorKey->CleanUp();
		delete doorKey;
		doorKey = nullptr;
	}

	//Coinpool cleanup is done in map.cpp
	//Trappool cleanup is done in map.cpp
	//Enemypool cleanup is done in map.cpp

	//GUI
	SDL_DestroyTexture(guiKey);
	guiKey = nullptr;

	return true;
}

bool GameScene::ReloadLevel()
{
	app->map->UnLoadMapObjects();
	app->map->LoadMapObjects();

	//Fruits
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
	delete fruitItem;
	fruitItem = nullptr;

	//Traps
	ListItem<Trap*>* trapItem;
	trapItem = app->map->mapData.trap.start;

	while (trapItem != NULL)
	{
		if (trapItem->data != nullptr)
		{
			trapItem->data->Start();
		}
		trapItem = trapItem->next;
	}
	delete trapItem;
	trapItem = nullptr;

	//Door key
	if (doorKey != nullptr)
	{
		doorKey->CleanUp();
		delete doorKey;
	}
	doorKey = new DoorKey(1520, 1056);
	doorKey->Start();
	key = false;

	//froggy
	froggy->CleanUp();
	delete froggy;
	froggy = new Player();
	froggy->Start();
	healthBar->setFrameFollow(&froggy->health);

	//ENEMIES

	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{
		if (enemyItem->data != nullptr)
		{
			enemyItem->data->Start();
		}
		enemyItem = enemyItem->next;
	}

	delete enemyItem;
	enemyItem = nullptr;

	//Checkpoint
	checkpoint->CleanUp();
	delete checkpoint;
	checkpoint = new Checkpoint(1712, 352);
	checkpoint->Start();

	return true;
}

void GameScene::OnCollision(Collider* c1, Collider* c2)
{
	//Fruits
	ListItem<Coin*>* fruitItem;
	fruitItem = app->map->mapData.fruits.start;

	while (fruitItem != NULL)
	{
		if (fruitItem->data != nullptr && fruitItem->data->col == c1)
		{
			fruitItem->data->OnCollision(c2);
			delete fruitItem->data;
			fruitItem->data = nullptr;
		}
		fruitItem = fruitItem->next;
	}

	//Player/ Froggy
	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->OnCollision(c2);
	}

	//Enemy
	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{

		if (enemyItem->data != nullptr && enemyItem->data->col == c1)
		{
			enemyItem->data->OnCollision(c2);
		}

		enemyItem = enemyItem->next;
	}

	delete enemyItem;
	enemyItem = nullptr;

	//Rest
	if (trophy != nullptr && trophy->col == c1)
	{
		trophy->OnCollision(c2);
	}

	if (checkpoint != nullptr && checkpoint->col == c2)
	{
		checkpoint->OnCollision(c1);
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
	//Player / Froggy
	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->WillCollision();
	}

	//Enemies
	ListItem<Enemy*>* enemyItem;
	enemyItem = app->map->mapData.enemies.start;

	while (enemyItem != NULL)
	{

		if (enemyItem->data != nullptr && enemyItem->data->col == c1)
		{
			enemyItem->data->WillCollision();
		}

		enemyItem = enemyItem->next;
	}

	delete enemyItem;
	enemyItem = nullptr;
}

