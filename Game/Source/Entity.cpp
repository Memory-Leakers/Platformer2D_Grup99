#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{

}

bool Entity::Start()
{

	return true;
}

bool Entity::PreUpdate()
{

	return true;
}

bool Entity::Update(float dt)
{

	return true;
}

bool Entity::PostUpdate()
{

	return true;
}

bool Entity::CleanUp()
{

	return true;
}

void Entity::OnCollision(Collider* col)
{

}

void Entity::WillCollision()
{

}

bool Entity::Death()
{

	return true;
}

void Entity::Save(pugi::xml_node& data) const
{

}