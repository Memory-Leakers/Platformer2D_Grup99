
#include "WalkingEnemy.h"

WalkingEnemy::WalkingEnemy(iPoint position, float speed):Enemy(position,speed)
{
	
}

WalkingEnemy::~WalkingEnemy()
{
}

bool WalkingEnemy::Start()
{
	return false;
}

bool WalkingEnemy::PreUpdate()
{
	return false;
}

bool WalkingEnemy::Update(float dt)
{
	return false;
}

bool WalkingEnemy::PostUpdate()
{
	return false;
}

bool WalkingEnemy::CleanUp()
{
	return false;
}

void WalkingEnemy::OnCollision(Collider* col)
{
}

void WalkingEnemy::WillCollision()
{
}
