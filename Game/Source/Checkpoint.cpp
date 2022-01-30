#include "Checkpoint.h"
#include "App.h"


Checkpoint::Checkpoint(int posX, int posY, CHECKSTATE state)
{
	if (state != BASE)
	{
		currentState = state;
		stateChanged = true;
	}

	id = InteractablesId::CHECKPOINT;
	id_type = 10;

	position.x = posX;
	position.y = posY;

	bounds.x = position.x;
	bounds.y = position.y;
	bounds.w = 16;
	bounds.h = 48;

	loadAnimBase();
	loadAnimActivation();
	loadAnimActivated();
}

Checkpoint::~Checkpoint()
{

}

bool Checkpoint::Start()
{
	stateMachine();
	this->col = app->col->AddCollider(bounds, Type::AREA, app->scene);

	btn_tex = app->tex->Load("Assets/Menu/GUI/E.png");
	btn.PushBack({0, 0, 16, 16 });
	btn.loop = false;

	return true;
}

bool Checkpoint::PreUpdate()
{
	stateMachine();

	return true;
}

bool Checkpoint::Update(float dt)
{
	switch (currentState)
	{
		case BASE:
			animBase.Update();
			break;
		case ACTIVATION:
			animActivation.Update();
			if (savePetition == SAVEPETITION_SAVE)
			{
				if (app->scene->gameScene->em.getPlayer()->playerScore > app->scene->highScoreI)
				{
					app->scene->highScoreI = app->scene->gameScene->em.getPlayer()->playerScore;
				}

				app->SaveGameRequest();
				app->scene->gameScene->checkPoint = true;

				savePetition = SAVEPETITION_BLOCK;
			}
			break;
		case ACTIVATED:
			animActivated.Update();
			//savePetition = SAVEPETITION_BLOCK;
			break;
	}

	if (btn_Switch)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			app->scene->gameScene->em.getOtherCheckpoint(this);
		}
	}


	return true;
}

bool Checkpoint::PostUpdate()
{
	switch (currentState)
	{
		case BASE:
			rect = &animBase.GetCurrentFrame();
			break;
		case ACTIVATION:
			rect = &animActivation.GetCurrentFrame();
			
			if (animActivation.getCurrentFrameI() == 0 && savePetition == SAVEPETITION_NONE)
			{//On first frame, save the game
				savePetition = SAVEPETITION_SAVE;

			}
			
			if (animActivation.getCurrentFrameI() == animActivation.size()-1)
			{//CHANGE STATE IF IT'S THE ANIMATION LAST FRAME
				currentState = ACTIVATED;
				stateChanged = true;
				
			}
			break;
		case ACTIVATED:
			rect = &animActivated.GetCurrentFrame();
			break;
	}


	if (btn_Switch)
	{
		btn_rect = &btn.getFrame(0);
		app->render->DrawTexture(btn_tex, position.x, position.y-20, btn_rect);
	}

	//DRAW
	app->render->DrawTexture(tex, position.x, position.y, rect);

	//Debug
	if (app->scene->debugTiles)
	{
		app->render->DrawRectangle(bounds, 255, 255, 255, 80);
	}

	btn_Switch = false;

	return true;
}

bool Checkpoint::CleanUp()
{
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(btn_tex);
	tex = nullptr;

	if (col != nullptr)
	{
		col->pendingToDelete = true;
	}

	rect = nullptr;

	return true;
}

void Checkpoint::OnCollision(Collider* col)
{
	if (col->type == Type::PLAYER && currentState != ACTIVATED)
	{
		currentState = ACTIVATION;
		stateChanged = true;
	}

	if (currentState == ACTIVATED)
	{
		btn_Switch = true;
	}
}

void Checkpoint::WillCollision() 
{
	SDL_DestroyTexture(tex);
	tex = nullptr;
	col->pendingToDelete = true;
}

void Checkpoint::stateMachine()
{
	if (stateChanged) {
		SDL_DestroyTexture(tex);
		tex = nullptr;
		switch (currentState)
		{
			case BASE:
				tex = app->tex->Load("Assets/Items/Checkpoints/Checkpoint/CheckpointBase.png");
				break;
			case ACTIVATION:
				tex = app->tex->Load("Assets/Items/Checkpoints/Checkpoint/CheckpointActivation.png");
				break;
			case ACTIVATED:
				tex = app->tex->Load("Assets/Items/Checkpoints/Checkpoint/CheckpointActivated.png");
				break;
		}
		stateChanged = false;
	}

}

void Checkpoint::loadAnimBase()
{
	animBase.PushBack({0, 0, 32, 48});
	animBase.speed = 0.35;
	animBase.hasIdle = true;
}

void Checkpoint::loadAnimActivation()
{
	animActivation.PushBack({ 32, 0, 32, 48 });
	animActivation.PushBack({ 64, 0, 32, 48 });
	animActivation.PushBack({ 96, 0, 32, 48 });
	animActivation.PushBack({ 128, 0, 32, 48 });
	animActivation.PushBack({ 160, 0, 32, 48 });
	animActivation.PushBack({ 192, 0, 32, 48 });
	animActivation.PushBack({ 224, 0, 32, 48 });
	animActivation.PushBack({ 256, 0, 32, 48 });
	animActivation.PushBack({ 288, 0, 32, 48 });
	animActivation.PushBack({ 320, 0, 32, 48 });
	animActivation.PushBack({ 352, 0, 32, 48 });
	animActivation.PushBack({ 384, 0, 32, 48 });
	animActivation.PushBack({ 416, 0, 32, 48 });
	animActivation.PushBack({ 448, 0, 32, 48 });
	animActivation.PushBack({ 480, 0, 32, 48 });
	animActivation.PushBack({ 512, 0, 32, 48 });
	animActivation.PushBack({ 544, 0, 32, 48 });
	animActivation.PushBack({ 576, 0, 32, 48 });
	animActivation.PushBack({ 608, 0, 32, 48 });
	animActivation.PushBack({ 640, 0, 32, 48 });
	animActivation.PushBack({ 672, 0, 32, 48 });
	animActivation.PushBack({ 704, 0, 32, 48 });
	animActivation.PushBack({ 736, 0, 32, 48 });
	animActivation.PushBack({ 768, 0, 32, 48 });
	animActivation.PushBack({ 800, 0, 32, 48 });

	animActivation.speed = 0.30;
	animActivation.hasIdle = false;
}

void Checkpoint::loadAnimActivated()
{
	animActivated.PushBack({ 0, 0, 48, 48});
	animActivated.PushBack({ 96, 0, 48, 48 });
	animActivated.PushBack({ 144, 0, 48, 48 });
	animActivated.PushBack({ 192, 0, 48, 48 });
	animActivated.PushBack({ 240, 0, 48, 48 });
	animActivated.PushBack({ 288, 0, 48, 48 });
	animActivated.PushBack({ 336, 0, 48, 48 });
	animActivated.PushBack({ 384, 0, 48, 48 });
	animActivated.PushBack({ 432, 0, 48, 48 });

	animActivated.speed = 0.30;
	animActivated.hasIdle = false;
}

void Checkpoint::loadData(pugi::xml_node node)
{
	
	//dirAnimActivated = Walking_Enemy_textures_node.child("angryPigHit").attribute("path").as_string();

}

void Checkpoint::Save(pugi::xml_node& data) const 
{
	pugi::xml_node i = data.child("interactables").append_child("i");
	i.append_attribute("posX") = position.x;
	i.append_attribute("posY") = position.y;
	i.append_attribute("id") = (int)id;
	i.append_attribute("state") = currentState;
}