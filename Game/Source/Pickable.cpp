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

void Pickable::Save(pugi::xml_node& data) const
{
	pugi::xml_node i = data.child("pickables").append_child("p");
	i.append_attribute("posX") = position.x;
	i.append_attribute("posY") = position.y;
	i.append_attribute("id") = (int)pickable_id;
}

