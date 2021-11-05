#include "Player.h"
#include "App.h"
#include <vector>

Player::Player()
{
	/*
	#define camX -118
	#define camY -2401
	*/
	position.x = 207;
	position.y = 1429;

	//Rect for col
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 22;
	bounds.h = 26;

	//IDLE Anim
	idleAnim.PushBack({0,14,23,26});//IDLE
	idleAnim.PushBack({32,14,23,26});
	idleAnim.PushBack({64,14,23,26});//IDLE
	idleAnim.PushBack({96,14,23,26});
	idleAnim.PushBack({128,14,23,26 });
	idleAnim.PushBack({160,14,23,26 });
	idleAnim.PushBack({ 192,14,23,26 });
	idleAnim.PushBack({ 224,14,23,26 });
	idleAnim.PushBack({ 256,14,23,26 });
	idleAnim.PushBack({ 288,14,23,26 });
	idleAnim.PushBack({ 320,14,23,26 });
	idleAnim.PushBack({ 0,14,23,26 });
	idleAnim.speed = 0.25;
	idleAnim.hasIdle = false;
	//Right Anim
	rightAnim.PushBack({0,45,25,28});
	rightAnim.PushBack({32,45,25,28});
	rightAnim.PushBack({64,45,25,28});
	rightAnim.PushBack({96,45,25,28});
	rightAnim.PushBack({127,45,25,28});
	rightAnim.PushBack({159,45,25,28});
	rightAnim.PushBack({192,45,25,28});
	rightAnim.PushBack({224,45,25,28});
	rightAnim.PushBack({256,45,25,28});
	rightAnim.PushBack({288,45,25,28});
	rightAnim.PushBack({319,45,25,28});
	rightAnim.PushBack({351,45,25,28});
	rightAnim.PushBack({ 0,45,25,28 });
	rightAnim.speed = 0.30;
	rightAnim.hasIdle = false;
	//Left Anim
	leftAnim.PushBack({ 0,45,25,28 });
	leftAnim.PushBack({ 32,45,25,28 });
	leftAnim.PushBack({ 64,45,25,28 });
	leftAnim.PushBack({ 96,45,25,28 });
	leftAnim.PushBack({ 127,45,25,28 });
	leftAnim.PushBack({ 159,45,25,28 });
	leftAnim.PushBack({ 192,45,25,28 });
	leftAnim.PushBack({ 224,45,25,28 });
	leftAnim.PushBack({ 256,45,25,28 });
	leftAnim.PushBack({ 288,45,25,28 });
	leftAnim.PushBack({ 319,45,25,28 });
	leftAnim.PushBack({ 351,45,25,28 });
	leftAnim.PushBack({ 0,45,25,28 });
	leftAnim.speed = 0.30;
	leftAnim.hasIdle = false;
	currentAnimation = &idleAnim;

	// Init move direccion
	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}

	
}

bool Player::Start()
{

	//Texture
	player_tex = app->tex->Load("Assets/MainCharacters/Ninja_Frog_Sprites.png");

	this->jumpHeight = 16.0f;
	this->acceleration = 1.0f;
	this->velocityMax = 10.f;
	this->drag = 0.98f;

	//Collider
	this->col = app->col->AddCollider(bounds, Type::PLAYER, app->scene);

	return true;
}

bool Player::PreUpdate()
{
	isFlip = false;
	
	onGround = true;

	willcollide = false;

	return true;
}

bool Player::Update(float dt)
{
	///COLL
	WillCollision();
	///
	currentAnimation->Update();

	jumptimer.Update();
	
	
	if(canMoveDir[DOWN] && jumptimer.getDeltaTime() < 0.75f)
	{
		position.y += gravity;
	}
	/*if(!canMoveDir[RIGHT])
	{
		position.y -= 1;
	}*/
	
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && canMoveDir[UP])
	{
		/*if (onGround)
		{
			velocity.y = jumpHeight * -1;
			position.y += velocity.y;
			onGround = false;
			
		}*/
		if(!canMoveDir[DOWN])
		{
			Jump(dt);
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && canMoveDir[DOWN])
	{
		position.y += speed;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && canMoveDir[LEFT])
	{
		
		position.x -= speed;
		currentAnimation = &leftAnim;
		isFlip = true;
		direction = LEFT;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && canMoveDir[RIGHT])
	{
		
		position.x += speed;
		currentAnimation = &rightAnim;
		direction = RIGHT;
		
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		currentAnimation = &idleAnim;
		direction = UP;
	}
	

	printf("\n%d , %d\n", position.x, position.y);

	// Reset Movemenet
	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}

	col->SetPos(position);

	cout << playerScore << endl;

	return true;
}

