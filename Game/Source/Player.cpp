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
	jumpAnim.PushBack({ 0,110,23,28 });
	jumpAnim.speed = 0.30;
	jumpAnim.hasIdle = true;
	//doublejump anim
	doublejumpAnim.PushBack({ 0,174,24,28 });
	doublejumpAnim.PushBack({ 32,174,24,28 });
	doublejumpAnim.PushBack({ 65,174,24,28 });
	doublejumpAnim.PushBack({ 97,174,24,28 });
	doublejumpAnim.PushBack({ 127,174,24,28 });
	doublejumpAnim.PushBack({ 159,174,24,28 });
	doublejumpAnim.speed = 0.30;
	doublejumpAnim.hasIdle = false;
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
	if (health <= 0)
	{
		Death();
	}

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
	//Player death
	if (health == 0)
	{
		return true;
	}
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN) { health--; }

	///COLL
	WillCollision();

	//Last direction
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		lastDirHorizontal = LEFT;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		lastDirHorizontal = RIGHT;
	}

	//Hurt animation
	if (hurt)
	{
		if (startHurt)
		{
			hurtTime = dt;
			startHurt = false;
		}
		if (dt - hurtTime <= hurtTimeJump) 
		{
			if (lastDirHorizontal == LEFT && canMoveDir[LEFT])
			{
				position.x -= 5;
							}
			else if (lastDirHorizontal == RIGHT && canMoveDir[RIGHT])
			{
				position.x += 5;
			}
			else {
				position.x -= 5;
			}

			if (canMoveDir[UP])
			{
				position.y -= 6;
			}
			else {
				position.y += 2;
			}
		}

		///if (canMoveDir[DOWN])
		//{
			canMoveDir[RIGHT] = false; //Prevents going back to the hurt col
			canMoveDir[LEFT] = false;
			canMoveDir[UP] = false;
		//}

		if  (dt - hurtTime <= hurtOpacityTime)
		{
			texOpacity = textureOpacityHurt;
		}
		else {
			hurt = false;
			texOpacity = textureOpacityNormal;
		}
	}


	//RELOCATOR
	if (!godMode)
	{
		float tempTime = previousJumpTime + JumpTime;
		jumpTimer.Update();
		if (jumpTimer.getDeltaTime() < tempTime && canMoveDir[UP])
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
		}
		if(!canMoveDir[DOWN])
		{
			currentAnimation = &idleAnim;
		}
		if (jumpTimer.getDeltaTime() < tempTime && jumpcounter > 1)
		{
			currentAnimation = &doublejumpAnim;
		}

		if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) && canMoveDir[UP] && jumpcounter < MAX_JUMPS)
		{
			previousJumpTime = jumpTimer.getDeltaTime() + JumpTime;
			jumpcounter += 1;
			app->audio->PlayFx(app->scene->gameScene->playerjumpSFX, 0);
			
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && canMoveDir[LEFT])
		{
			if (!canMoveDir[DOWN])
			{
				app->audio->PlayFx(app->scene->gameScene->playerwalkSFX, 0);
			}
			position.x -= MOVESPEED;
			currentAnimation = &leftAnim;
			isFlip = true;
			if(canMoveDir[DOWN])
			{
				currentAnimation = &jumpAnim;
				leftpressed = true;
				if(jumpTimer.getDeltaTime() > tempTime)
				{
					currentAnimation = &fallAnim;
				}
				if (jumpTimer.getDeltaTime() < tempTime && jumpcounter > 1)
				{
					currentAnimation = &doublejumpAnim;
				}
			}

		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && canMoveDir[RIGHT])
		{
			if (!canMoveDir[DOWN]) 
			{
				app->audio->PlayFx(app->scene->gameScene->playerwalkSFX, 0);
			}
			leftpressed = false;
			position.x += MOVESPEED;
			currentAnimation = &rightAnim;
			if (canMoveDir[DOWN])
			{
				currentAnimation = &jumpAnim;
				if (jumpTimer.getDeltaTime() > tempTime)
				{
					currentAnimation = &fallAnim;
				}
				if (jumpTimer.getDeltaTime() < tempTime && jumpcounter > 1)
				{
					currentAnimation = &doublejumpAnim;
				}
			}
		}
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && canMoveDir[DOWN])
		{
			position.y += 2;
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

		

		// Reset Movemenet
		for (int i = 0; i < 4; i++)
		{
			canMoveDir[i] = true;
		}

		//cout << playerScore << endl;
	}
	else
	{
		if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT))
		{
			position.y -= 4;
		}
		if ((app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT))
		{
			position.y += 4;
		}
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		{
			position.x -= 4;
			currentAnimation = &leftAnim;
			isFlip = true;

		}
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		{
			position.x += 4;
			currentAnimation = &rightAnim;
		}
	}
	
	// Animation update
	currentAnimation->Update();
	if (this->col != nullptr)
	{
		col->SetPos(position);
	}
	return true;
}

