#pragma once

#include "GuiControl.h"
#include "Point.h"
#include "SString.h"
#include "Animation.h"

class GuiSlider : public GuiControl
{
public:
	GuiSlider(uint32 id, SDL_Rect bounds, const char* text, int font);
	virtual ~GuiSlider();

	bool Update(float dt) override;
	bool Draw(Render* render) override;
	bool CleanUp() override;

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	int sliderPos = 0;
	int aux;

private:
	SDL_Rect* bar_rect = nullptr;

	SDL_Texture* tex2 = nullptr;
	Animation anim2;

	

	

	
};

