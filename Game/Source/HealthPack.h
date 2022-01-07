#pragma once
#include "Pickable.h"



class HealthPack : public Pickable
{
public:
	HealthPack(int posX, int posY);

	~HealthPack();

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;
};

