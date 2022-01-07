#pragma once
#include "Entity.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"
#include "Collider.h"
#include "Timer.h"
#include "PugiXml/src/pugixml.hpp"
#include <math.h>


#define MAX_JUMPS 2
#define JumpTime 0.18f
#define JUMPSPEED 3
#define MOVESPEED 3

#define textureOpacityNormal 255
#define textureOpacityHurt 155


enum Dir
{
	UP = 0,
	DOWN,
	LEFT,
	RIGHT
};

class Player : public Entity
{
public:
	Player();

	~Player();

	// Called when the module is activated
	// Loads the necessary textures for the player
	bool Start() override;

	bool PreUpdate();
	// Called at the middle of the application loop
	// Processes new input and handles player movement
	bool Update(float dt) override;

	// Called at the end of the application loop
	// Performs the render call of the player sprite
	bool PostUpdate() override;

	bool CleanUp() override;

	// Collision callback, called when the player intersects with another
	void OnCollision(Collider* col) override;

	// Callback if will be collision, called when the player intersects with another
	void WillCollision() override;

	bool Death() override;

	iPoint GetPlayerCenterPosition();	

private:

	pugi::xml_document playerfile;
	pugi::xml_node player_state_node;

	// A set of animations
	Animation idleAnim;

	Animation upAnim;

	Animation downAnim;

	Animation leftAnim;

	Animation rightAnim;

	Animation jumpAnim;

	Animation fallAnim;

	Animation doublejumpAnim;

	float hurtTimeJump = 0.20f;
	float movementPrevention = 0.15f;
	float hurtOpacityTime = 1.25f;
	float hurtTime = 0.0f;
	bool hit = false;
	bool startHurt = false;
	int lastDirHorizontal = LEFT;
	int texOpacity = 255;
	int hurtSpeedHorizontal = 5;
	int hurtSpeedVertical = 6;

public:

	//Physics
	int jumpcounter = 0;

	iPoint playercenter;
	iPoint jumphigh;
	iPoint resultvec;

	Timer jumpTimer;

	float previousJumpTime = -1;

	//Movement & textures
	bool canMoveDir[4];

	bool isFlip = false;

	//Others
	int playerScore = 0;

	bool godMode = false;

	bool leftpressed = false;

};
