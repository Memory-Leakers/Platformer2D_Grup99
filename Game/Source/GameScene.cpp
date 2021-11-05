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

	coin = new Coin(227, 1429);
	coin->Start();



	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;

	if (coin != nullptr && coin->pendingToDelete)
	{
		delete coin;
		coin = nullptr;
	}
	froggy->PreUpdate();


	return ret;
}

bool GameScene::Update(float dt)
{
	bool ret = true;

	froggy->Update(dt);
	
	if (coin != nullptr)
	{
		coin->Update(dt);
	}
	

	//DEBUG
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

	if (coin != nullptr)
	{
		coin->PostUpdate();
	}
	

	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	froggy->CleanUp();
	delete froggy;
	froggy = nullptr;

	if (coin != nullptr)
	{
		delete coin;
		coin = nullptr;
	}
	

	return true;
}

void GameScene::OnCollision(Collider* c1, Collider* c2)
{
	
	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->OnCollision(c2);
	}

	if (coin != nullptr && coin->col == c1)
	{
		coin->OnCollision(c2);
	}
	/*
	for (int i = 0; i < MAX_POWERUPS; ++i)
	{
		if (powerUps[i] != nullptr && powerUps[i]->getCollider() == c1)
		{
			powerUps[i]->OnCollision(c2);
		}
	}
	*/

	ListItem<Collider*>* colItem;
	colItem = app->map->mapData.col.start;


	while (colItem != NULL)
	{
		// Whenever it collides with something
		if (colItem->data == c1)
		{

			
			//sceneObstacles[i]->OnCollision(c2);
			
		}
		colItem = colItem->next;
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
}

void GameScene::WillCollision(Collider* c1, Collider* c2)
{
	
	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->WillCollision();
	}
}
