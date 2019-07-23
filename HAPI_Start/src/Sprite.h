#pragma once

#include "Global.h"

class Texture;
struct Frame;
class Sprite
{
public:
	Sprite(Texture& tileSheet);
	Sprite(Texture& tileSheet, Vector2i startingPosition, int tileID);

	Vector2i getSize() const;
	Vector2i getPosition() const;
	Texture& getTexture() const;
	bool isAlpha() const;
	Frame getFrame() const;
	Rectangle getFrameRect() const;

	void setID(int tileID);
	void setPosition(Vector2i position);

private:
	Texture* m_texture;
	Vector2i m_position;
	Vector2i m_size;
	int m_tileID;
};