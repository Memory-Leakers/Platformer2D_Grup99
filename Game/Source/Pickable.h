#pragma once
#include "Entity.h"
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

enum class PickID
{
	NONE = 0,
	FRUIT,
	HEARTH,
	KEY
};

class Pickable : public Entity
{
public:
	Pickable();

	virtual ~Pickable();

	virtual bool Start() override;

	virtual bool PreUpdate() override;

	virtual bool Update(float dt) override;

	virtual bool PostUpdate() override;

	virtual bool CleanUp() override;

	virtual void OnCollision(Collider* col) override;

	virtual void WillCollision() override;

	void Save(pugi::xml_node& data) const override;

public:

	PickID pickable_id = PickID::NONE;

	int pickId = NONE;

protected:
	Animation anim;
};

