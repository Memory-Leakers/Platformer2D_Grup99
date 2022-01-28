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


	btn1 = (GuiButton*)gm.CreateGuiControl(GuiControlType::BUTTON, 0, "play", msFont, { 10, 10, 80, 20 }, this);

	lbl1 = (GuiLabel*)gm.CreateGuiControl(GuiControlType::LABEL, 1, "bon dia", msFont, { 10, 10, 80, 80 }, this);
	
	cbx1 = (GuiCheckbox*)gm.CreateGuiControl(GuiControlType::CHECKBOX, 2, "musica", msFont, { 10, 120, 20, 20 }, this);

	sld1 = (GuiSlider*)gm.CreateGuiControl(GuiControlType::SLIDER, 3, "sound", msFont, { 10, 160, 20, 20 }, this);

	return true;
}

bool MenuScene::PreUpdate()
{
	gm.PreUpdate();
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

	sceneStarted = false;
	return true;
}

bool MenuScene::OnGuiMouseClickEvent(GuiControl* control)
{
	///
	//PASSA 2 COPS why?
	//
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
			std::cout << "Click on button 1" << std::endl;
		}

	}
	//Other cases here

	default: break;
	}

	return true;
}