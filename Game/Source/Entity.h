#pragma once
#include "Point.h"
#include "Collider.h"
#include "Animation.h"
#include "List.h"
#include "PugiXml/src/pugixml.hpp"

class Entity
{
public:
	Entity();

	virtual ~Entity();

	/// <summary>
	/// Called when the module is activated
	/// Loads the necessary data for the entity
	/// </summary>
	/// <returns></returns>
	virtual bool Start();

	/// <summary>
	/// Called before any other update
	/// </summary>
	/// <returns></returns>
	virtual bool PreUpdate();
	
	/// <summary>
	/// Called at the middle of the application loop
	/// Processes new input and handles the movement of the entity
	/// </summary>
	/// <param name="dt"></param>
	/// <returns></returns>
	virtual bool Update(float dt);

	/// <summary>
	/// Called at the end of the application loop
	/// Performs the render of an Entity
	/// </summary>
	/// <returns></returns>
	virtual bool PostUpdate();

	virtual bool CleanUp();

	/// <summary>
	/// Collision callback, called when the entity intersects with another entity's collision box
	/// </summary>
	/// <param name="col"></param>
	virtual void OnCollision(Collider* col);

	/// <summary>
	/// Callback if will be collision, called when the entity intersects with a tilemapped collision box
	/// </summary>
	virtual void WillCollision();

	/// <summary>
	/// Is triggered whenever the entity dies
	/// </summary>
	/// <returns></returns>
	virtual bool Death();

	/// <summary>
	/// Called when the game is going to be saved. Saves the current state of the entity
	/// </summary>
	/// <param name="data"></param>
	/// <returns></returns>
	virtual void Save(pugi::xml_node& data) const;

public:
	//Stats
	int maxHealth = 3;
	int health = maxHealth;

	bool hurt = false;
	bool dead = false;

	//Physics
	int gravity = 3;
	Collider* col = nullptr;

	iPoint position;

	//Movement & Textures
	SDL_Rect bounds;
	SDL_Rect* rect = nullptr;
	bool isFlip = false;

	SDL_Texture* tex = nullptr;
	Animation* currentAnim = nullptr;

	/*GETTERS AND SETTERS*/
	virtual int getHealth()
	{
		return health;
	}
};

