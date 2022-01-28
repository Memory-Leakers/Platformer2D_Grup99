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


	btn1 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 0, "play", msFont, { 230,160, 80, 20 }, this);

	btn2 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 1, "", msFont, { 230,190, 80, 20 }, this);

	btn3 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 2, "", msFont, { 230,220, 80, 20 }, this);

	cbx1 = (GuiCheckbox*)gm.CreateGuiControl(GuiControlType::CHECKBOX, 3, "fullscreen", msFont, { 10, 120, 20, 20 }, this);

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


sld1 = (GuiSlider*)gm.CreateGuiControl(GuiControlType::SLIDER, 3, "sound", msFont, { 10, 160, 20, 20 }, this);


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
	return true;
}

bool MenuScene::CleanUp()
{
	gm.CleanUp();
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
