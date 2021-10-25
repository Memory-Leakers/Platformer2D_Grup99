#pragma once
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"


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

public:

	iPoint position;

	SDL_Rect bounds;

	bool isFlip = false;

	SDL_Texture* player_tex;

	// A set of animations
	Animation idleAnim;

	Animation upAnim;

	Animation downAnim;

	Animation leftAnim;

	Animation rightAnim;

	Animation* currentAnimation = nullptr;

};

