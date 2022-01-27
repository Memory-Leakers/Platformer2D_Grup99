#include "GuiCheckbox.h"
#include "Render.h"
#include "App.h"

GuiCheckbox::GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text, int font) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;
	this->font = font;

	canClick = true;
	drawBasic = false;


	tex = app->tex->Load("Assets/Menu/GUI/Checkbox.png");

	anim.PushBack({ 0,0,16,16 });
	anim.PushBack({ 16,0,16,16 });
	anim.PushBack({ 32,0,16,16 });
	anim.PushBack({ 48,0,16,16 });
	anim.loop = false;

}

GuiCheckbox::~GuiCheckbox()
{

}

bool GuiCheckbox::Update(float dt)
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
				state = GuiControlState::SELECTED; //Prevents double entry
				active = !active;
			}
			else
			{
				state = GuiControlState::FOCUSED;
			}
		}
		else state = GuiControlState::NORMAL;
	}
	return false;
}

bool GuiCheckbox::Draw(Render* render)
{

	bounds.w = 8;
	bounds.h = 8;

	switch (state)
	{
		case GuiControlState::DISABLED:
			render->DrawRectangle(bounds, 0, 0, 0, 0);
			break;
		case GuiControlState::NORMAL:
			if (active)
			{
				//render->DrawRectangle(bounds, 0, 255, 0, 255);
				rect = &anim.getFrame(2);
			}
			else
			{
				//render->DrawRectangle(bounds, 255, 0, 0, 255);
				rect = &anim.getFrame(0);
			}
			
			break;
		case GuiControlState::FOCUSED:
			if (active)
			{
				//render->DrawRectangle(bounds, 0, 255, 0, 100);
				rect = &anim.getFrame(3);
			}
			else
			{
				//render->DrawRectangle(bounds, 255, 0, 0, 100);
				rect = &anim.getFrame(1);
			}
			break;
		case GuiControlState::PRESSED:
			//render->DrawRectangle(bounds, 255, 255, 255, 255);
			break;
		case GuiControlState::SELECTED:
			//render->DrawRectangle(bounds, 0, 255, 0, 255);
			break;
	}

	app->render->DrawTexture(tex, bounds.x - (bounds.w/2), bounds.y - (bounds.h/2), rect, 1.0f, SDL_FLIP_HORIZONTAL);

	//TEXT
	app->font->BlitText(bounds.x + bounds.w+ 5 , bounds.y, font, text.GetString());

	return false;
}

bool GuiCheckbox::CleanUp()
{
	rect = nullptr;
	SDL_DestroyTexture(tex);

	return true;
}