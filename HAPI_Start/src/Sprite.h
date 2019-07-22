#pragma once

#include "Global.h"

class Texture;
struct Frame;
struct Sprite
{
	Sprite(Texture& texture, Vector2i startingPosition, int tileID);

	bool isAlpha() const;
	Frame getFrame() const;
	Rectangle getFrameRect() const;

	Texture& texture;
	Vector2i position;
	Vector2i size;
	int tileID;
};