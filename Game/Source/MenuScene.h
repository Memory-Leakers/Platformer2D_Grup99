#pragma once

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

	SDL_Rect rect;

	int titlefont2;

	bool savefiled = false;

	bool settingsopened = false;

	bool sceneStarted = false;

	int msFont = -1;
};

