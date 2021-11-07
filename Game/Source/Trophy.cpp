#include "Trophy.h"
#include "App.h"

Trophy::Trophy(int posX, int posY)
{

	position.x = posX;
	position.y = posY;

	bounds.x = position.x +16;
	bounds.y = position.y +16;
	bounds.w = 32;
	bounds.h = 16;

	animPres.PushBack({ 448, 0 , 64, 64 });
	animPres.PushBack({ 384, 0 , 64, 64 });
	animPres.PushBack({ 320, 0 , 64, 64 });
	animPres.PushBack({ 256, 0 , 64, 64 });
	animPres.PushBack({ 192, 0 , 64, 64 });
	animPres.PushBack({ 128, 0 , 64, 64 });
	animPres.PushBack({ 64, 0 , 64, 64 });
	animPres.PushBack({ 0, 0 , 64, 64 });
	animPres.PushBack({ 64, 0 , 64, 64 });
	animPres.PushBack({ 128, 0 , 64, 64 });
	animPres.PushBack({ 192, 0 , 64, 64 });
	animPres.PushBack({ 256, 0 , 64, 64 });
	animPres.PushBack({ 320, 0 , 64, 64 });
	animPres.PushBack({ 384, 0 , 64, 64 });
	animPres.PushBack({ 448, 0 , 64, 64 });

	animPres.speed = 0.35;
	animPres.hasIdle = true;

}

Trophy::~Trophy()
{

}

bool Trophy::Start()
{
	tex = app->tex->Load("Assets/Items/Checkpoints/End/End.png");

	this->col = app->col->AddCollider(bounds, Type::PICKER, app->scene);

	return true;
}

bool Trophy::PreUpdate()
{

	return true;
}

bool Trophy::Update(float dt)
{
	
	if (animPres.getCurrentFrameI() == 7)
	{
		return true;
	}
	else {
		animPres.Update();
	}
	
	return true;
}

bool Trophy::PostUpdate()
{
	rect = &animPres.GetCurrentFrame();

	app->render->DrawTexture(tex, position.x, position.y, rect);
	

	//Debug
	if (app->scene->gameScene->debugTiles)
	{
		app->render->DrawRectangle(bounds, 255, 255, 255, 80);
	}

	return true;
}

bool Trophy::CleanUp()
{

	return true;
}

void Trophy::OnCollision(Collider* col)
{
	if (collided) return;


	if (col->type == Type::PLAYER && !collided)
	{
		//animPres.hasIdle = false;
		collided = true;
	}
	else {
		collided = false;
	}
}

void Trophy::WillCollision()
{

}