#pragma once

#include "Interactable.h"
#include "SDL/include/SDL.h"
#include "Point.h"
#include "Animation.h"
#include "Collider.h"
#include "PugiXml/src/pugixml.hpp"



enum CHECKSTATE
{
	BASE = 0,
	ACTIVATION,
	ACTIVATED
};

enum SAVEPETITION
{
	SAVEPETITION_NONE,
	SAVEPETITION_SAVE,
	SAVEPETITION_BLOCK
};

class Checkpoint : public Interactable
{
public:
	Checkpoint(int posX, int posY, CHECKSTATE state = BASE);

	~Checkpoint();

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;

	void loadData(pugi::xml_node node);

	void Save(pugi::xml_node& data) const override;

private:
	void loadAnimBase();
	void loadAnimActivation();
	void loadAnimActivated();
	
	void stateMachine() override;

private:
	Animation animBase;
	Animation animActivation;
	Animation animActivated;

	CHECKSTATE currentState = BASE;

	int savePetition = SAVEPETITION_NONE;

	SString dirAnimBase;
	SString dirAnimActivation;
	SString dirAnimActivated;

	SDL_Texture* btn_tex;
	Animation btn;
	SDL_Rect* btn_rect;

	bool btn_Switch = false;

public:

	CHECKSTATE getCurrentState()
	{
		return currentState;
	}

	void setCurrentState(int newState = true)
	{
		currentState = (CHECKSTATE) newState;
	}
};

