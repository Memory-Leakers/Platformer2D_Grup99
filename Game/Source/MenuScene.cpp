#include "MenuScene.h"
#include "App.h"


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


	gm.CreateGuiControl(GuiControlType::BUTTON, 0, "play", msFont, { 230,160, 80, 20 }, this);

	gm.CreateGuiControl(GuiControlType::BUTTON, 1, "", msFont, { 230,190, 80, 20 }, this);

	gm.CreateGuiControl(GuiControlType::BUTTON, 2, "", msFont, { 230,220, 80, 20 }, this);

	gm.CreateGuiControl(GuiControlType::CHECKBOX, 3, "fullscreen", msFont, { 10, 120, 20, 20 }, this);

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
		savefiled = true;
	}

	 rect = { 100,50,250,250 };


	gm.CreateGuiControl(GuiControlType::SLIDER, 3, "sound", msFont, { 10, 160, 20, 20 }, this);


	return true;
}

bool MenuScene::PreUpdate()
{
	gm.PreUpdate();

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	if (settingsopened && ((mouseX < rect.x) && (mouseX > (rect.x + rect.w)) &&
							(mouseY < rect.y) && (mouseY > (rect.y + rect.h))))
	{
	}
	else
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
		{
			settingsopened = false;
		}
	}


	return true;
}

bool MenuScene::Update(float dt)
{
	gm.Update(dt);

	return true;
}

bool MenuScene::PostUpdate()
{
	gm.Draw();

	app->font->BlitText(170, 50, titlefont2, "ninja froggy's super adventure");
	if (savefiled == true)
	{
		app->font->BlitText(238, 196, titlefont2, "continue");
	}

	app->font->BlitText(238, 226, titlefont2, "settings");

	if (settingsopened)
	{
		app->render->DrawRectangle(rect, 0, 125, 255, 80);
	}
	return true;
}

bool MenuScene::CleanUp()
{
	gm.CleanUp();
	//gm = GuiManager();

	app->font->UnLoad(msFont);
	app->font->UnLoad(titlefont2);

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
		if (control->id == 0)
		{
			std::cout << "Click on button 0" << std::endl;
			app->scene->changeScene(CScene::GAMESCENE);
		}

		if (control->id == 1)
		{
			std::cout << "Click on button 2" << std::endl;
			app->scene->changeScene(CScene::GAMESCENELOAD);

		}

		if (control->id == 2)
		{
			std::cout << "Click on button 3" << std::endl;

			settingsopened = true;
		}

	}
	//Other cases here

	default: break;
	}

	return true;
}
