#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__


#include "Log.h"
#include "Player.h"

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

	void OnCollision(Collider* c1, Collider* c2);

	void WillCollision(Collider* c1, Collider* c2);

public:
	bool debugTiles = false;

	Player* froggy = nullptr;
};

#endif // __GAMESCENE_H__
