#pragma once

#include "Global.h"
#include <functional>

class Texture;
struct Frame;
class Sprite
{
public:
	Sprite();
	Sprite(Vector2f startingPosition, int tileID);

	int getID() const;
	Vector2i getSize() const;
	Vector2f getPosition() const;
	bool isAlpha() const;
	Frame getFrame() const;
	Rectangle getFrameRect() const;
	Texture& getTexture() const;

	void setID(int tileID);
	void setPosition(Vector2f position);
	void setSize(Vector2i size);

private:
	std::reference_wrapper<Texture> m_texture;
	Vector2f m_position;
	Vector2i m_size;
	int m_tileID;
};