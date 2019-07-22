#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(Texture & texture, Vector2i startingPosition, int tileID)
	: texture(texture),
	position(startingPosition),
	size(texture.getTileSize(), texture.getTileSize()),
	tileID(tileID)
{}

Frame Sprite::getFrame() const
{
	return texture.getFrame(tileID);
}

Rectangle Sprite::getFrameRect() const
{
	return texture.getFrameRect(tileID);
}