#pragma once
#include "Enemy.h"

#define walkingPathRange 11

class WalkingEnemy :
    public Enemy
{
public:
	WalkingEnemy(int x, int y);

	~WalkingEnemy();

	bool Start() override;

	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called each loop iteration
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	iPoint GetCenterEnemyPos();

private:
	void stateMachine() override;

private:

	float speed = 2.0f;

	int cont = 0;

	int size;

	iPoint lastPlayerPos = { 0,0 };

	SDL_Texture* enemytextures[4];

	Animation angrypigIdle;

	Animation angrypigRun;

	Animation angrypigWalk;

	Animation angrypigHit;

	pugi::xml_document WalkingEnemyfile;
	pugi::xml_node Walking_Enemy_node;

	float jumpTime = 0.35f;
	float startJump = 0.0f;
	float jumpDelayTime = 1.65f; //Delay of jump after jump
	bool jumping = false;
};
