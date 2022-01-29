#include "GuiManager.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"



GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, int font, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L14: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* control = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, text, font);
			break;
		case GuiControlType::LABEL:
			control = new GuiLabel(id, bounds, text, font);
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckbox(id, bounds, text, font);
			break;
		case GuiControlType::SLIDER:
			control = new GuiSlider(id, bounds, text, font);
			break;
	// More Gui Controls can go here

	default:
		break;
	}

	//Set the observer

	//if (type != GuiControlType::LABEL)
	//{
		control->SetObserver(observer);
	//}
	//control->SetTexture(texture);

	// Created GuiControls are added to the list of controls
	if (control != nullptr) controls.add(control);

	return control;
}

bool GuiManager::Update(float dt)
{	
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt,doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::UpdateAll(float dt, bool doLogic) {

	if (doLogic) {

		ListItem<GuiControl*>* control = controls.start;

		while (control != nullptr)
		{
			control->data->Update(dt);
			control = control->next;
		}

	}
	return true; 

}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = controls.start;

	while (control != nullptr)
	{
		
		control->data->Draw(app->render);
		control = control->next;
		
	}

	return true;

}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = controls.start;

	while (control != NULL)
	{
		control->data->CleanUp();
		RELEASE(control->data);


		control = control->next;
	}
	RELEASE(control);

	return true;
}



