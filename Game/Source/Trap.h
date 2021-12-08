#pragma once
#include "Enemy.h"
#include "SDL_image/include/SDL_image.h"

enum class TrapDirection
{
	UP = 0,
	DOWN,
	RIGHT,
	LEFT
};

class Trap :
    public Enemy
{
public:
    Trap(int x, int y, TrapDirection trapDirection = TrapDirection::UP);
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
	TrapDirection direction = TrapDirection::UP;
public:
	iPoint pos;
	SDL_Texture* tex;
	SDL_Rect bounds;
	SDL_Rect* rect;
	Collider* col;

};

