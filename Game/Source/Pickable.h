#pragma once
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"

enum PickType
{
	NONE = -1,
	FRUIT = 0,
	DOORKEY
};

class Pickable
{
public:
	Pickable();

	~Pickable();

	virtual bool Start();

	virtual bool PreUpdate();

	virtual bool Update(float dt);

	virtual bool PostUpdate();

	virtual bool CleanUp();

	virtual void OnCollision(Collider* col);

	virtual void WillCollision();

public:
	bool pendingToDelete = false;

	Collider* col = nullptr;

	int pickId = NONE;
};

