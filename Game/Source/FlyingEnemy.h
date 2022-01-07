#pragma once
#include "Enemy.h"

#define flyingPathRange 16

class FlyingEnemy :
    public Enemy
{
public:
	FlyingEnemy(int x,int y);

	~FlyingEnemy();

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

	float speed = 1.0f;

	int cont = 0;

	int size;

	bool playerflip;

	int playerposX;

	iPoint lastPlayerPos = { 0,0 };

	SDL_Texture* enemytextures[4];

	Animation ghostIdle;

	Animation ghostHit;

	Animation ghostDisappear;

	Animation ghostAppear;

	pugi::xml_document FlyingEnemyfile;
	pugi::xml_node Flying_Enemy_node;
};

