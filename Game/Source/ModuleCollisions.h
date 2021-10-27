#ifndef __MODULECOLLISIONS_H__
#define __MODULECOLLISIONS_H__

#define MAX_COLLIDERS 2000

#include "Module.h"

#include "PugiXml\src\pugixml.hpp"


class ModuleCollisions : public Module
{
public:
	// Constructor
	// Fills all collision matrix data
	ModuleCollisions();

	// Destructor
	~ModuleCollisions();

	void Init();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	// Called at the beginning of the application loop
	// Removes all colliders pending to delete
	// Checks for new collisions and calls its listeners
	bool PreUpdate();

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update();

	// Called at the end of the application loop
	// Draw all colliders (if debug mode is enabled)
	bool PostUpdate();

	// Removes all existing colliders
	bool CleanUp();

	// Adds a new collider to the list
	Collider* AddCollider(SDL_Rect rect, Type type, Module* listener = nullptr);

	// Draws all existing colliders with some transparency
	void DebugDraw();

	void CleanUpScene();

public:

	SString name;
	bool active;

private:
	// All existing colliders in the scene
	Collider* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2
	bool matrix[uint(Type::MAX)][uint(Type::MAX)];

	// Simple debugging flag to draw all colliders
	bool debug = false;
};

#endif // __MODULECOLLISIONS_H__
