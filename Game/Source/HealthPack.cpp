#include "HealthPack.h"
#include "App.h"

HealthPack::HealthPack(int posX = 0, int posY = 0)
{
	pickable_id = PickID::HEARTH;

	position.x = posX;
	position.y = posY;

	bounds.x = posX;
	bounds.y = posY;
	bounds.w = 16;
	bounds.h = 16;

	anim.PushBack({ 0, 0, 16, 16 });
	anim.PushBack({ 16, 0, 16, 16 });
	anim.PushBack({ 32, 0, 16, 16 });
	anim.PushBack({ 48, 0, 16, 16 });
	anim.PushBack({ 32, 0, 16, 16 });
	anim.PushBack({ 16, 0, 16, 16 });
	anim.speed = 0.1;
	anim.hasIdle = false;
}

HealthPack::~HealthPack()
{

}

bool HealthPack::Start()
{
	this->tex = app->tex->Load("Assets/Items/Hearth.png");
	this->col = app->col->AddCollider(bounds, Type::PICKER, app->scene);

	return true;
}


bool HealthPack::PreUpdate()
{

	return true;
}


bool HealthPack::Update(float dt)
{
	if (col == NULL)
	{
		this->col = app->col->AddCollider(bounds, Type::PICKER, app->scene);
	}

	anim.Update();
	return true;
}


bool HealthPack::PostUpdate()
{
	rect = &anim.GetCurrentFrame();
	app->render->DrawTexture(tex, position.x, position.y, rect);
	//Debug
	if (app->scene->gameScene->debugTiles && col != NULL)
	{
		app->render->DrawRectangle(bounds, 113, 85, 84, 127);
	}
	return true;
}


bool HealthPack::CleanUp()
{
	pendingToDelete = true;

	if (col != nullptr)
	{
		this->col->pendingToDelete = true;
	}
	rect = nullptr;
	SDL_DestroyTexture(this->tex);
	tex = nullptr;

	position.~Point();

	return true;
}


void HealthPack::OnCollision(Collider* col)
{
	if (col->type == Type::PLAYER)
	{
		if (app->scene->gameScene->froggy->health < 3)
		{
			app->scene->gameScene->froggy->health += 1;
		}
		CleanUp();
	}
	
}

void HealthPack::WillCollision()
{


}