bool Player::PostUpdate()
{
	if (health == 0)
	{
		return true;
	}

	playerRect = &currentAnimation->GetCurrentFrame();

	iPoint tempPos = position;

	SDL_SetTextureAlphaMod(player_tex, texOpacity);

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

		std::cout << "Player pos-> " << position.x << " | " << position.y << std::endl;
	}

	return true;
}

bool Player::CleanUp() {

	playerRect = nullptr;
	SDL_DestroyTexture(player_tex);
	player_tex = nullptr;
	currentAnimation = nullptr;
	playerRect = nullptr;

	this->col->pendingToDelete = true;

	return true;
}

Player::~Player()
{

}

bool Player::Death()
{
	app->scene->gameScene->pendingtoReload = true;

	if (app->scene->gameScene->checkpoint->getCurrentState() == 2) //2 == Activated
	{
		app->LoadGameRequest();
	}
	else {
		app->scene->gameScene->pendingtoReload = true;
	}
	return true;
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
	if (col->type == Type::TRAP && !hurt && !godMode)
	{
		std::cout << "OUCH" << std::endl;
		hurt = true;
		startHurt = true;
		health--;
	}
}

void Player::WillCollision()
{
	if (godMode) return;


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
					case 243: //Collisions
						//UP
						if (py <= by + 16 && py >= by && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[UP] = false;
						}

						//DOWN
						if (py + bounds.h >= by && py <= by && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[DOWN] = false;
						}

						//LEFT
						if (px <= bx + 16 && px >= bx && py + bounds.h > by && py < by + 16)
						{
							canMoveDir[LEFT] = false;

						}

						//RIGHT
						if (px + bounds.w >= bx && px <= bx && py + (bounds.h) > by && py < by + 16)
						{
							canMoveDir[RIGHT] = false;
						}
						while (py + bounds.h > by && py < by && px + bounds.w > bx && px < bx + 16 && !canMoveDir[DOWN] && canMoveDir[UP]) //DOWN
						{
							position.y -= 1;
							py -= 1;
							break;
						}
						while (py <= by + 16 && py >= by && px + bounds.w > bx && px < bx + 16 && canMoveDir[DOWN] && !canMoveDir[UP]) //DOWN
						{
							position.y += 1;
							py += 1;
							break;
						}
						while (px + bounds.w > bx && px < bx && py + bounds.h > by && py < by + 16 && canMoveDir[LEFT] && !canMoveDir[RIGHT] && !canMoveDir[DOWN])
						{
							position.x -= 1;
							px -= 1;
							break;
						}
						while (px < bx + 16 && px > bx && py + bounds.h > by && py < by + 16 && !canMoveDir[LEFT] && canMoveDir[RIGHT] && !canMoveDir[DOWN])
						{
							position.x += 1;
							px += 1;
							break;
						}
						break;

					case 244: //DETH AREA
						if (py + bounds.h >= by && py <= by && px + bounds.w > bx && px < bx + 16)
						{
							health = 0;
						}
						break;
					case 245: //Doors
						if (app->scene->gameScene->key == true) break;
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
					case 247: //Platform move up

						//DOWN
						if (py + bounds.h >= by && py <= by -12 && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[DOWN] = false;

							while (py + bounds.h > by)
							{
								position.y -= 1;
								py -= 1;
							}

							if (app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN)
							{
								position.y += gravity*2;
							}

						}

						break;
					case 248: //WIN
						if (py + bounds.h >= by && py <= by && px + bounds.w > bx && px < bx + 16)
						{
							app->scene->gameScene->pendingtoReload = true;
						}
						break;
					}
				}
			}
		}
		mapLayerItem = mapLayerItem->next;
	}
}
