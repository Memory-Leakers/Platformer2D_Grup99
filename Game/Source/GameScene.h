#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__


#include "GuiManager.h"
#include "GuiButton.h"
#include "EntityManager.h"
#include "Log.h"
#include "Entity.h"
#include "Player.h"
#include "Coin.h"
#include "Trophy.h"
#include "DoorKey.h"
#include "Checkpoint.h"
#include "WalkingEnemy.h"
#include "FlyingEnemy.h"
#include "GUI.h"


enum GameMenu
{
	RESUME = 0,
	SETTINGS,
	BACK,
	QUIT
};

class GameScene : public Module
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

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	bool pause = false;

	GuiManager gm;

public:
	EntityManager em;

	Trophy* trophy = nullptr;

	Checkpoint* checkpoint = nullptr;

	DoorKey* doorKey = nullptr;

	Timer jumptimer;

	bool pendingtoReload = false;

	bool key = false;
	bool checkPoint = false;

	SDL_Rect* guiKeyRect = nullptr;
	SDL_Texture* guiKey = nullptr;
	SDL_Texture* pathTex;

	GUI* healthBar = nullptr;

	uint pickupSFX;
	uint keypickupSFX;
	uint playerjumpSFX;
	uint playerwalkSFX;

	Timer playTime;
	float timeSave;

	int titlefont = -1;

	bool sceneStarted = false;
};

#endif // __GAMESCENE_H__
