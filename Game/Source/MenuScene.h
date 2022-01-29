#pragma once
#include "GuiControl.h"
#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiLabel.h"
#include "GuiCheckbox.h"
#include "GuiSlider.h"

class MenuScene : public Module
{
public:
	MenuScene();

	virtual ~MenuScene();

	bool Awake();
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	GuiManager gm;
	GuiManager gmsettings;
	GuiButton* btn1;
	GuiButton* btn2;
	GuiButton* btn3;
	GuiButton* btn4;
	GuiCheckbox* cbx1;
	GuiCheckbox* cbx2;
	GuiSlider* sld1;
	GuiSlider* sld2;
	GuiLabel* lbl1;
	

	SDL_Rect rect;
	SDL_Rect rect2;

	int titlefont2;

	bool savefiled = false;

	bool settingsopened = false;

	bool creditsopened = false;

	bool sceneStarted = false;

	int msFont = -1;
};

