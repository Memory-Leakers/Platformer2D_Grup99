#include "Module.h"

bool Module::Start()
{
	return true;
}

bool Module::PreUpdate()
{
	return true;
}

bool Module::Update(float dt)
{
	return true;
}

bool Module::PostUpdate()
{
	return true;
}

void Module::OnCollision(Collider* c1, Collider* c2)
{

}

void Module::WillCollision(Collider* c1, Collider* c2)
{

}

bool Module::CleanUp()
{
	return true;
}

bool Module::SaveSettings(pugi::xml_node& config)
{
	return true;
}