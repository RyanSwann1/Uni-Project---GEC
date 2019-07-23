#include "Sprite.h"
#include "Texture.h"
#include <assert.h>

Sprite::Sprite()
{
}

Sprite::Sprite(Texture & texture, Vector2i startingPosition, int tileID)
	: m_texture(&texture),
	m_position(startingPosition),
	m_size(texture.getTileSize(), texture.getTileSize()),
	m_tileID(tileID)
{}

Vector2i Sprite::getSize() const
{
	return m_size;
}

Vector2i Sprite::getPosition() const
{
	return m_position;
}

Texture & Sprite::getTexture() const
{
	assert(m_texture);
	return *m_texture;
}

bool Sprite::isAlpha() const
{
	return m_texture->getFrame(m_tileID).alpha;
}

Frame Sprite::getFrame() const
{
	assert(m_texture);
	return m_texture->getFrame(m_tileID);
}

Rectangle Sprite::getFrameRect() const
{
	assert(m_texture);
	return m_texture->getFrameRect(m_tileID);
}

void Sprite::setPosition(Vector2i position)
{
	m_position = position;
}