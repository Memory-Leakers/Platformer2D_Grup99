#pragma once
#include "Interactable.h"
#include "SDL_image/include/SDL_image.h"

class Trap : public Interactable
{
public:
    Trap(int x, int y, InteractablesId trapDirection = InteractablesId::SPIKETRAP_TOP);
    ~Trap();

	bool Start() override;

	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called each loop iteration
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;

public:
	int getHitPoints()
	{
		return hitPoints;
	}

private:
	int hitPoints = 1;
};

