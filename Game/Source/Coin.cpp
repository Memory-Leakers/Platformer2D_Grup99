#include "Coin.h"
#include "App.h"
#include "SDL_mixer/include/SDL_mixer.h"


Coin::Coin(int posX = 0, int posY = 0)
{
	position.x = posX;
	position.y = posY;

	bounds.x = posX+8;
	bounds.y = posY+8;
	bounds.w = 16;
	bounds.h = 16;

	anim.PushBack({ 0, 0, 32, 32 });
	anim.PushBack({ 32, 0, 32, 32 });
	anim.PushBack({ 64, 0, 32, 32 });
	anim.PushBack({ 96, 0, 32, 32 });
	anim.PushBack({ 128, 0, 32, 32 });
	anim.PushBack({ 160, 0, 32, 32 });
	anim.PushBack({ 192, 0, 32, 32 });
	anim.PushBack({ 224, 0, 32, 32 });
	anim.PushBack({ 256, 0, 32, 32 });
	anim.PushBack({ 288, 0, 32, 32 });
	anim.PushBack({ 320, 0, 32, 32 });
	anim.PushBack({ 352, 0, 32, 32 });
	anim.PushBack({ 384, 0, 32, 32 });
	anim.PushBack({ 416, 0, 32, 32 });
	anim.PushBack({ 448, 0, 32, 32 });
	anim.PushBack({ 480, 0, 32, 32 });
	anim.PushBack({ 512, 0, 32, 32 });
	anim.speed = 0.3;
	anim.hasIdle = false;
}

Coin::~Coin()
{

}

bool Coin::Start()
{
	textureSelector();

	this->col = app->col->AddCollider(bounds, Type::PICKER, app->scene);

	return true;
}

bool Coin::PreUpdate()
{


	return true;
}

bool Coin::Update(float dt)
{

	anim.Update();

	return true;
}

bool Coin::PostUpdate()
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

bool Coin::CleanUp()
{
	pendingToDelete = true;

	if (col != nullptr)
	{
		this->col->pendingToDelete = true;
	}
	rect = nullptr;
	SDL_DestroyTexture(this->tex);
	tex = nullptr;

	return true;
}

void Coin::OnCollision(Collider* col)
{
	if (col->type == Type::PLAYER)
	{
		app->scene->gameScene->froggy->playerScore += fruitPoints;
		app->audio->PlayFx(app->scene->gameScene->pickupSFX, 0);
		CleanUp();
	}
}

void Coin::WillCollision()
{
	cout << "What?" << endl;
}

void Coin::textureSelector()
{
	int num = rand() % 7;
	switch (num)
	{
		case Apple:
			this->tex = app->tex->Load("Assets/Items/Fruits/Apple.png");
			break;
		case Bananas:
			this->tex = app->tex->Load("Assets/Items/Fruits/Bananas.png");
			break;
		case Cherries:
			this->tex = app->tex->Load("Assets/Items/Fruits/Cherries.png");
			break;
		case Kiwi:
			this->tex = app->tex->Load("Assets/Items/Fruits/Kiwi.png");
			break;
		case Melon:
			this->tex = app->tex->Load("Assets/Items/Fruits/Melon.png");
			break;
		case Orange:
			this->tex = app->tex->Load("Assets/Items/Fruits/Orange.png");
			break;
		case Pineapple:
			this->tex = app->tex->Load("Assets/Items/Fruits/Pineapple.png");
			break;
		case Strawberry:
			this->tex = app->tex->Load("Assets/Items/Fruits/Strawberry.png");
	}
}
