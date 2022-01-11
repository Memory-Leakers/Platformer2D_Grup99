#include "ModuleFonts.h"
#include "App.h"


ModuleFonts::ModuleFonts()
{
}

ModuleFonts::~ModuleFonts()
{
}

int ModuleFonts::Load(const char* texturePath, const char* characters, uint rows)
{
	int id = -1;

	if (texturePath == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = app->tex->Load(texturePath);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texturePath, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texturePath, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// TODO 1: Finish storing font data

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	font.totalLength = strlen(characters);
	font.columns = fonts[id].totalLength / rows;

	uint tex_w, tex_h;
	app->tex->GetSize(tex, tex_w, tex_h);
	font.char_w = tex_w / font.columns;
	font.char_h = tex_h / font.rows;

	LOG("Successfully loaded BMP font from %s", texturePath);

	return id;
}

void ModuleFonts::UnLoad(int fontIndex)
{
	if (fontIndex >= 0 && fontIndex < MAX_FONTS && fonts[fontIndex].texture != nullptr)
	{
		app->tex->UnLoad(fonts[fontIndex].texture);
		fonts[fontIndex].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", fontIndex);
	}
}

void ModuleFonts::BlitText(int x, int y, int fontIndex, const char* text) const
{
	if (text == nullptr || fontIndex < 0 || fontIndex >= MAX_FONTS || fonts[fontIndex].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", fontIndex);
		return;
	}

	const Font* font = &fonts[fontIndex];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		// TODO 2: Find the character in the table and its position in the texture, then Blit
		uint charIndex = 0;

		// Find the location of the current character in the lookup table
		for (uint j = 0; j < font->totalLength; ++j)
		{
			if (font->table[j] == text[i])
			{
				charIndex = j;
				break;
			}
		}

		// Retrieve the position of the current character in the sprite
		spriteRect.x = spriteRect.w * (charIndex % font->columns);
		spriteRect.y = spriteRect.h * (charIndex / font->columns);

		app->render->DrawTexture(font->texture, x, y, &spriteRect, 0.0f);

		// Advance the position where we blit the next character
		x += spriteRect.w;
	}
}
