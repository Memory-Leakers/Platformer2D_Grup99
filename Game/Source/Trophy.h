#pragma once
#include "Interactable.h"
#include "SDL/include/SDL.h"
#include "Point.h"
#include "Animation.h"
#include "Collider.h"

class Trophy : public Interactable
{
public:
	Trophy(int posX, int posY);

	~Trophy();

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;
private:

	Animation animPres;
};

