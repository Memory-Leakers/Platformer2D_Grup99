#pragma once
#include "Pickable.h"
class DoorKey :
    public Pickable
{
public:
	DoorKey(int posX, int posY);

	~DoorKey();

	bool Start() override;

	bool PreUpdate() override;

	bool Update(float dt) override;

	bool PostUpdate() override;

	bool CleanUp() override;

	void OnCollision(Collider* col) override;

	void WillCollision() override;
};

