#include "App.h"
#include "WalkingEnemy.h"

WalkingEnemy::WalkingEnemy()
{

	Walking_Enemy_node = app->LoadWalkingEnemy(WalkingEnemyfile);

	Walking_Enemy_node = WalkingEnemyfile.child("enemy_state");

	pos.x = Walking_Enemy_node.child("position").attribute("posX").as_int();
	pos.y = Walking_Enemy_node.child("position").attribute("posY").as_int();

	/*gravity = Walking_Enemy_node.child("gravity").attribute("value").as_int();*/
	
	Enemybounds.x = pos.x;
	Enemybounds.y = pos.y;
	Enemybounds.w = 32;
	Enemybounds.h = 28;


	//SET OF ANIMATIONS

	//IDLE
	angrypigIdle.PushBack({ 2,2,33,28 });
	angrypigIdle.PushBack({ 38,2,33,28 });
	angrypigIdle.PushBack({ 75,2,33,28 });
	angrypigIdle.PushBack({ 111,2,33,28 });
	angrypigIdle.PushBack({ 147,2,33,28 });
	angrypigIdle.PushBack({ 183,2,33,28 });
	angrypigIdle.PushBack({ 219,2,33,28 });
	angrypigIdle.PushBack({ 255,2,33,28 });
	angrypigIdle.PushBack({ 292,2,33,28 });
	angrypigIdle.speed = 0.20;
	angrypigIdle.hasIdle = false;

	//RUN
	angrypigRun.PushBack({ 4,1,33,29 });
	angrypigRun.PushBack({ 40,1,33,29 });
	angrypigRun.PushBack({ 76,1,33,29 });
	angrypigRun.PushBack({ 112,1,33,29 });
	angrypigRun.PushBack({ 146,1,33,29 });
	angrypigRun.PushBack({ 182,1,33,29 });
	angrypigRun.PushBack({ 218,1,33,29 });
	angrypigRun.PushBack({ 254,1,33,29 });
	angrypigRun.PushBack({ 290,1,33,29 });
	angrypigRun.PushBack({ 326,1,33,29 });
	angrypigRun.PushBack({ 362,1,33,29 });
	angrypigRun.PushBack({ 398,1,33,29 });
	angrypigRun.speed = 0.20;
	angrypigRun.hasIdle = false;

	//WALK
	angrypigWalk.PushBack({ 4,0,33,29 });
	angrypigWalk.PushBack({ 40,0,33,29 });
	angrypigWalk.PushBack({ 76,0,33,29 });
	angrypigWalk.PushBack({ 112,0,33,29 });
	angrypigWalk.PushBack({ 148,0,33,29 });
	angrypigWalk.PushBack({ 184,0,33,29 });
	angrypigWalk.PushBack({ 218,0,33,29 });
	angrypigWalk.PushBack({ 254,0,33,29 });
	angrypigWalk.PushBack({ 290,0,33,29 });
	angrypigWalk.PushBack({ 326,0,33,29 });
	angrypigWalk.PushBack({ 362,0,33,29 });
	angrypigWalk.PushBack({ 398,0,33,29 });
	angrypigWalk.PushBack({ 434,0,33,29 });
	angrypigWalk.PushBack({ 471,0,33,29 });
	angrypigWalk.PushBack({ 506,0,33,29 });
	angrypigWalk.PushBack({ 542,0,33,29 });
	angrypigWalk.speed = 0.20;
	angrypigWalk.hasIdle = false;

	//HIT
	angrypigHit.PushBack({147,2,33,28});
	angrypigHit.PushBack({111,2,33,28});
	angrypigHit.PushBack({74,2,33,28 });
	angrypigHit.PushBack({37,2,35,28 });
	angrypigHit.speed = 0.20;
	angrypigHit.hasIdle = false;


	Currentenemyanimation = &angrypigIdle;
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
	enemytextures[1] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigRun").attribute("path").as_string());
	//WALK
	enemytextures[2] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigWalk").attribute("path").as_string());
	//HIT
	enemytextures[3] = app->tex->Load(Walking_Enemy_textures_node.child("angryPigHit").attribute("path").as_string());

	this->col = app->col->AddCollider(Enemybounds, Type::ENEMY, app->scene);
	
	return true;
}

bool WalkingEnemy::PreUpdate()
{
	if (app->map->WorldToMap(app->scene->gameScene->froggy->position.x, app->scene->gameScene->froggy->position.y) != lastPlayerPos)
	{
		 pathFindingA(app->scene->gameScene->peppa->pos, app->scene->gameScene->froggy->position);
	}
	lastPlayerPos = app->map->WorldToMap(app->scene->gameScene->froggy->position.x, app->scene->gameScene->froggy->position.y);

	return true;
}

bool WalkingEnemy::Update(float dt)
{
	Currentenemyanimation->Update();
	

	const DynArray<iPoint>* path = GetLastPath();

	if ( cont < path->Count())
	{
		iPoint pos = app->map->MapToWorld(path->At(cont)->x, path->At(cont)->y);
		/*int magnitude = pos.Module();*/
		
		if (pos.x >= this->pos.x && canMoveDir[RIGHT])
		{
			this->pos.x += speed;
		}
		else if(pos.x <= this->pos.x && canMoveDir[LEFT])
		{
			this->pos.x -= speed;
		}
		/*else if (pos.y <= this->pos.y && canMoveDir[UP])
		{
			this->pos.y -= speed;
		}*/
		/*if (canMoveDir[DOWN])
		{
			this->pos.y += speed;
		}*/

		//this->pos = pos;
		

		cont++;
	}
	else 
	{
		cont = 0;

	}
	col->SetPos(pos);

	for (int i = 0; i < 4; i++)
	{
		canMoveDir[i] = true;
	}
	
	return true;
}

bool WalkingEnemy::PostUpdate()
{
	EnemyRect = &Currentenemyanimation->GetCurrentFrame();

	iPoint tempPos = pos;

	app->render->DrawTexture(enemytextures[0], tempPos.x, tempPos.y, EnemyRect);

	if (app->scene->gameScene->debugTiles)
	{

		Enemybounds.x = pos.x;
		Enemybounds.y = pos.y;
		app->render->DrawRectangle(Enemybounds, 255, 200, 255, 80);
		std::cout << " EnemyBounds:" << pos.x << "," << pos.y << endl;
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
	

	return true;
}

void WalkingEnemy::OnCollision(Collider* col)
{
	
	
}

void WalkingEnemy::WillCollision()
{
	Enemy::WillCollision();
}

iPoint WalkingEnemy::GetCenterEnemyPos()
{
	iPoint pos;
	pos.x = this->pos.x + (Enemybounds.w / 2);
	pos.y = this->pos.y + (Enemybounds.h / 2);
	return pos;
}
