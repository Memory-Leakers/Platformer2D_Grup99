#include "Interactable.h"

Interactable::Interactable()
{

}

Interactable::~Interactable()
{

}

bool Interactable::Start()
{

	return true;
}

bool Interactable::PreUpdate()
{

	return true;
}

bool Interactable::Update(float dt)
{

	return true;
}

bool Interactable::PostUpdate()
{

	return true;
}

bool Interactable::CleanUp()
{

	return true;
}

void Interactable::OnCollision(Collider* col)
{

}

void Interactable::WillCollision()
{
	
}

void Interactable::stateMachine()
{

}

void Interactable::Save(pugi::xml_node& data) const
{
	pugi::xml_node i = data.child("interactables").append_child("i");
	i.append_attribute("posX") = position.x;
	i.append_attribute("posY") = position.y;
	i.append_attribute("id") = (int) id;
}