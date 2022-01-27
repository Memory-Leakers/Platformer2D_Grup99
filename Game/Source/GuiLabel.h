#pragma once

#include "GuiControl.h"
#include "Point.h"
#include "SString.h"


class GuiLabel : public GuiControl
{
public:
	GuiLabel(uint32 id, SDL_Rect bounds, const char* text, int font);
	virtual ~GuiLabel();

	bool Update(float dt) override;
	bool Draw(Render* render) override;


};

 