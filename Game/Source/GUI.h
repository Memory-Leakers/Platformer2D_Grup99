#pragma once

#include "SDL/include/SDL.h"
#include "Animation.h"
#include "Point.h"
#include "DynArray.h"
#include "SString.h"


enum GUI_TYPE
{
	FRAME_FOLLOW = 0,
	TEXT_PRINTER
};

class GUI
{
public:
	GUI(SString tex, iPoint pos, int frames, SDL_Rect bounds, int frameSize, bool camRelative = true);

	~GUI();


	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

public:

	void setFrameFollow(int* value)
	{
		//frame_value = nullptr;
		
		frame_value = value;
		type = FRAME_FOLLOW;
	}

public:
	SDL_Texture* tex;
	Animation anim;
	float texScale = 1.0f;

	iPoint positionOnScreen;
	iPoint position;

	bool camRelative = true;

	int frames = 0;
	SDL_Rect bounds;
	SDL_Rect* guiRect;
	int frameSize = 0;
	bool frameOnX = true;

	GUI_TYPE type = FRAME_FOLLOW;

	//FRAME FOLLOW
	int* frame_value = 0;

};

