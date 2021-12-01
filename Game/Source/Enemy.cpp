#include "Enemy.h"
#include "DynArray.h"
#include "List.h"

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

//void Enemy::pathFindingA(const iPoint& origin, const iPoint& destination)
//{
//    PathList open;
//    PathList closed;
//
//    PathNode originNode;
//
//    originNode.pos = origin;
//
//    open.list.add(originNode)
//}
//
//Enemy::PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
//{
//}
//
//Enemy::PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent)
//{
//}
//
//Enemy::PathNode::PathNode(const PathNode& node): g(node.g), h(node.h), pos(node.pos), parent(node.parent)
//{
//}
//
//int Enemy::PathNode::Score() const
//{
//    return g+h;
//}
//
//int Enemy::PathNode::CalculateF(const iPoint& destination)
//{
//    g = parent->g + 1;
//    h = pos.DistanceTo(destination);
//    return g+h;
//}
