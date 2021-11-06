#pragma once
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"
#include "Timer.h"
#include "PugiXml/src/pugixml.hpp"
#include <math.h>


#define MAX_JUMPS 2
#define JumpTime 0.25f
#define JUMPSPEED 2



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

private:

	// A set of animations

	Animation idleAnim;

	Animation upAnim;

	Animation downAnim;

	Animation leftAnim;

	Animation rightAnim;

	Animation jumpAnim;

	Animation fallAnim;

	Animation* currentAnimation = nullptr;

	

public:

	//Physics

	int gravity;

	int jumpcounter = 0;

	iPoint playercenter;
	iPoint jumphigh;
	iPoint resultvec;

	iPoint position;

	Timer jumpTimer;

	float previousJumpTime = -1;

	//Movement & textures

	bool canMoveDir[4];

	SDL_Rect bounds;

	SDL_Rect* playerRect;

	bool isFlip = false;

	SDL_Texture* player_tex;

	// Collider
	Collider* col = nullptr;

	int playerScore = 0;

	bool godMode = false;

private:
	bool halfUpDown = false; //false down

	bool leftpressed = false;
};
