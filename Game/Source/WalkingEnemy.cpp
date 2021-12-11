#include "App.h"
#include "WalkingEnemy.h"

WalkingEnemy::WalkingEnemy(int x, int y)
{

	Walking_Enemy_node = app->LoadWalkingEnemy(WalkingEnemyfile);

	Walking_Enemy_node = WalkingEnemyfile.child("enemy_state");

	this->pos.x = x;
	this->pos.y = y - 14;

	/*gravity = Walking_Enemy_node.child("gravity").attribute("value").as_int();*/
	
	Enemybounds.x = pos.x;
	Enemybounds.y = pos.y;
	Enemybounds.w = 32;
	Enemybounds.h = 30;

	//SET OF ANIMATIONS

	//IDLE
	angrypigIdle.PushBack({ 0,0,36,30 });
	angrypigIdle.PushBack({ 36,0,36,30 });
	angrypigIdle.PushBack({ 72,0,36,30 });
	angrypigIdle.PushBack({ 108,0,36,30 });
	angrypigIdle.PushBack({ 144,0,36,30 });
	angrypigIdle.PushBack({ 180,0,36,30 });
	angrypigIdle.PushBack({ 216,0,36,30 });
	angrypigIdle.PushBack({ 252,0,36,30 });
	angrypigIdle.PushBack({ 288,0,36,30 });

	angrypigIdle.speed = 0.20;
	angrypigIdle.hasIdle = false;

	//RUN
	angrypigRun.PushBack({ 0,0,36,30 });
	angrypigRun.PushBack({ 36,0,36,30 });
	angrypigRun.PushBack({ 72,0,36,30 });
	angrypigRun.PushBack({ 108,0,36,30 });
	angrypigRun.PushBack({ 144,0,36,30 });
	angrypigRun.PushBack({ 180,0,36,30 });
	angrypigRun.PushBack({ 216,0,36,30 });
	angrypigRun.PushBack({ 252,0,36,30 });
	angrypigRun.PushBack({ 288,0,36,30 });
	angrypigRun.PushBack({ 324,0,36,30 });
	angrypigRun.PushBack({ 360,0,36,30 });
	angrypigRun.PushBack({ 396,0,36,30 });
	angrypigRun.speed = 0.20;
	angrypigRun.hasIdle = false;

	//WALK
	angrypigWalk.PushBack({ 0,0,36,30 });
	angrypigWalk.PushBack({ 36,0,36,30 });
	angrypigWalk.PushBack({ 72,0,36,30 });
	angrypigWalk.PushBack({ 108,0,36,30 });
	angrypigWalk.PushBack({ 144,0,36,30 });
	angrypigWalk.PushBack({ 180,0,36,30 });
	angrypigWalk.PushBack({ 216,0,36,30 });
	angrypigWalk.PushBack({ 252,0,36,30 });
	angrypigWalk.PushBack({ 288,0,36,30 });
	angrypigWalk.PushBack({ 324,0,36,30 });
	angrypigWalk.PushBack({ 360,0,36,30 });
	angrypigWalk.PushBack({ 396,0,36,30 });
	angrypigWalk.PushBack({ 432,0,36,30 });
	angrypigWalk.PushBack({ 468,0,36,30 });
	angrypigWalk.PushBack({ 504,0,36,30 });
	angrypigWalk.PushBack({ 540,0,36,30 });

	angrypigWalk.speed = 0.20;
	angrypigWalk.hasIdle = false;

	//HIT
	angrypigHit.PushBack({ 0,0,36,30 });
	angrypigHit.PushBack({ 36,0,36,30 });
	angrypigHit.PushBack({ 72,0,36,30 });
	angrypigHit.PushBack({ 108,0,36,30 });
	angrypigHit.PushBack({ 144,0,36,30 });
	angrypigHit.speed = 0.20;
	angrypigHit.hasIdle = false;


	Currentenemyanimation = &angrypigIdle;

	//DELETES OPEN XML FILE
}

WalkingEnemy::~WalkingEnemy()
{
}

bool WalkingEnemy::Start()
{
	pugi::xml_node Walking_Enemy_textures_node = Walking_Enemy_node.child("textures");
	//IDLE
	enemytextures[0] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigIdle").attribute("path").as_string());
	//RUN
	enemytextures[2] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigRun").attribute("path").as_string());
	//WALK
	enemytextures[1] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigWalk").attribute("path").as_string());
	//HIT
	enemytextures[3] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigHit").attribute("path").as_string());

	this->col = app->col->AddCollider(Enemybounds, Type::ENEMY, app->scene);

	return true;
}

