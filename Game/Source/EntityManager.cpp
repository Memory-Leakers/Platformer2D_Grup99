#include "EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{

}

bool EntityManager::Start()
{
	froggy->Start();

	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != nullptr)
	{
		if (item->data != nullptr)
		{
			item->data->Start();
		}
		item = item->next;
	}

	delete item;
	item = nullptr;

	return true;
}

bool EntityManager::CleanUp()
{
	if (froggy != nullptr)
	{
		froggy->CleanUp();
		delete froggy;
		froggy = nullptr;
	}
	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != nullptr)
	{
		if (item->data != nullptr)
		{
			item->data->CleanUp();
			delete item->data;
			item->data = nullptr;
		}
		item = item->next;
	}

	delete item;
	item = nullptr;

	return true;
}

bool EntityManager::PreUpdate()
{
	froggy->PreUpdate();

	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != nullptr)
	{
		if (item->data != nullptr)
		{
			if (item->data->dead)
			{
				item->data->CleanUp();
				delete item->data;
				item->data = nullptr;

				item->next;
				continue;
			}

			item->data->PreUpdate();
		}
		item = item->next;
	}

	delete item;
	item = nullptr;

	return true;
}

bool EntityManager::Update(float dt)
{
	froggy->Update(dt);

	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != nullptr)
	{
		if (item->data != nullptr)
		{
			item->data->Update(dt);
		}
		item = item->next;
	}

	delete item;
	item = nullptr;

	return true;
}

bool EntityManager::PostUpdate()
{
	froggy->PostUpdate();

	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != nullptr)
	{
		if (item->data != nullptr)
		{
			item->data->PostUpdate();
		}
		item = item->next;
	}

	delete item;
	item = nullptr;

	return true;
}

void EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	if (froggy != nullptr && froggy->col == c1)
	{
		froggy->OnCollision(c2);
	}

	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != nullptr)
	{
		if (item->data != nullptr && item->data->col == c1)
		{
			item->data->OnCollision(c2);
		}

		item = item->next;
	}

	delete item;
	item = nullptr;
}

void EntityManager::WillCollision()
{

}

bool EntityManager::LoadState(pugi::xml_node& data)
{
	/*CLEAR ENTITIES*/
	CleanUp();

	froggy = new Player();

	pugi::xml_node p = data.child("pickables").first_child();
	pugi::xml_node e = data.child("enemies").first_child();
	pugi::xml_node i = data.child("interactables").first_child();

	//LOAD PICKABLES
	while (p != NULL)
	{
		int posX = p.attribute("posX").as_int();
		int posY = p.attribute("posY").as_int();

		switch ((PickID) p.attribute("id").as_int())
		{
		case PickID::FRUIT://COIN
			Entities.add(new Coin(posX, posY));
			break;
		case PickID::HEARTH://HEALTHPACK
			Entities.add(new HealthPack(posX, posY));
			break;
		case PickID::KEY://KEY
			Entities.add(new DoorKey(posX, posY));
		}

		p = p.next_sibling();
	}

	//LOAD ENEMIES
	while (e != NULL)
	{
		int posX = e.attribute("posX").as_int();
		int posY = e.attribute("posY").as_int();

		switch ((EnemyId) e.attribute("id").as_int())
		{
		case EnemyId::WALKING:
			Entities.add(new WalkingEnemy(posX, posY));
			break;
		case EnemyId::FLYING:
			Entities.add(new FlyingEnemy(posX, posY));
			break;
		}

		e = e.next_sibling();
	}

	//LOAD INTERACTABLES
	while (i != NULL)
	{
		int posX = i.attribute("posX").as_int();
		int posY = i.attribute("posY").as_int();

		switch ((InteractablesId)i.attribute("id").as_int())
		{
		case InteractablesId::CHECKPOINT:
			Entities.add(new Checkpoint(posX, posY,(CHECKSTATE) i.attribute("state").as_int()));
			break;
		case InteractablesId::TROPHY:
			Entities.add(new Trophy(posX, posY));
			break;
		case InteractablesId::SPIKETRAP_TOP:
			Entities.add(new Trap(posX, posY, InteractablesId::SPIKETRAP_TOP));
			break;
		case InteractablesId::SPIKETRAP_BOTTOM:
			Entities.add(new Trap(posX, posY, InteractablesId::SPIKETRAP_BOTTOM));
			break;
		case InteractablesId::SPIKETRAP_LEFT:
			Entities.add(new Trap(posX, posY, InteractablesId::SPIKETRAP_LEFT));
			break;
		case InteractablesId::SPIKETRAP_RIGHT:
			Entities.add(new Trap(posX, posY, InteractablesId::SPIKETRAP_RIGHT));
			break;
		}

		i = i.next_sibling();
	}

	Start();

	return true;
}

bool EntityManager::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node& pickables = data.child("pickables");
	pugi::xml_node& enemies = data.child("enemies");
	pugi::xml_node& interactables = data.child("interactables");

	//REMOVING OLD SAVE DATA
	while (pickables.child("p"))
	{
		pickables.remove_child("p");
	}
	while (enemies.child("e"))
	{
		enemies.remove_child("e");
	}
	while (interactables.child("i"))
	{
		interactables.remove_child("i");
	}

	//WRITING NEW SAVE DATA
	ListItem<Entity*>* item;
	item = Entities.start;

	while (item != NULL)
	{
		if (item->data != nullptr)
		{
			if (!item->data->dead)
			{
				item->data->Save(data);
			}
		}
		item = item->next;
	}

	delete item;
	item = nullptr;

	return true;
}

//-----------------------------------

void EntityManager::addEntity(Entity* e)
{
	Entities.add(e);
}