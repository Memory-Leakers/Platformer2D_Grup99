#include "Player.h"
#include "App.h"

Player::Player()
{
	/*
	#define camX -118
	#define camY -2401
	*/
	pugi::xml_document playerfile;
	pugi::xml_node player_state_node;

	player_state_node = app->LoadPlayer(playerfile);

	player_state_node = playerfile.child("player_state");

	position.x = player_state_node.child("position").attribute("posX").as_int();
	position.y = player_state_node.child("position").attribute("posY").as_int();

	playerScore = player_state_node.child("score").attribute("score").as_int();

	gravity = player_state_node.child("gravity").attribute("value").as_int();

	//Rect for col
	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 22;
	bounds.h = 26;

	//IDLE Anim
	idleAnim.PushBack({ 0,14,23,26 });
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
	
	//Jump Anim
	/*jumpAnim.PushBack({ 0,110,23,28 });*/
	jumpAnim.PushBack({ 0,174,24,28 });
	jumpAnim.PushBack({ 32,174,24,28 });
	jumpAnim.PushBack({ 65,174,24,28 });
	jumpAnim.PushBack({ 97,174,24,28 });
	jumpAnim.PushBack({ 127,174,24,28 });
	jumpAnim.PushBack({ 159,174,24,28 });
	jumpAnim.speed = 0.30;
	jumpAnim.hasIdle = false;
	//Fall Anim 
	fallAnim.PushBack({ 0,143,24,26 });
	fallAnim.speed = 0.30;
	fallAnim.hasIdle = true;

	currentAnimation = &idleAnim;
	// Init move direccion
	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}
	canMoveDir[UP] = false;
	
}

bool Player::Start()
{

	//Texture
	player_tex = app->tex->Load("Assets/MainCharacters/Ninja_Frog_Sprites.png");

	//Collider
	this->col = app->col->AddCollider(bounds, Type::PLAYER, app->scene);

	return true;
}

bool Player::PreUpdate()
{
	if (!leftpressed)
	{
		isFlip = false;
	}
	else
	{
		isFlip = true;
	}

	return true;
}

bool Player::Update(float dt)
{
	///COLL
	WillCollision();
	///
	currentAnimation->Update();

	//RELOCATOR
	if(!canMoveDir[RIGHT] && !canMoveDir[LEFT])
	{
		if (halfUpDown)
		{
			position.y -= 2;
		}
		else {
			position.y += 2;
		}
	}

	float tempTime = previousJumpTime + JumpTime;
	jumpTimer.Update();
	if(jumpTimer.getDeltaTime() < tempTime && canMoveDir[UP])
	{
		position.y -= JUMPSPEED;
		currentAnimation = &jumpAnim;
	}
	else if (canMoveDir[DOWN])
	{
		position.y += gravity;
		previousJumpTime = -1;
		currentAnimation = &fallAnim;
		
	}
	else
	{
		jumpcounter = 0;
		//position.y -= JUMPSPEED*2 +2;
	}

	if(!canMoveDir[DOWN])
	{
		currentAnimation = &idleAnim;
	}
	
	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && canMoveDir[UP] && jumpcounter < MAX_JUMPS)
	{
		previousJumpTime = jumpTimer.getDeltaTime() + JumpTime;
		jumpcounter += 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && canMoveDir[LEFT])
	{
		
		position.x -= 2;
		currentAnimation = &leftAnim;
		isFlip = true;
		if(canMoveDir[DOWN])
		{
			currentAnimation = &jumpAnim;
			leftpressed = true;
		}
		
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && canMoveDir[RIGHT])
	{
		leftpressed = false;
		position.x += 2;
		currentAnimation = &rightAnim;
		if (canMoveDir[DOWN])
		{
			currentAnimation = &jumpAnim;
		}
		
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP && !canMoveDir[DOWN])
	{
		currentAnimation = &idleAnim;
		leftpressed = false;
		
	}
	if(app->input->GetKey(SDL_SCANCODE_A) == KEY_UP && !canMoveDir[DOWN])
	{
		currentAnimation = &idleAnim;
		isFlip = true;
		leftpressed = true;
		
	}

	printf("\n%d , %d\n", position.x, position.y);

	// Reset Movemenet
	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}
	

	col->SetPos(position);

	//cout << playerScore << endl;
	

	return true;
}

bool Player::PostUpdate()
{
	playerRect = &currentAnimation->GetCurrentFrame();

	iPoint tempPos = position;

	if (isFlip)
	{
		app->render->DrawTexture(player_tex, tempPos.x, tempPos.y, playerRect, 1.0f, SDL_FLIP_HORIZONTAL);
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
					int aux = py - JUMPSPEED;
					switch (gid)
					{
					case 243:

						//DOWN
						
						if (py + bounds.h >= by&& py <= by && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[DOWN] = false;

							if (aux + bounds.h >= by && aux <= by) { position.y -= JUMPSPEED; }

							if (py + bounds.h/2 >= by)
							{
								halfUpDown = false;
							}
							else if (py + bounds.h/2 <= by)
							{
								halfUpDown = true;
							}	
						}
						

						//UP
						if (py >= by + 16 && py <= by + 16 && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[UP] = false;
							if (py >= by + 8)
							{
								halfUpDown = false;
							}
							else if (py <= by + 8)
							{
								halfUpDown = true;
							}
						}

						//RIGHT
						if (px + bounds.w >= bx && px <= bx && py + bounds.h > by && py < by + 16)
						{
							canMoveDir[RIGHT] = false;

						}

						//LEFT
						if (px + bounds.w >= bx + 16 && px <= bx + 16 && py + bounds.h > by && py < by + 16)
						{
							canMoveDir[LEFT] = false;
						}

						break;

					case 244: //DETH AREA

						break;

					case 247: //Platform move up

						//DOWN
						if (py + bounds.h >= by && py <= by - (bounds.h - 4) && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[DOWN] = false;

							if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
							{
								position.y += gravity+1;
							}

						}

						break;
					}
				}
			}
		}
		mapLayerItem = mapLayerItem->next;
	}
}
