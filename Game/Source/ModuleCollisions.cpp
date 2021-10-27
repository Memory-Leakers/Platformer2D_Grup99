#include "ModuleCollisions.h"

#include "App.h"
#include "Log.h"
#include "Render.h"
#include "Input.h"

#include "External/SDL/include/SDL_Scancode.h"

ModuleCollisions::ModuleCollisions()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		colliders[i] = nullptr;
	}

	//TRUE -> Can be detected BY FIRST OBJECT // FALSE -> CAN'T BE DETECTED BY FIRST OBJECT
	// EX-> matrix[uint(Type::WALL)][uint(Type::PLAYER)] = false;
	// WALL CAN DETECT PLAYER

	//WALL
	matrix[uint(Type::WALL)][uint(Type::WALL)] = false;
	matrix[uint(Type::WALL)][uint(Type::PLAYER)] = false;
	matrix[uint(Type::WALL)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::WALL)][uint(Type::PICKER)] = false;
	matrix[uint(Type::WALL)][uint(Type::AREA)] = false;

	//Player
	matrix[uint(Type::PLAYER)][uint(Type::WALL)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::PLAYER)] = false;
	matrix[uint(Type::PLAYER)][uint(Type::ENEMY)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::PICKER)] = true;
	matrix[uint(Type::PLAYER)][uint(Type::AREA)] = true;

	//ENEMY
	matrix[uint(Type::ENEMY)][uint(Type::WALL)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::ENEMY)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::PICKER)] = false;
	matrix[uint(Type::ENEMY)][uint(Type::AREA)] = false;

	//PICKER
	matrix[uint(Type::PICKER)][uint(Type::WALL)] = false;
	matrix[uint(Type::PICKER)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::PICKER)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::PICKER)][uint(Type::PICKER)] = false;
	matrix[uint(Type::PICKER)][uint(Type::AREA)] = false;

	//Area
	matrix[uint(Type::PICKER)][uint(Type::WALL)] = false;
	matrix[uint(Type::PICKER)][uint(Type::PLAYER)] = true;
	matrix[uint(Type::PICKER)][uint(Type::ENEMY)] = false;
	matrix[uint(Type::PICKER)][uint(Type::PICKER)] = false;
	matrix[uint(Type::PICKER)][uint(Type::AREA)] = false;

	
}

// Destructor
ModuleCollisions::~ModuleCollisions()
{

}

bool ModuleCollisions::Awake(pugi::xml_node& config)
{
	return true;
}



bool ModuleCollisions::Start()
{
	return true;
}

bool ModuleCollisions::PreUpdate()
{
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->pendingToDelete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	Collider* c1 = NULL;
	Collider* c2 = NULL;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// Skip empty colliders
		if (colliders[i] == nullptr)
		{
			continue;
		}

		c1 = colliders[i];

		// Avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// Skip empty colliders
			if (colliders[k] == nullptr)
			{
				continue;
			}

			c2 = colliders[k];

			if (c1->WillIntersects(c2->rect))
			{
				if (matrix[uint(c1->type)][uint(c2->type)] && c1->listener)
					c1->listener->WillCollision(c1, c2);

				if (matrix[uint(c2->type)][uint(c1->type)] && c2->listener)
					c2->listener->WillCollision(c2, c1);
			}

			if (c1->Intersects(c2->rect))
			{
				if (matrix[uint(c1->type)][uint(c2->type)] && c1->listener)
					c1->listener->OnCollision(c1, c2);

				if (matrix[uint(c2->type)][uint(c1->type)] && c2->listener)
					c2->listener->OnCollision(c2, c1);
			}
		}
	}

	return true;
}

bool ModuleCollisions::Update()
{
	return true;
}

bool ModuleCollisions::PostUpdate()
{
	DebugDraw();
	//if (App->debug->debugColBox)
	//{
		//DebugDraw();
	//}
	
	return true;
}

void ModuleCollisions::DebugDraw()
{
	
	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
			case Type::WALL: // WHITE
				//app->render->DrawRectangle(colliders[i]->rect, 255, 255, 255, 80);
				break;
			}
	}
	
}

// Called before quitting
bool ModuleCollisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

void ModuleCollisions::CleanUpScene()
{
	LOG("Freeing all colliders in Scene");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}
}

Collider* ModuleCollisions::AddCollider(SDL_Rect rect, Type type, Module* listener)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, listener);
			break;
		}
	}

	return ret;
}
