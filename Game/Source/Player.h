#pragma once
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"


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

public:

	iPoint position;

	bool canMoveDir[4];

	SDL_Rect bounds;

	SDL_Rect* playerRect;

	bool isFlip = false;

	SDL_Texture* player_tex;

	// A set of animations
	Animation idleAnim;

	Animation upAnim;

	Animation downAnim;

	Animation leftAnim;

	Animation rightAnim;

	Animation* currentAnimation = nullptr;

	// Collider
	Collider* col = nullptr;

};

