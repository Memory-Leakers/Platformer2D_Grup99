#pragma once
#include "Enemy.h"
class WalkingEnemy :
    public Enemy
{
public:
	WalkingEnemy(iPoint position, float speed);

	~WalkingEnemy();

	bool Start() override;

	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called each loop iteration
	bool PostUpdate() override;

	// Called before quitting
	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;

private:




	
};

