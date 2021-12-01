#include "GUI.h"
#include "App.h"

GUI::GUI(SDL_Texture* tex, iPoint pos, int frames, SDL_Rect bounds, int frameSize, bool camRelative = true)
{
	this->tex = tex;
	this->position = pos;
	this->camRelative = camRelative;
	this->frames = frames;
	this->bounds = bounds;
	this->frameSize = frameSize;
}

bool GUI::Start()
{
	//LoadAnims
	for (int x = 0; bounds.x > x; x+=frameSize)
	{
		anim.PushBack({ x , bounds.h, bounds.w, bounds.h });
	}

	anim.speed = 0;
	anim.hasIdle = false;

	return true;
}

bool GUI::Update()
{
	if (type == FRAME_FOLLOW)
	{
		anim.getFrame(*frame_value);
	}
}

bool GUI::PostUpdate()
{
	

	guiRect = &anim.GetCurrentFrame();

	app->render->DrawTexture(tex, position.x, position.y, guiRect);
}