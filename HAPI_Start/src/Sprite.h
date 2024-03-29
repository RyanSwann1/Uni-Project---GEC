#pragma once

#include "Global.h"
#include <functional>

class Texture;
struct Frame;
class Sprite
{
public:
	Sprite(Texture& texture);
	Sprite(Texture& texture, Vector2f startingPosition, int tileID);

	int getID() const;
	Vector2i getOriginalSize() const;
	Vector2i getSize() const;
	Vector2f getPosition() const;
	bool isAlpha() const;
	Frame getFrame() const;
	Rectangle getFrameRect() const;
	Texture& getTexture() const;

	void setID(int tileID);
	void setPosition(Vector2f position);
	void setWidth(int width);

private:
	std::reference_wrapper<Texture> m_tileSheet;
	Vector2f m_position;
	Vector2i m_originalSize;
	Vector2i m_size;

	int m_tileID;
};