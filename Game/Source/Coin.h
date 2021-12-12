#pragma once
#include "Pickable.h"


#define fruitPoints 100
enum FruitType {
	Apple = 0,
	Bananas,
	Cherries,
	Kiwi,
	Melon,
	Orange,
	Pineapple,
	Strawberry
};

class Coin :public Pickable
{
public:
	Coin(int posX, int posY);

	~Coin();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* col) override;

	void WillCollision() override;

	void textureSelector();

public:

	SDL_Rect bounds;

	SDL_Rect* rect;

	SDL_Texture* tex;

private:
	Animation anim;

};

