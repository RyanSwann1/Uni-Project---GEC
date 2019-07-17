#pragma once

#include "Global.h"

struct Texture;
struct Window;
struct Sprite
{
	Sprite(Texture& texture, Vector2i startingPosition, Vector2i startingSize);

	Rectangle getRect() const;
	void draw(Window& window) const;

	Texture& m_texture;
	Vector2i m_position;
	Vector2i m_size;
};