bool WalkingEnemy::PreUpdate()
{
	int xx = (pos.x - app->scene->gameScene->froggy->position.x);
	int yy = (pos.y - app->scene->gameScene->froggy->position.y);
	int posPDif = sqrt(xx*xx + yy*yy);

	if (posPDif > 200)
	{
		return true;
	}
	//if ( app->scene->gameScene->froggy->position != lastPlayerPos)
	//{
		 pathFindingA(pos, app->scene->gameScene->froggy->position);
	//}
	lastPlayerPos = app->scene->gameScene->froggy->position;

	return true;
}

bool WalkingEnemy::Update(float dt)
{

	if (damaged)
	{
		Currentenemyanimation->Update();
		eState = EnemyState::HIT;
		return true;
	}

	///COLL
	WillCollision();
	
	const DynArray<iPoint>* path = GetLastPath();


	int pathCount = path->Count();

	if (pathCount <= walkingPathRange && pathCount != 0)
	{
		eState = EnemyState::WALKING;
	}
	else
	{
		eState = EnemyState::IDLE;
	}

	if (cont < pathCount && pathCount <= walkingPathRange)
	{
		iPoint pos;
		
		pos = app->map->MapToWorld(path->At(cont)->x, path->At(cont)->y);

		int posDifX = abs(pos.x+ 8 - this->pos.x);

		//Movement
		if (posDifX >= 15)
		{
			if (pos.x > this->pos.x && canMoveDir[RIGHT])
			{
				this->pos.x += speed;
				
			}
			else if (pos.x < this->pos.x && canMoveDir[LEFT])
			{
				this->pos.x -= speed;
			}
		}
		
 		int posDifY = abs(pos.y - this->pos.y);
		//Jump
		if (pos.y < this->pos.y && !jumping 
			&& !canMoveDir[DOWN] && posDifY > 8 
			&& dt - startJump >= jumpDelayTime)
		{
			jumping = true;
			startJump = dt;
		}


		cont++;
	}
	else 
	{
		cont = 0;
	}

	//JUMP
	if (jumping == true)
	{
		if (dt - startJump <= jumpTime)
		{
			if (canMoveDir[UP]) this->pos.y -= 2;
		}
		else {
			jumping = false;
		}
	}

	//Gravity
	if (canMoveDir[DOWN] && !jumping)
	{
		this->pos.y += 4;
	}

	col->SetPos(pos);

	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}
	

	Currentenemyanimation->Update();

	return true;
}

bool WalkingEnemy::PostUpdate()
{
	stateMachine();

	if (app->scene->gameScene->debugTiles)
	{
		Enemybounds.x = pos.x;
		Enemybounds.y = pos.y;
		app->render->DrawRectangle(Enemybounds, 0, 200, 255, 80);
		std::cout << " EnemyBounds:" << pos.x << "," << pos.y << endl;


		//PATH
		const DynArray<iPoint>* path = GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawRectangle({ pos.x, pos.y, 16, 16 }, 0, 0, 0, 160);
		}

	}
	return true;
}

bool WalkingEnemy::CleanUp()
{
	EnemyRect = nullptr;
	for (int i = 0; i < 4; i++)
	{
		SDL_DestroyTexture(enemytextures[i]);
		enemytextures[i] = nullptr;
	}
	Currentenemyanimation = nullptr;

	if (col != nullptr)
	{
		col->pendingToDelete = true;
	}
	WalkingEnemyfile.~xml_document();
	return true;
}


iPoint WalkingEnemy::GetCenterEnemyPos()
{
	iPoint pos;
	pos.x = this->pos.x + (Enemybounds.w / 2);
	pos.y = this->pos.y + (Enemybounds.h / 2);
	return pos;
}

void WalkingEnemy::stateMachine()
{
	iPoint tempPos = pos;

	switch (eState)
	{
	case EnemyState::IDLE:
		Currentenemyanimation = &angrypigIdle;
		break;
	case EnemyState::WALKING:
		Currentenemyanimation = &angrypigWalk;
		break;
	case EnemyState::RUNNING:
		Currentenemyanimation = &angrypigRun;
		break;
	case EnemyState::HIT:
		Currentenemyanimation = &angrypigHit;
		break;
	case EnemyState::DEATH:
		//app->render->DrawTexture(enemytextures[3], tempPos.x, tempPos.y, EnemyRect);
		break;
	}
	EnemyRect = &Currentenemyanimation->GetCurrentFrame();

	if (Currentenemyanimation == &angrypigHit && Currentenemyanimation->getCurrentFrameI() == angrypigHit.size() - 1)
	{
		damaged = false;
	}

	app->render->DrawTexture(enemytextures[(int) eState], tempPos.x, tempPos.y, EnemyRect);


}