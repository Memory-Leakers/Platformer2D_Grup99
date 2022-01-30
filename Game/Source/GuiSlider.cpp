#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include <string.h>

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int font) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;

	this->bounds.h = 4;
	this->bounds.w = 101;
	this->text = text;
	this->font = font;

	canClick = true;
	drawBasic = false;

	tex2 = app->tex->Load("Assets/Menu/GUI/Slider.png");
	tex = app->tex->Load("Assets/Menu/GUI/SliderBar.png");

	anim.PushBack({ 0,5,102,9 });
	anim2.PushBack({ 0,0,5,14 });
	anim2.PushBack({ 16,0,5,14 });
	anim2.loop = false;
	anim.loop = false;

	bar_rect = &anim.getFrame(0);
	sliderPos = this->bounds.x+this->bounds.w-1;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP
				&&
				state != GuiControlState::SELECTED)
			{
				NotifyObserver();
				app->audio->PlayFx(app->scene->buttonSFX, 0);
				state = GuiControlState::SELECTED; //Prevents double entry
			}
			else
			{
				state = GuiControlState::FOCUSED;
			}
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				sliderPos =mouseX - bounds.x;
				sliderPos += bounds.x;
			}
		}
		else state = GuiControlState::NORMAL;
	}

	return true;
}

bool GuiSlider::Draw(Render* render)
{

	if (app->scene->debugTiles)
	{
		switch (state)
		{
			case GuiControlState::DISABLED:
				render->DrawRectangle(bounds, 0, 0, 0, 0, true, false);
				break;
			case GuiControlState::NORMAL:
				render->DrawRectangle(bounds, 255, 0, 0, 255, true, false);
				break;
			case GuiControlState::FOCUSED:
				render->DrawRectangle(bounds, 255, 0, 0, 100, true, false);
				break;
			case GuiControlState::PRESSED:
				render->DrawRectangle(bounds, 255, 255, 255, 255, true, false);
				break;
			case GuiControlState::SELECTED:
				render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
				break;
		}
	}

	switch (state)
	{
		case GuiControlState::DISABLED:
			//render->DrawRectangle(bounds, 0, 0, 0, 0, true, false);
			break;
		case GuiControlState::NORMAL:
			rect = &anim2.getFrame(0);
			break;
		case GuiControlState::FOCUSED:
			rect = &anim2.getFrame(1);
			break;
	}

	//Draw Bar
	app->render->DrawTexture(tex, bounds.x+2, bounds.y, bar_rect, 1.0f, SDL_FLIP_HORIZONTAL, (0.0), 2147483647, 214783647, 1.0F, false);

	//DrawSliderButton
	app->render->DrawTexture(tex2, sliderPos, bounds.y-5, rect, 1.0f, SDL_FLIP_HORIZONTAL, (0.0), 2147483647, 214783647, 1.0F, false);

	if (app->scene->menuScene->settingsopened || app->scene->gameScene->settingopen)
	{
		aux = bounds.x;
	}
	else
	{
		aux = 10;
	}

	std::string s = std::to_string(sliderPos-aux);
	char const* value = s.c_str();
	//TEXT
	app->font->BlitText(bounds.x, bounds.y - 20, font, text.GetString());
	app->font->BlitText(bounds.x+110, bounds.y-5, font, value);

	return true;
}


bool GuiSlider::CleanUp()
{
	rect = nullptr;
	bar_rect = nullptr;
	SDL_DestroyTexture(tex);
	app->font->UnLoad(font);
	return true;
}
