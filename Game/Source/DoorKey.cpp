#include "DoorKey.h"
#include "App.h"

DoorKey::DoorKey(int posX = 0, int posY = 0)
{
	position.x = posX;
	position.y = posY;

	bounds.x = posX + 8;
	bounds.y = posY + 8;
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

	pickId = PickType::DOORKEY;

}

DoorKey::~DoorKey()
{

}

bool DoorKey::Start()
{
	this->tex = app->tex->Load("Assets/Items/DoorKey.png");

	this->col = app->col->AddCollider(bounds, Type::PICKER, app->scene);
	
	return true;
}

bool DoorKey::PreUpdate()
{

	return true;
}

bool DoorKey::Update(float dt)
{
	anim.Update();

	return true;
}

bool DoorKey::PostUpdate()
{
	rect = &anim.GetCurrentFrame();

	app->render->DrawTexture(tex, position.x, position.y, rect);

	//Debug
	if (app->scene->gameScene->debugTiles)
	{
		app->render->DrawRectangle(bounds, 113, 85, 84, 127);
	}

	return true;
}

bool DoorKey::CleanUp()
{
	pendingToDelete = true;
	this->col->pendingToDelete = true;
	rect = nullptr;
	SDL_DestroyTexture(tex);
	tex = nullptr;

	return true;
}

void DoorKey::OnCollision(Collider* col) {
	if (col->type == Type::PLAYER)
	{
		app->scene->gameScene->key = true;
		app->audio->PlayFx(app->scene->gameScene->keypickupSFX, 0);
		CleanUp();
	}
}

void DoorKey::WillCollision()
{

}