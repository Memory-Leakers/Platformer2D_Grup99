#include "App.h"
#include "Trap.h"


Trap::Trap(int x, int y, InteractablesId trapDirection)
{
    id = trapDirection;
   
    position.x = x;
    position.y = y;

    int dif = 9;
    switch (id)
    {
        case InteractablesId::SPIKETRAP_TOP:
            bounds.x = x;
            bounds.y = y + dif;
            bounds.w = 16;
            bounds.h = dif;
            break;
        case InteractablesId::SPIKETRAP_BOTTOM:
            bounds.x = x;
            bounds.y = y;
            bounds.w = 16;
            bounds.h = dif;
            break;
        case InteractablesId::SPIKETRAP_RIGHT:
            bounds.x = x;
            bounds.y = y;
            bounds.w = 8;
            bounds.h = 15;
            break;
        case InteractablesId::SPIKETRAP_LEFT:
            bounds.x = x + dif;
            bounds.y = y;
            bounds.w = dif;
            bounds.h = 15;
            break;
    }

    rect = new SDL_Rect({ 0, 0, 16, 16 });
}

Trap::~Trap()
{

}


bool Trap::Start()
{
    if (id == InteractablesId::SPIKETRAP_TOP || id == InteractablesId::SPIKETRAP_BOTTOM)
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
    switch (id)
    {
    case InteractablesId::SPIKETRAP_TOP:
        app->render->DrawTexture(tex, position.x, position.y, rect);
        break;
    case InteractablesId::SPIKETRAP_BOTTOM:
        app->render->DrawTexture(tex, position.x, position.y, rect, 1.0F, SDL_FLIP_VERTICAL);
        break;
    case InteractablesId::SPIKETRAP_RIGHT:
        app->render->DrawTexture(tex, position.x, position.y, rect);
        break;
    case InteractablesId::SPIKETRAP_LEFT:
        app->render->DrawTexture(tex, position.x, position.y, rect, 1.0f, SDL_FLIP_HORIZONTAL);
        break;

    }


    //Debug
    if (app->scene->debugTiles)
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