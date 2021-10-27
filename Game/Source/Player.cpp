#include "Player.h"
#include "App.h"

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
	bounds.h = 27;

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

	//Collider
	this->col = app->col->AddCollider(bounds, Type::PLAYER, app->scene);

	return true;
}

bool Player::Update(float dt)
{
	///COLL
	WillCollision();
	///
	currentAnimation->Update();

	if ((app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) && canMoveDir[UP])
		position.y -= 1;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && canMoveDir[DOWN])
		position.y += 1;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && canMoveDir[LEFT])
	{
		position.x -= 1;
		currentAnimation = &leftAnim;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && canMoveDir[RIGHT])
	{
		position.x += 1;
		currentAnimation = &rightAnim;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP || app->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		
		currentAnimation = &idleAnim;
	}

	//printf("\n%d , %d\n", position.x, position.y);

	// Reset Movemenet
	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}

	//bounds.x = position.x;
	//bounds.y = position.y;

	return true;
}

bool Player::PostUpdate()
{
	playerRect = &currentAnimation->GetCurrentFrame();

	iPoint tempPos = position;

	app->render->DrawTexture(player_tex, tempPos.x, tempPos.y, playerRect);

	//Debug
	if (app->scene->gameScene->debugTiles)
	{
		bounds.x = position.x;
		bounds.y = position.y;
		app->render->DrawRectangle(bounds, 255, 255, 255, 80);
	}

	return true;
}

bool Player::CleanUp()
{
	delete this->col;
	this->col = nullptr;

	return true;
}

Player::~Player()
{

}

void Player::OnCollision(Collider* col)
{
	//if (!godMode)
	//{
		switch (col->type)
		{
			/*
		case Type::EXPLOSION:
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
			break;
		*/
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
						if (py + bounds.h >= by && py <= by && px + bounds.w > bx && px < bx + 16)
						{
							canMoveDir[DOWN] = false;
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