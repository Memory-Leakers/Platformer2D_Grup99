#include "Coin.h"
#include "App.h"


Coin::Coin(int posX, int posY)
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
	app->scene->gameScene->froggy->playerScore += fruitPoints;

	pendingToDelete = true;
	col->pendingToDelete = true;
	return true;
}

void Coin::OnCollision(Collider* col)
{
	if (col->type == Type::PLAYER)
	{
		CleanUp();
	}
}

void Coin::WillCollision()
{
	cout << "What?" << endl;
}

void Coin::textureSelector()
{
	srand(time(0));

	int num;
	num = rand() % 7;

	switch (num)
	{
		case Apple:
			tex = app->tex->Load("Assets/Items/Fruits/Apple.png");
			break;
		case Bananas:
			tex = app->tex->Load("Assets/Items/Fruits/Bananas.png");
			break;
		case Cherries:
			tex = app->tex->Load("Assets/Items/Fruits/Cherries.png");
			break;
		case Kiwi:
			tex = app->tex->Load("Assets/Items/Fruits/Kiwi.png");
			break;
		case Melon:
			tex = app->tex->Load("Assets/Items/Fruits/Melon.png");
			break;
		case Orange:
			tex = app->tex->Load("Assets/Items/Fruits/Orange.png");
			break;
		case Pineapple:
			tex = app->tex->Load("Assets/Items/Fruits/Pineapple.png");
			break;
		case Strawberry:
			tex = app->tex->Load("Assets/Items/Fruits/Strawberry.png");
			break;
	}
}
