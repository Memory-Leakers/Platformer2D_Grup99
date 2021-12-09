#pragma once
#include "Enemy.h"
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

	void WillCollision() override;

	iPoint GetCenterEnemyPos();
	

private:

	iPoint pos;
	float speed;

	int cont = 0;

	int size;

	iPoint lastPlayerPos = { 0,0 };

	// Collider
	Collider* col = nullptr;


	SDL_Texture* enemytextures[4];

	Animation angrypigIdle;

	Animation angrypigRun;

	Animation angrypigWalk;

	Animation angrypigHit;

	Animation* Currentenemyanimation;

	pugi::xml_document WalkingEnemyfile;
	pugi::xml_node Walking_Enemy_node;

	SDL_Rect Enemybounds;

	SDL_Rect* EnemyRect;
};

