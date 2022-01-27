#pragma once
#include "GuiControl.h"
#include "Point.h"
#include "SString.h"
#include "Animation.h"


class GuiCheckbox : public GuiControl
{
public:
	GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text, int font);
	virtual ~GuiCheckbox();

	bool Update(float dt) override;
	bool Draw(Render* render) override;
	bool CleanUp() override;

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	bool active = false;

private:
	SDL_Texture* tex = nullptr;
	Animation anim;

	SDL_Rect* rect = nullptr;

};

