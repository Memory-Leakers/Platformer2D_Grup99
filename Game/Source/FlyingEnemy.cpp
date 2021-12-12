#include "App.h"
#include "FlyingEnemy.h"

FlyingEnemy::FlyingEnemy(int x,int y)
{

	enemy_id = 2;

	Flying_Enemy_node = app->LoadWalkingEnemy(FlyingEnemyfile);

	Flying_Enemy_node = FlyingEnemyfile.child("enemy_state").child("enemy_fly");

	this->pos.x = x;
	this->pos.y = y;

	Enemybounds.x = this->pos.x;
	Enemybounds.y = this->pos.y;
	Enemybounds.w = 32;
	Enemybounds.h = 28;


	//SET OF ANIMATIONS

	//IDLE
	ghostIdle.PushBack({ 9,0,31,30 });
	ghostIdle.PushBack({ 53,0,31,30 });
	ghostIdle.PushBack({ 96,0,31,30 });
	ghostIdle.PushBack({ 140,0,31,30 });
	ghostIdle.PushBack({ 184,0,31,30 });
	ghostIdle.PushBack({ 229,0,31,30 });
	ghostIdle.PushBack({ 273,0,31,30 });
	ghostIdle.PushBack({ 318,0,31,30 });
	ghostIdle.PushBack({ 362,0,31,30 });
	ghostIdle.PushBack({ 406,0,31,30 });
	ghostIdle.speed = 0.20;
	ghostIdle.hasIdle = false;

	//HIT
	ghostHit.PushBack({ 8,3,31,28 });
	ghostHit.PushBack({ 52,3,31,28 });
	ghostHit.PushBack({ 96,3,31,28 });
	ghostHit.PushBack({ 141,3,31,28 });
	ghostHit.PushBack({ 185,3,31,28 });
	ghostHit.speed = 0.20;
	ghostHit.hasIdle = false;

	//Disappear
	ghostDisappear.PushBack({ 0,0,31,28 });
	ghostDisappear.PushBack({ 31,0,31,28 });
	ghostDisappear.PushBack({ 74,0,31,28 });
	ghostDisappear.speed = 0.30;
	ghostDisappear.hasIdle = false;
	ghostDisappear.loop = false;

	//Appear
	ghostAppear.PushBack({ 53,3,31,28 });
	ghostAppear.PushBack({ 93,3,31,28 });
	ghostAppear.PushBack({ 139,3,31,28 });
	ghostAppear.speed = 0.30;
	ghostAppear.hasIdle = false;
	ghostAppear.loop = false;

	Currentenemyanimation = &ghostIdle;


}

FlyingEnemy::~FlyingEnemy()
{
}

bool FlyingEnemy::Start()
{
	pugi::xml_node Flying_Enemy_textures_node = Flying_Enemy_node.child("textures");

	//IDLE
	enemytextures[0] = app->tex->Load(Flying_Enemy_textures_node.child("ghostIdle").attribute("path").as_string());
	//HIT
	enemytextures[1] = app->tex->Load(Flying_Enemy_textures_node.child("ghostHit").attribute("path").as_string());
	//DEATH
	enemytextures[2] = app->tex->Load(Flying_Enemy_textures_node.child("ghostDeath").attribute("path").as_string());
	//Appear
	enemytextures[3] = app->tex->Load(Flying_Enemy_textures_node.child("ghostAppear").attribute("path").as_string());

	this->col = app->col->AddCollider(Enemybounds, Type::ENEMY, app->scene);

	return true;
}

bool FlyingEnemy::PreUpdate()
{
	int xx = (pos.x - app->scene->gameScene->froggy->position.x);
	int yy = (pos.y - app->scene->gameScene->froggy->position.y);
	int posPDif = sqrt(xx * xx + yy * yy);

	if (posPDif > 200)
	{
		return true;
	}
	pathFindingA(pos, app->scene->gameScene->froggy->position);

	return true;
}

bool FlyingEnemy::Update(float dt)
{
	//WillCollision();

	


	const DynArray<iPoint>* path = GetLastPath();


	int pathCount = path->Count();

	if (cont < pathCount && pathCount <= flyingPathRange)
	{
		if (app->scene->gameScene->froggy->isFlip == isFlip)
		{
			return true;
		}
		
		iPoint pos;

		//Flickering prevention
		//if (path->Count() != 2)
		//{
		pos = app->map->MapToWorld(path->At(cont)->x, path->At(cont)->y);
		//}
		//else {
		//	 pos = app->map->MapToWorld(path->At(1)->x, path->At(1)->y);
		//}

		int posDifX = abs(pos.x + 8 - this->pos.x);

		//Movement
		if (posDifX >= 15)
		{
			if (pos.x > this->pos.x && canMoveDir[RIGHT])
			{
				this->pos.x += speed;
				isFlip = true;
			}
			else if (pos.x < this->pos.x && canMoveDir[LEFT])
			{
				this->pos.x -= speed;
				isFlip = false;
			}
		}
		int posDifY = abs(pos.y - this->pos.y);
		if (posDifY >= 15)
		{
			if (pos.y < this->pos.y && canMoveDir[UP])
			{
				this->pos.y -= speed;
			}
			else if (pos.y >= this->pos.y && canMoveDir[DOWN])
			{
 				this->pos.y += speed;
			}
			
			
		}
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

	Currentenemyanimation->Update();

	return false;
}

bool FlyingEnemy::PostUpdate()
{
	EnemyRect = &Currentenemyanimation->GetCurrentFrame();

	iPoint tempPos = pos;
	if (isFlip)
	{
		app->render->DrawTexture(enemytextures[0], tempPos.x, tempPos.y, EnemyRect, 1.0f, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		app->render->DrawTexture(enemytextures[0], tempPos.x, tempPos.y, EnemyRect);
	}
	if (app->scene->gameScene->debugTiles)
	{
		Enemybounds.x = pos.x;
		Enemybounds.y = pos.y;
		app->render->DrawRectangle(Enemybounds, 255, 200, 255, 80);
		std::cout << " EnemyBounds:" << pos.x << "," << pos.y << endl;


		//PATH
		const DynArray<iPoint>* path = GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawRectangle({ pos.x, pos.y, 16, 16 }, 0,155, 0, 80);
		}

	}
	return true;
}

bool FlyingEnemy::CleanUp()
{
	EnemyRect = nullptr;
	for (int i = 0; i < 3; i++)
	{
		SDL_DestroyTexture(enemytextures[i]);
		enemytextures[i] = nullptr;
	}
	Currentenemyanimation = nullptr;

	if (col != nullptr)
	{
		col->pendingToDelete = true;
	}
	FlyingEnemyfile.~xml_document();

	return false;
}

void FlyingEnemy::OnCollision(Collider* col)
{
}

iPoint FlyingEnemy::GetCenterEnemyPos()
{
	iPoint pos;
	pos.x = this->pos.x + (Enemybounds.w / 2);
	pos.y = this->pos.y + (Enemybounds.h / 2);
	return pos;
}
