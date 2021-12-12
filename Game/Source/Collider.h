#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "External/SDL/include/SDL_Rect.h"

#include "Point.h"
#include "SString.h"

class Module;


//MAX MUST BE AT THE END OF THE LIST
enum class Type
{
	NONE = -1,
	WALL,
	PLAYER,
	ENEMY,
	PICKER,
	AREA,
	TRAP,
	MAX
};

struct Collider
{
	// Methods
	Collider(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	~Collider();

	void SetPos(int x, int y);

	void SetPos(iPoint pos);

	iPoint getPos();

	bool Intersects(const SDL_Rect& r) const;

	bool WillIntersects(const SDL_Rect& r) const;

	// Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;

	SString getTypeAsString();

};

#endif // !__COLLIDER_H__