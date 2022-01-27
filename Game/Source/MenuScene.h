#pragma once

#include "GuiManager.h"
#include "GuiButton.h"
#include "GuiLabel.h"
#include "GuiCheckbox.h"

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
	GuiButton* btn1;
	GuiLabel* lbl1;
	GuiCheckbox* cbx1;

	bool sceneStarted = false;

	int msFont = -1;
};

