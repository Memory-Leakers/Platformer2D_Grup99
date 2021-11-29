#include "Enemy.h"


Enemy::Enemy()
{

}

Enemy::~Enemy()
{
}



bool Enemy::Start()
{
    return true;
}

bool Enemy::PreUpdate()
{
    return true;
}

bool Enemy::Update(float dt)
{
    return true;
}

bool Enemy::PostUpdate()
{
    return false;
}

bool Enemy::CleanUp()
{
    return true;
}

void Enemy::OnCollision(Collider* col)
{
}

void Enemy::WillCollision()
{
}
