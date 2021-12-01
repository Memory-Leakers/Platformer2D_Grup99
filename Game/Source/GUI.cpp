#include "GUI.h"
#include "App.h"

GUI::GUI(SString tex, iPoint pos, int frames, SDL_Rect bounds, int frameSize, bool camRelative)
{
	this->tex = app->tex->Load(tex.GetString());
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

	return true;
}

bool GUI::PostUpdate()
{
	

	guiRect = &anim.GetCurrentFrame();

	app->render->DrawTexture(tex, position.x, position.y, guiRect);

	return true;
}