#include "Pickable.h"


Pickable::Pickable()
{

}

Pickable::~Pickable()
{

}

bool Pickable::Start()
{
	return true;
}

bool Pickable::PreUpdate()
{
	return true;
}

bool Pickable::Update(float dt)
{
	return true;
}

bool Pickable::PostUpdate()
{
	return true;
}

bool Pickable::CleanUp()
{
	return true;
}

void Pickable::OnCollision(Collider* col)
{

}

void Pickable::WillCollision()
{

}

