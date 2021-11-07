#pragma once
#include "SDL/include/SDL.h"
#include "Point.h"
#include "Animation.h"
#include "Collider.h"

class Trophy
{
public:
	Trophy(int posX, int posY);

	~Trophy();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* col);

	void WillCollision();
private:

	Animation animPres;

public:

	SDL_Rect* rect;

	iPoint position;

	SDL_Rect bounds;

	Collider* col = nullptr;

	SDL_Texture* tex;

	bool collided = false;
};

