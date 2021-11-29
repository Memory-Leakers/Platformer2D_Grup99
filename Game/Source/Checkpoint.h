#pragma once

#include "SDL/include/SDL.h"
#include "Point.h"
#include "Animation.h"
#include "Collider.h"


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

class Checkpoint
{
public:
	Checkpoint(int posX, int posY);

	~Checkpoint();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* col);

	void WillCollision();

private:
	void loadAnimBase();
	void loadAnimActivation();
	void loadAnimActivated();
private:

	Animation animBase;
	Animation animActivation;
	Animation animActivated;

	CHECKSTATE currentState = BASE;
	void stateMachine();
	bool stateChanged = true;

	int savePetition = SAVEPETITION_NONE;

public:

	SDL_Rect* rect;
	iPoint position;
	SDL_Rect bounds;
	Collider* col = nullptr;
	SDL_Texture* tex;
	//SDL_Texture* texBase;
	//SDL_Texture* texActivation;
	//SDL_Texture* texActivated;
	bool collided = false;

public:

	int getCurrentState()
	{
		return currentState;
	}

	void setCurrentState(int newState)
	{
		currentState = (CHECKSTATE) newState;
	}

	void setStateChanged(bool value)
	{
		stateChanged = value;
	}


};

