#pragma once
#include "Pickable.h"
class DoorKey :
    public Pickable
{
public:
	DoorKey(int posX, int posY);

	~DoorKey();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* col) override;

	void WillCollision() override;

private:

	iPoint position;

	SDL_Rect bounds;

	SDL_Rect* rect;

	SDL_Texture* tex;

private:
	Animation anim;

	uint keypickupSFX;
};

