#include "Collider.h"
#include "DynArray.h"

#pragma once


enum class EnemyState {
	IDLE = 0,
	WALKING,
	RUNNING,
	HIT,
	DEATH
};

class Enemy
{
public:
	Enemy();

	~Enemy();

	virtual bool Start();

	// Called each loop iteration
	virtual bool PreUpdate();

	// Called each loop iteration
	virtual bool Update(float dt);

	// Called each loop iteration
	virtual bool PostUpdate();

	// Called before quitting
	virtual bool CleanUp();

	virtual void OnCollision(Collider* col);

	virtual void WillCollision();

	virtual int pathFindingA(const iPoint& origin, const iPoint& destination);

	const DynArray<iPoint>* GetLastPath();

private:
	virtual void stateMachine();

public:
	// Collider
	Collider* col = nullptr;

	bool death = false;
private:

	iPoint pos;
	// we store the created path here
	DynArray<iPoint> lastPath;

	SDL_Rect Enemybounds;

	bool canMoveDir[4];



	EnemyState eState = EnemyState::IDLE;

	int health = 2;
	bool damaged = false;

	friend class WalkingEnemy;
	
	friend class FlyingEnemy;
};



struct PathList;

struct PathNode
{
	int g;
	int h;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end

	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill);
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);
};

struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	ListItem<PathNode>* Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	ListItem<PathNode>* GetNodeLowestScore() const;

	// The list itself, note they are not pointers!
	List<PathNode> list;
};
