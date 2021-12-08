#include "Trap.h"
#include "App.h"

Trap::Trap(int x, int y, TrapDirection trapDirection)
{
    pos.x = x;
    pos.y = y;

    direction = trapDirection;

    switch (trapDirection)
    {
        case TrapDirection::UP:
            bounds.x = x;
            bounds.y = y + 8;
            bounds.w = 16;
            bounds.h = 8;
            break;
        case TrapDirection::DOWN:
            bounds.x = x;
            bounds.y = y;
            bounds.w = 16;
            bounds.h = 8;
            break;
        case TrapDirection::RIGHT:
            bounds.x = x;
            bounds.y = y;
            bounds.w = 8;
            bounds.h = 16;
            break;
        case TrapDirection::LEFT:
            bounds.x = x + 8;
            bounds.y = y;
            bounds.w = 8;
            bounds.h = 16;
            break;

    }

    rect = new SDL_Rect({ 0, 0, 16, 16 });

}

Trap::~Trap()
{

}


bool Trap::Start()
{
    if (direction == TrapDirection::UP || direction == TrapDirection::DOWN)
    {
        tex = app->tex->Load("Assets/Traps/Spikes/Spike.png");
    }else
    {
        tex = app->tex->Load("Assets/Traps/Spikes/Spike2.png");
    }

    this->col = app->col->AddCollider(bounds, Type::TRAP, app->scene);

    return true;
}

bool Trap::PreUpdate()
{


    return true;
}

bool Trap::Update(float dt)
{
    return true;
}

bool Trap::PostUpdate()
{
    switch (direction)
    {
    case TrapDirection::UP:
        app->render->DrawTexture(tex, pos.x, pos.y, rect);
        break;
    case TrapDirection::DOWN:
        app->render->DrawTexture(tex, pos.x, pos.y, rect, 1.0F, SDL_FLIP_VERTICAL);
        break;
    case TrapDirection::RIGHT:
        app->render->DrawTexture(tex, pos.x, pos.y, rect);
        break;
    case TrapDirection::LEFT:
        app->render->DrawTexture(tex, pos.x, pos.y, rect, 1.0f, SDL_FLIP_HORIZONTAL);
        break;

    }

   

    //Debug
    if (app->scene->gameScene->debugTiles)
    {
        app->render->DrawRectangle(bounds, 0, 0, 0, 80);
    }

    return false;
}

bool Trap::CleanUp()
{
    if (col != nullptr)
	{
		this->col->pendingToDelete = true;
	}

    delete rect;
    rect = nullptr;
    SDL_DestroyTexture(this->tex);
    tex = nullptr;

    return true;
}

void Trap::OnCollision(Collider* col)
{

}

void Trap::WillCollision()
{
}