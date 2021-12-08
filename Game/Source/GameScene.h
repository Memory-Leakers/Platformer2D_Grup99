#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "Log.h"
#include "List.h"
#include "Player.h"
#include "Coin.h"
#include "Trophy.h"
#include "DoorKey.h"
#include "Checkpoint.h"
#include "WalkingEnemy.h"

class GameScene
{
public:

	GameScene();

	// Destructor
	virtual ~GameScene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start() ;

	// Called before all Updates
	bool PreUpdate() ;

	// Called each loop iteration
	bool Update(float dt) ;

	// Called before all Updates
	bool PostUpdate() ;

	// Called before quitting
	bool CleanUp() ;

	bool ReloadLevel();

	void OnCollision(Collider* c1, Collider* c2);

	void WillCollision(Collider* c1, Collider* c2);

private:
	void gameUI();

public:
	bool debugTiles = false;

	Player* froggy = nullptr;

	WalkingEnemy* peppa = nullptr;

	Trophy* trophy = nullptr;

	Checkpoint* checkpoint = nullptr;

	DoorKey* doorKey = nullptr;
	
	Timer jumptimer;

	bool pendingtoReload = false;

	bool key = false;

	SDL_Rect* guiKeyRect = nullptr;
	SDL_Texture* guiKey = nullptr;
	SDL_Texture* pathTex;

	uint pickupSFX;
	uint keypickupSFX;
	uint playerjumpSFX;
	uint playerwalkSFX;

private:
	
	
};

#endif // __GAMESCENE_H__
