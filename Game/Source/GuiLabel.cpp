#include "GuiLabel.h"
#include "Render.h"
#include "App.h"

GuiLabel::GuiLabel(uint32 id, SDL_Rect bounds, const char* text, int font) : GuiControl(GuiControlType::LABEL, id)
{
	this->bounds = bounds;
	this->text = text;
	this->font = font;

}

GuiLabel::~GuiLabel()
{

}

bool GuiLabel::Update(float dt)
{

	return true;
}

bool GuiLabel::Draw(Render* render)
{
	//TEXT
	app->font->BlitText(bounds.x + (bounds.w / 4), bounds.y + (bounds.h / 4), font, text.GetString());
	return true;
}