#include "GameScene.h"
#include "App.h"
#include "DynArray.h"
#include "External/SDL_mixer/include/SDL_mixer.h"

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

	cbx3 = (GuiCheckbox*)gmsetting.CreateGuiControl(GuiControlType::CHECKBOX, 3, "fullscreen", titlefont, { 200, 160, 20, 20 }, this);

	cbx4 = (GuiCheckbox*)gmsetting.CreateGuiControl(GuiControlType::CHECKBOX, 4, "vsync", titlefont, { 200, 180, 20, 20 }, this);

	sld5 = (GuiSlider*)gmsetting.CreateGuiControl(GuiControlType::SLIDER, 5, "music", titlefont, { 200, 100, 20, 20 }, this);

	sld6 = (GuiSlider*)gmsetting.CreateGuiControl(GuiControlType::SLIDER, 6, "fx", titlefont, { 200, 140, 20, 20 }, this);

	cbx3->active = app->win->fullscreen_window;
	cbx4->active = app->render->VSync;

	rec = { 150,10,250,250 };

	return ret;
}

bool GameScene::PreUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		pause = !pause;
	}
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	if (pause)
	{
		if (settingopen && ((mouseX > rec.x) && (mouseX < (rec.x + rec.w)) &&
			(mouseY > rec.y) && (mouseY < (rec.y + rec.h))))
		{
		}
		else
		{
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				settingopen = false;
			}
		}
		playTime.Reset();
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

	//Timer
	playTime.Update();
	if (playTime.getDeltaTime() >= 1)
	{
		timeSave += 1;
		playTime.Reset();
	}

	if (app->win->fullscreen_window)
	{
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetWindowSize(app->win->window, &app->render->camera.w, &app->render->camera.h);
	}
	else
	{
		SDL_SetWindowFullscreen(app->win->window, 0);
	}

	return ret;
}

bool GameScene::Update(float dt)
{
	bool ret = true;

	if (pause)
	{
		gm.Update(dt);
		gmsetting.Update(dt);
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
	std::string ss = "score; ";
	for (int i = 5; i != s.size(); i--){
		ss += "0";
	}
	ss += s;
	char const* score = ss.c_str();

	std::string hs = std::to_string(app->scene->highScoreI);
	std::string hss = "high score; ";
	for (int y = 5; y != hs.size(); y--) {
		hss += "0";
	}
	hss += hs;

	char const* highScore = hss.c_str();

	//Draw Score
	app->font->BlitText(110, 20, titlefont, score);

	//Draw High Score
	app->font->BlitText(110, 10, titlefont, highScore);

	//Timer
	int t = (int) timeSave;
	int hou = floor(t / 3600);
	int min = floor(t / 60);
	int sec = t - min * 60;

	std::string ts = std::to_string(hou);
	ts += ";";
	ts += std::to_string(min);
	ts += ";";
	ts += std::to_string(sec);

	app->font->BlitText(300, 10, titlefont, ts.c_str());

	if (pause)
	{
		app->render->DrawRectangle({ -200, 0, app->win->screenSurface->w,  app->win->screenSurface->h }, 0, 0, 0, 100, true, false);
		gm.Draw();

		if (settingopen)
		{
			app->render->DrawRectangle(rec, 0, 0, 110, 255, true, false);
			gmsetting.Draw();
		}
		
	}


	return ret;
}

bool GameScene::CleanUp()
{
	LOG("Freeing Game Scene");

	//Menu
	gm.CleanUp();
	gmsetting.CleanUp();
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
	playTime.Reset();
	//Cleaning
	em.CleanUp();
	key = false;
	checkPoint = false;

	//Re-Setting
	em.setPlayer(new Player());
	em.addEntity(new Trophy(2640, 752));
	em.addEntity(new Checkpoint(944, 144));
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
		if (!settingopen)
		{
			//Checks the GUI element ID
			if (control->id == GameMenu::RESUME)
			{
				pause = false;
			}

			if (control->id == GameMenu::SETTINGS)
			{
				std::cout << "Click on Settings button " << std::endl;

				settingopen = true;
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
	}
	//Other cases here
	case GuiControlType::CHECKBOX:
	{
		if (control->id == 3)
		{
			std::cout << "Click on button 3" << std::endl;
			if (!cbx3->active)
			{
				app->win->fullscreen_window = true;

			}
			else
			{
				app->win->fullscreen_window = false;


			}

		}

		if (control->id == 4)
		{
			std::cout << "Click on button 4" << std::endl;
			if (!cbx4->active)
			{
				app->render->VSync = true;

				LOG("Using vsync");


			}
			else
			{
				app->render->VSync = false;

				LOG("Not using vsync");
			}

		}
	}
	case GuiControlType::SLIDER:
	{
		if (control->id == 5)
		{
			std::cout << "Click on button 5" << std::endl;

			if (lastposX != sld5->sliderPos)
			{

				test = Mix_VolumeMusic(sld5->sliderPos - sld5->aux);

				std::cout << test << " previous volume" << std::endl;

				lastposX = sld5->sliderPos;
			}

		}

		if (control->id == 6)
		{
			std::cout << "Click on button 6" << std::endl;

			if (lastposX2 != sld6->sliderPos)
			{

				test = Mix_Volume(-1, sld6->sliderPos - sld6->aux);

				std::cout << test << " previous fx" << std::endl;

				lastposX2 = sld6->sliderPos;
			}

		}
	}

	default: break;
	}

	return true;
}
