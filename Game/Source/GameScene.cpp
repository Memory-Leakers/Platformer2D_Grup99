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
	sceneStarted = true;

	//ENTITY MANAGER
	em.setPlayer(new Player());
	em.addEntity(new Trophy(2640, 752));
	em.addEntity(new Checkpoint(944, 144));
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

	//Settings
	gm.CreateGuiControl(GuiControlType::BUTTON, GameMenu::RESUME, "resume", titlefont, { 10, 50, 80, 20 }, this);
	gm.CreateGuiControl(GuiControlType::BUTTON, GameMenu::SETTINGS, "settings", titlefont, { 10, 80, 80, 20 }, this);
	gm.CreateGuiControl(GuiControlType::BUTTON, GameMenu::BACK, "back to title", titlefont, {10, 110, 80, 20}, this);
	gm.CreateGuiControl(GuiControlType::BUTTON, GameMenu::QUIT, "exit", titlefont, {10, 140, 80, 20}, this);

	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		pause = !pause;
	}

	if (pause)
	{
		gm.PreUpdate();
		return true;
	}
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

	if (pause)
	{
		gm.Update(dt);
		return true;
	}
	//Entities
	em.Update(dt);

	//GUI
	healthBar->Update();

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


	std::string s = std::to_string(em.getPlayer()->playerScore);
	std::string ss;
	for (int i = 5; i != s.size(); i--){
		ss += "0";
	}
	ss += s;
	char const* value = ss.c_str();

	app->font->BlitText(110, 20, titlefont, value);


	if (pause)
	{
		app->render->DrawRectangle({ -200, 0, app->win->screenSurface->w,  app->win->screenSurface->h }, 0, 0, 0, 100, true, false);
		gm.Draw();


	}


	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	//Menu
	gm.CleanUp();
	//gm = GuiManager();
	app->font->UnLoad(titlefont);

	//Entities
	em.CleanUp();
	//em = EntityManager();

	//GUI
	SDL_DestroyTexture(guiKey);
	guiKey = nullptr;

	healthBar->CleanUp();
	delete healthBar;
	healthBar = nullptr;

	sceneStarted = false;
	pause = false;



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

bool GameScene::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (control->id == GameMenu::RESUME)
		{
			pause = false;
		}

		if (control->id == GameMenu::SETTINGS)
		{

		}

		if (control->id == GameMenu::BACK)
		{
			app->scene->changeScene(CScene::MENUSCENE);
		}

		if (control->id == GameMenu::QUIT)
		{
			app->scene->exitPetition = true;
		}

	}
	//Other cases here

	default: break;
	}

	return true;
}