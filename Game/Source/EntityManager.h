#pragma once
#include "Entity.h"
#include "Player.h"
#include "Coin.h"
#include "HealthPack.h"
#include "DoorKey.h"
#include "WalkingEnemy.h"
#include "FlyingEnemy.h"
#include "Checkpoint.h"
#include "Trophy.h"
#include "Trap.h"
#include "List.h"
#include "Collider.h"

class EntityManager
{
public:
	EntityManager();
	
	~EntityManager();

	bool Start();

	bool CleanUp();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	void OnCollision(Collider* c1, Collider* c2);
	void WillCollision();

	bool LoadState(pugi::xml_node& data);
	bool SaveState(pugi::xml_node& data) const;

	void addEntity(Entity* e);


private:
	List<Entity*> Entities;

	Player* froggy;

public:
	/*GETTERS AND SETTERS*/
	Player* getPlayer()
	{
		return froggy;
	}

	void setPlayer(Player* froggy)
	{
		this->froggy = froggy;
	}
};