bool Player::PostUpdate()
{
	playerRect = &currentAnimation->GetCurrentFrame();

	iPoint tempPos = position;

	if (isFlip)
	{
		app->render->DrawTexture(player_tex, tempPos.x, tempPos.y, playerRect, 1.0f, SDL_FLIP_HORIZONTAL);
		isFlip = false;
	}
	else
	{
		app->render->DrawTexture(player_tex, tempPos.x, tempPos.y, playerRect);
	}

	//Debug
	if (app->scene->gameScene->debugTiles)
	{

		bounds.x = position.x;
		bounds.y = position.y;
		app->render->DrawRectangle(bounds, 255, 255, 255, 80);
	}

	return true;
}

bool Player::CleanUp() {

	playerRect = nullptr;
	player_tex = nullptr;
	currentAnimation = nullptr;


	return true;
}

Player::~Player()
{

}

void Player::Jump(float dt)
{
	jumptimer.Reset();
	switch (direction)
	{
	case RIGHT:
		// And test that the character is not on the ground again.
		/*if (position.y < 1429)
		{
			position.y = groundHeight(pos.x);
			IsJumping = false;
		}*/
		if (jumptimer.getDeltaTime() <= 0.75f && !canMoveDir[DOWN]) 
		{
			playercenter = GetPlayerCenterPosition();
			jumphigh = { playercenter.x + 8,playercenter.y - 16 };
			resultvec.x = (jumphigh.x - playercenter.x) / sqrt(pow((jumphigh.x - playercenter.x), 2));
			resultvec.y = (jumphigh.y - playercenter.y) / sqrt(pow((jumphigh.y - playercenter.y), 2));

			
			position.y += (velocity.y * speed);
		}
		break;

	case LEFT:
		if (jumptimer.getDeltaTime() <= 0.75f && !canMoveDir[DOWN])
		{
			playercenter = GetPlayerCenterPosition();
			jumphigh = { playercenter.x - 8,playercenter.y - 16 };
			resultvec.x = (jumphigh.x - playercenter.x) / sqrt(pow((jumphigh.x - playercenter.x), 2));
			resultvec.y = (jumphigh.y - playercenter.y) / sqrt(pow((jumphigh.y - playercenter.y), 2));

			position.x -= (velocity.x * 3);
			position.y += (velocity.y * speed);
		}
		break;

	case UP:
		if (jumptimer.getDeltaTime() <= 0.75f && !canMoveDir[DOWN])
		{
			playercenter = GetPlayerCenterPosition();
			jumphigh = { playercenter.x - 0,playercenter.y - 16 };
			/*resultvec.x = (jumphigh.x - playercenter.x) / sqrt(pow((jumphigh.x - playercenter.x), 2));*/
			resultvec.y = (jumphigh.y - playercenter.y) / sqrt(pow((jumphigh.y - playercenter.y), 2));

			position.y += (resultvec.y * 32);
		}
		break;
	}
}

iPoint Player::GetPlayerCenterPosition()
{
	//Position player center
	iPoint pos;
	pos.x = this->position.x + (bounds.w/2);
	pos.y = this->position.y + (bounds.h /2);

	return pos;
}


void Player::OnCollision(Collider* col)
{
	//if (!godMode)
	//{
		switch (col->type)
		{

		case Type::WALL:

			onGround = true;
			
			break;
		
		/*case Type::EXPLOSION:
		case Type::ENEMY:
			if (invensibleTime <= 0 && InGrid(col)) Die(); break;

		case Type::FIREPOWER:
			App->scene->playerSettings->powerUpFlame++; break;

		case Type::BOMBPOWER:
			App->scene->playerSettings->maxBomb++;
			App->scene->playerSettings->RemainBomb(true); break;

		case Type::INVINCIBLEPOWER:
			invensibleTime = 10;
			playerInvensible.invensibleCount = SDL_GetTicks() - (App->debug->pauseTimeOffset * 1000);
			break;*/

		}

	//}
}

void Player::WillCollision()
{
	int px = position.x;
	int py = position.y;

	ListItem<MapLayer*>* mapLayerItem;
	mapLayerItem = app->map->mapData.layers.start;

	while (mapLayerItem != NULL) {
		if (mapLayerItem->data->properties.GetProperty("Navigation") == 1)
		{
			for (int x = 0; x < mapLayerItem->data->width; x++)
			{
				for (int y = 0; y < mapLayerItem->data->height; y++)
				{
					int gid = mapLayerItem->data->Get(x, y);

					int bx = x * 16;
					int by = y * 16;

					switch (gid)
					{
					case 243:

						//DOWN
						if (py + bounds.h >= by&& py <= by && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[DOWN] = false;
							willcollide = true;
						}

						//UP
						if (py >= by + 16 && py <= by + 16 && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[UP] = false;
							
						}

						//RIGHT
						if (px + bounds.w >= bx && px <= bx && py + bounds.h > by && py < by + 16)
						{
							canMoveDir[RIGHT] = false;
						}

						//LEFT
						if (px >= bx + 16 && px <= bx + 16 && py + bounds.h > by && py < by + 16)
						{
							canMoveDir[LEFT] = false;
						}

						break;
					}
				}
			}
		}
		mapLayerItem = mapLayerItem->next;
	}
}
