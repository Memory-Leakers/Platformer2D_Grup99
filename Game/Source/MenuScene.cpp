#include "MenuScene.h"
#include "App.h"
#include "External/SDL_mixer/include/SDL_mixer.h"


MenuScene::MenuScene()
{

}

MenuScene::~MenuScene()
{

}

bool MenuScene::Awake()
{


	return true;
}

bool MenuScene::Start()
{
	sceneStarted = true;

	msFont = app->font->Load("Assets/Fonts/rtype_font3.png", { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" }, 2);


	btn1 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 0, "play", msFont, { 20,100, 80, 20 }, this);

	btn2 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 1, "continue", msFont, { 20,130, 80, 20 }, this);

	btn3 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 2, "settings", msFont, { 20,160, 80, 20 }, this);

	btn4 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 7, "credits", msFont, { 20,190, 80, 20 }, this);

	btn5 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 8, "exit", msFont, { 20, 220, 80, 20 }, this);

	cbx1 = (GuiCheckbox*)gmsettings.CreateGuiControl(GuiControlType::CHECKBOX, 3, "fullscreen", msFont, { 210, 160, 20, 20 }, this);

	cbx2 = (GuiCheckbox*)gmsettings.CreateGuiControl(GuiControlType::CHECKBOX, 4, "vsync", msFont, { 210, 180, 20, 20 }, this);

	sld1 = (GuiSlider*)gmsettings.CreateGuiControl(GuiControlType::SLIDER, 5, "music", msFont, { 210, 100, 20, 20 }, this);

	sld2 = (GuiSlider*)gmsettings.CreateGuiControl(GuiControlType::SLIDER, 6, "fx", msFont, { 210, 140, 20, 20 }, this);

	char lookupTable[] = { "! @,_./0123456789$;< ?abcdefghijklmnopqrstuvwxyz" };
	titlefont2 = app->font->Load("Assets/Fonts/rtype_font3.png", lookupTable, 2);

	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(SAVE_STATE_FILENAME);

	if (result == NULL)
	{
		std::cout << " No se ha encontrado un archivo de partida guardada" << endl;
		savefiled = false;
	}
	else
	{
		std::cout << "Se ha encontrado un archivo de partida guardada" << endl;
		if (doc.child("game_state").child("scene").child("raw").attribute("value").as_bool())
		{
			savefiled = false;
		}
		else
		{
			savefiled = true;
		}
	}

	 rect = { 150,10,250,250 };
	 rect2 = { 200,67,150,150 };

	 Mix_VolumeMusic(0);

	 Mix_Volume(-1, 0);

	return true;
}

bool MenuScene::PreUpdate()
{

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	if (settingsopened && ((mouseX > rect.x) && (mouseX < (rect.x + rect.w)) &&
							(mouseY > rect.y) && (mouseY < (rect.y + rect.h))))
	{
	}
	else
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
		{
			settingsopened = false;
		}
	}

	if (creditsopened && ((mouseX > rect2.x) && (mouseX < (rect2.x + rect2.w)) &&
		(mouseY > rect2.y) && (mouseY < (rect2.y + rect2.h))))
	{
	}
	else
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
		{
			creditsopened = false;
		}
	}

	if (cbx1->active)
	{
		ListItem<GuiControl*>* control = app->guiManager->controls.start;
		int scale = app->win->GetScale();
		while (control != nullptr)
		{
			//HEREHERE
			control->data->bounds.x = control->data->bounds.x + app->render->camera.w / 2;
			control->data->bounds.y = control->data->bounds.y + app->render->camera.h / 2;
			control = control->next;

		}

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


	return true;
}

bool MenuScene::Update(float dt)
{
	if (savefiled == true)
	{
		btn2->disable = false;
	}
	else
	{
		btn2->disable = true;
	}

	gm.Update(dt);
	gmsettings.Update(dt);

	return true;
}

bool MenuScene::PostUpdate()
{
	gm.Draw();

	app->font->BlitText(20, 50, titlefont2, "ninja froggy's super adventure");

	if (creditsopened)
	{
		app->render->DrawRectangle(rect2, 60, 56, 89, 255, true, false);

		app->font->BlitText(228, 76, titlefont2, "authors");
		app->font->BlitText(218, 96, titlefont2, "pol rius");
		app->font->BlitText(218, 116, titlefont2, "robert recorda");

		app->font->BlitText(228, 146, titlefont2, "license");
		app->font->BlitText(218, 166, titlefont2, "mit license");

	}

	if (settingsopened)
	{
		app->render->DrawRectangle(rect, 60, 56, 89, 255, true, false);
		gmsettings.Draw();
	}



	return true;
}

bool MenuScene::CleanUp()
{
	gm.CleanUp();
	//gm = GuiManager();

	app->font->UnLoad(msFont);
	app->font->UnLoad(titlefont2);

	gmsettings.CleanUp();

	sceneStarted = false;
	return true;
}

bool MenuScene::OnGuiMouseClickEvent(GuiControl* control)
{

	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		//Checks the GUI element ID
		if (!settingsopened && !creditsopened)
		{
			if (control->id == 0)
			{
				std::cout << "Click on button 0" << std::endl;
				app->scene->changeScene(CScene::GAMESCENE);
			}

			if (control->id == 1)
			{
				std::cout << "Click on button 1" << std::endl;
				app->scene->changeScene(CScene::GAMESCENELOAD);

			}

			if (control->id == 2)
			{
				std::cout << "Click on button 2" << std::endl;

				settingsopened = true;
			}

			if (control->id == 7)
			{
				std::cout << "Click on credits button" << std::endl;

				creditsopened = true;
			}

			if (control->id == 8)
			{
				std::cout << "Click on exit button" << std::endl;

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
			if (!cbx1->active)
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
			if (!cbx2->active)
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

			if (lastposX != sld1->sliderPos)
			{

				test = Mix_VolumeMusic(sld1->sliderPos - sld1->aux);

				std::cout << test << " previous volume"<<std::endl;

				lastposX = sld1->sliderPos;
			}

		}

		if (control->id == 6)
		{
			std::cout << "Click on button 6" << std::endl;

			if (lastposX2 != sld2->sliderPos)
			{

				test = Mix_Volume(-1,sld2->sliderPos - sld2->aux);

				std::cout << test << " previous fx" << std::endl;

				lastposX2 = sld1->sliderPos;
			}

		}
	}

	default: break;
	}

	return true;
}
