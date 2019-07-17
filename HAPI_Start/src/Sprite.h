#pragma once

#include "Global.h"

class Texture;
struct Window;
struct Sprite
{
	Sprite(Texture& texture, Vector2i startingSize);

	void draw(Window& window, Vector2i position) const;

	Texture& m_texture;
	Vector2i m_size;
};