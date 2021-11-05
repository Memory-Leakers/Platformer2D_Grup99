#pragma once
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"
#include "Timer.h"
#include <math.h>

#define MAX_JUMPS 2
#define JumpTime 1750

enum Dir
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};


class Player
{
public:
	Player();

	~Player();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start();

	bool PreUpdate();
	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt);

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate();

	bool CleanUp();

	// Collision callback, called when the player intersects with another
	void OnCollision(Collider* col);

	// Callback if will be collision, called when the player intersects with another
	void WillCollision();

	void Jump(float dt);

	void move(const float dir_x, const float dir_y);

	iPoint GetPlayerCenterPosition();

	void Gravity();

private:

	// A set of animations

	Animation idleAnim;

	Animation upAnim;

	Animation downAnim;

	Animation leftAnim;

	Animation rightAnim;

	Animation* currentAnimation = nullptr;

public:

	//Physics

	int jumpcounter = MAX_JUMPS;

	iPoint playercenter;
	iPoint jumphigh;
	iPoint resultvec;

	float acceleration;

	iPoint velocity{ 1,1 };

	float velocityMax;

	Dir direction;

	float gravity = 2.0f;

	float maxGravity = 8.0f;

	float speed = 2.0f;

	iPoint position;

	float jumpHeight = 16;

	Timer jumptimer;

	float previousJumpTime = 0;

	//Movement & textures

	bool canMoveDir[4];

	SDL_Rect bounds;

	SDL_Rect* playerRect;

	bool isFlip = false;

	SDL_Texture* player_tex;

	// Collider
	Collider* col = nullptr;

	int playerScore = 0;
};
