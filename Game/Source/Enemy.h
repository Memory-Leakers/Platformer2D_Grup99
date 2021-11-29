#include "Collider.h"

#pragma once
class Enemy
{
public:
	Enemy();

	~Enemy();

	virtual bool Start();

	// Called each loop iteration
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

	virtual void OnCollision(Collider* col);

	virtual void WillCollision();

private:
	
};

