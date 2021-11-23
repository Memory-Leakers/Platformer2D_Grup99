#pragma once

#include "SDL/include/SDL.h"
#include "Point.h"
#include "Animation.h"
#include "Collider.h"



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

	Animation animBase;
	Animation animActivation;
	Animation animActivated;

public:

	SDL_Rect* rect;
	iPoint position;
	SDL_Rect bounds;
	Collider* col = nullptr;
	SDL_Texture* texBase;
	SDL_Texture* texActivation;
	SDL_Texture* texActivated;
	bool collided = false;

};

