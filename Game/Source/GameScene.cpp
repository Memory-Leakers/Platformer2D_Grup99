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

	//ENTITY MANAGER
	em.setPlayer(new Player());
	em.addEntity(new Trophy(2640, 752));
	em.addEntity(new Checkpoint(1712, 352));
	em.addEntity(new DoorKey(1520, 1056));

	em.Start();

	//GUI
	guiKeyRect = new SDL_Rect({ 16, 0, 16, 16 });
	guiKey = app->tex->Load("Assets/Items/DoorKey.png");

	healthBar = new GUI("Assets/Menu/Ingame/healthBar.png", { 5, 5 }, 4, {0, 0, 58, 16 }, 16);
	healthBar->texScale = 1.65f;
	healthBar->frameOnX = false;
	healthBar->setFrameFollow(&em.getPlayer()->health);
	healthBar->Start();


	//SOUNDS
	pickupSFX = app->audio->LoadFx("Assets/audio/fx/item_pickup.wav");
	keypickupSFX = app->audio->LoadFx("Assets/audio/fx/key_pickup.wav");
	playerjumpSFX = app->audio->LoadFx("Assets/audio/fx/8bit_jump.wav");
	playerwalkSFX = app->audio->LoadFx("Assets/audio/fx/walk_barefoot.wav");

	//Fonts
	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	titlefont = app->font->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;

	em.PreUpdate();

	//SET CAM ON FROGGY
	if (em.getPlayer() != nullptr && app->input->GetKey(SDL_SCANCODE_C) != KEY_REPEAT)
	{
		int scale = app->win->GetScale();

		app->render->camera.x = (em.getPlayer()->position.x *  -scale) + app->render->camera.w / 2 - em.getPlayer()->bounds.w;
		app->render->camera.y = (em.getPlayer()->position.y * -scale) + app->render->camera.h / 2 - em.getPlayer()->bounds.h;
	}

	return ret;
}

bool GameScene::Update(float dt)
{
	bool ret = true;

	//Entities
	em.Update(dt);

	//GUI
	healthBar->Update();

	//DEBUG
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
	{ //Shows all the debug information
		debugTiles = !debugTiles;
	}
	return ret;
}

bool GameScene::PostUpdate()
{
	bool ret = true;

	em.PostUpdate();

	//GUI
	if (key)
	{
		app->render->DrawTexture(guiKey, em.getPlayer()->position.x + 3 , em.getPlayer()->position.y - 16, guiKeyRect);
	}
	healthBar->PostUpdate();

	app->font->BlitText(150, 248, titlefont, "hiiii");

	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");
	
	//Entities
	em.CleanUp();

	//GUI
	SDL_DestroyTexture(guiKey);
	guiKey = nullptr;

	healthBar->CleanUp();
	delete healthBar;
	healthBar = nullptr;

	app->font->UnLoad(titlefont);

	return true;
}

bool GameScene::ReloadLevel()
{
	//Cleaning
	em.CleanUp();
	key = false;
	checkPoint = false;

	//Re-Setting
	em.setPlayer(new Player());
	em.addEntity(new Trophy(2640, 752));
	em.addEntity(new Checkpoint(1712, 352));
	em.addEntity(new DoorKey(1520, 1056));
	app->map->LoadMapEntities();

	healthBar->setFrameFollow(&em.getPlayer()->health);

	//Starting
	em.Start();

	return true;
}

void GameScene::OnCollision(Collider* c1, Collider* c2)
{
	em.OnCollision(c1, c2);
}

void GameScene::WillCollision(Collider* c1, Collider* c2)
{
}


