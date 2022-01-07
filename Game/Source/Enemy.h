#include "Entity.h"
#include "DynArray.h"

#pragma once


enum class EnemyState {
	IDLE = 0,
	WALKING,
	RUNNING,
	HIT,
	DEATH
};

enum class EnemyId
{
	NONE = 0,
	WALKING,
	FLYING
};

class Enemy : public Entity
{
public:
	Enemy();

	virtual ~Enemy();

	virtual bool Start() override;

	// Called each loop iteration
	virtual bool PreUpdate() override;

	// Called each loop iteration
	virtual bool Update(float dt) override;

	// Called each loop iteration
	virtual bool PostUpdate() override;

	// Called before quitting
	virtual bool CleanUp() override;

	virtual void OnCollision(Collider* col) override;

	virtual void WillCollision() override;

	virtual bool Death() override;

	virtual int pathFindingA(const iPoint& origin, const iPoint& destination);

	const DynArray<iPoint>* GetLastPath();

	void Save(pugi::xml_node& data) const override;

private:
	virtual void stateMachine();

public:
	EnemyId enemy_id = EnemyId::NONE;

private:

	// we store the created path here
	DynArray<iPoint> lastPath;

	bool canMoveDir[4];

	EnemyState eState = EnemyState::IDLE;

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
