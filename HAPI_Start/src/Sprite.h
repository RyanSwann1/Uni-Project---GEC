#pragma once

#include "Global.h"
#include <functional>

class Texture;
struct Frame;
class Sprite
{
public:
	Sprite();
	Sprite(Vector2i startingPosition, int tileID);

	Vector2i getSize() const;
	Vector2i getPosition() const;
	bool isAlpha() const;
	Frame getFrame() const;
	Rectangle getFrameRect() const;
	Texture& getTexture() const;

	void setID(int tileID);
	void setPosition(Vector2i position);

private:
	std::reference_wrapper<Texture> m_texture;
	Vector2i m_position;
	Vector2i m_size;
	int m_tileID;
};