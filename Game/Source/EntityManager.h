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

	void getOtherCheckpoint(Checkpoint* check = nullptr)
	{
		ListItem<Entity*>* item;
		item = Entities.start;

		while (item != nullptr)
		{
			if (item->data != nullptr)
			{
				if (item->data->id_type == 10)
				{
					if (check != nullptr)
					{
						if (item->data != check)
						{
							Checkpoint* aux = (Checkpoint*)item->data;
							if (aux->getCurrentState() == ACTIVATED)
							{
								froggy->position.x = item->data->position.x;
								froggy->position.y = item->data->position.y + 16;
							}
						}
					}
				}
			}
			item = item->next;
		}

		delete item;
		item = nullptr;
	}

	void setPlayer(Player* froggy)
	{
		this->froggy = froggy;
	}
};

