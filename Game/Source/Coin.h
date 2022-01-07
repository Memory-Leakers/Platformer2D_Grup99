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

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;

	void textureSelector();
};

