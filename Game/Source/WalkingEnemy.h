#pragma once
#include "Enemy.h"

#define walkingPathRange 12

class WalkingEnemy :
    public Enemy
{
public:
	WalkingEnemy();

	~WalkingEnemy();

	bool Start() override;

	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called each loop iteration
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	iPoint GetCenterEnemyPos();
	

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

	Animation* Currentenemyanimation;

	pugi::xml_document WalkingEnemyfile;
	pugi::xml_node Walking_Enemy_node;

	SDL_Rect* EnemyRect;

	float jumpTime = 0.35f;
	float startJump = 0.0f;
	bool jumping = false;
};

