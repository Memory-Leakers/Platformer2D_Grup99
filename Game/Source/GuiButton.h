#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"
#include "Point.h"
#include "SString.h"

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text, int font);
	virtual ~GuiButton();

	bool Update(float dt) override;
	bool Draw(Render* render) override;
	bool CleanUp() override;

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUIBUTTON_H__