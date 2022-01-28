#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, int font) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->font = font;
	this->id = id;

	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (id == 1 && app->scene->menuScene->savefiled == false)
	{
		state = GuiControlState::DISABLED;
	}
	if (state != GuiControlState::DISABLED)
	{
		// L14: TODO 3: Update the state of the GUiButton according to the mouse position
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);

		if ((mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{


			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{

				state = GuiControlState::PRESSED;
			}
			else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP &&
				state != GuiControlState::SELECTED)
			{// If mouse button pressed -> Generate event!
				NotifyObserver();
				state = GuiControlState::SELECTED; //Prevents double entry
			}
			else
			{
				state = GuiControlState::FOCUSED;
			}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
		
	}

	return false;
}

bool GuiButton::Draw(Render* render)
{
	
	// Draw the right button depending on state
	switch (state)
	{
		case GuiControlState::DISABLED:
			render->DrawRectangle(bounds, 0, 0, 0, 50);
			break;
		case GuiControlState::NORMAL:
			render->DrawRectangle(bounds, 255, 0, 0, 255);
			break;
		case GuiControlState::FOCUSED:
			render->DrawRectangle(bounds, 255, 255, 255, 160);
			break;
		case GuiControlState::PRESSED:
			render->DrawRectangle(bounds, 255, 255, 255, 255);
			break;
		case GuiControlState::SELECTED: 
			render->DrawRectangle(bounds, 0, 255, 0, 255);
			break;
	}

	//TEXT
	app->font->BlitText(bounds.x + (bounds.w/4), bounds.y + (bounds.h/4), font, text.GetString());

	return false;
}