#include "GUI.h"
#include "App.h"

GUI::GUI(SString tex, iPoint pos, int frames, SDL_Rect bounds, int frameSize, bool camRelative)
{
	this->tex = app->tex->Load(tex.GetString());
	this->positionOnScreen = pos;
	this->camRelative = camRelative;
	this->frames = frames;
	this->bounds = bounds;
	this->frameSize = frameSize;

	if (camRelative) this->positionOnScreen = pos;
	else this->position = pos;

}

bool GUI::Start()
{
	//LoadAnims
	for (int x = 0, f = 0; frames != f; f++ ,x+=frameSize)
	{
		if(frameOnX) anim.PushBack({ x , 0, bounds.w, bounds.h });
		else anim.PushBack({ 0 , x, bounds.w, bounds.h});
	}

	anim.speed = 0;
	anim.hasIdle = false;

	return true;
}

bool GUI::Update()
{
	if (camRelative)
	{
		int scale = app->win->GetScale();

		position.x = -app->render->camera.x/scale + positionOnScreen.x;
		position.y = -app->render->camera.y/scale + positionOnScreen.y;
	}

	

	return true;
}

bool GUI::PostUpdate()
{
	if (type == FRAME_FOLLOW)
	{
		guiRect = &anim.getFrame(*frame_value);
	}
	else {
		guiRect = &anim.GetCurrentFrame();
	}

	if (guiRect != nullptr)
	{
		std::cout << guiRect->x << ", " << guiRect->y << ", " << guiRect->w << ", " << guiRect->h << std::endl;
		app->render->DrawTexture(tex, position.x, position.y, guiRect, 1.0F, SDL_FLIP_NONE, 0.0, 2147483647, 2147483647, texScale);
	}

	return true;
}