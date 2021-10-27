#include "Collider.h"
#include <iostream>
using namespace std;

Collider::Collider(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener)
{
		
}

Collider::~Collider()
{
	//delete listener;
}

void Collider::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

void Collider::SetPos(iPoint pos)
{
	rect.x = pos.x;
	rect.y = pos.y;
}

iPoint Collider::getPos()
{
	return { rect.x, rect.y };
}

bool Collider::WillIntersects(const SDL_Rect& r) const
{
	return (rect.x <= r.x + r.w &&
		rect.x + rect.w >= r.x &&
		rect.y <= r.y + r.h &&
		rect.h + rect.y >= r.y);
}

bool Collider::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}

SString Collider::getTypeAsString()
{
	switch (type)
	{
	case Type::NONE:
		return "None";
	case Type::WALL:
		return "Wall";
	case Type::PLAYER:
		return "Player";
	case Type::ENEMY:
		return "Enemy";
	case Type::PICKER:
		return "Picker";
	case Type::AREA:
		return "Area";
	case Type::MAX:
		return "Max";
	}
}