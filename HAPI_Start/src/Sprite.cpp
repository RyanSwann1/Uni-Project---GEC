#include "Sprite.h"
#include "Texture.h"
#include <assert.h>

Sprite::Sprite(Texture& texture)
	: m_texture(texture),
	m_position(),
	m_size(texture.getTileSize(), texture.getTileSize()),
	m_tileID(static_cast<int>(eTileID::INVALID))
{}

Sprite::Sprite(Texture& texture, Vector2f startingPosition, int tileID)
	: m_texture(Textures::getInstance().getTexture()),
	m_position(startingPosition),
	m_size(Textures::getInstance().getTexture().getTileSize(), Textures::getInstance().getTexture().getTileSize()),
	m_tileID(tileID)
{}

int Sprite::getID() const
{
	return m_tileID;
}

Vector2i Sprite::getSize() const
{
	return m_size;
}

Vector2f Sprite::getPosition() const
{
	return m_position;
}

Texture & Sprite::getTexture() const
{
	return m_texture.get();
}

bool Sprite::isAlpha() const
{
	return m_texture.get().getFrame(m_tileID).alpha;
}

Frame Sprite::getFrame() const
{
	return m_texture.get().getFrame(m_tileID);
}

Rectangle Sprite::getFrameRect() const
{
	return m_texture.get().getFrameRect(m_tileID);
}

void Sprite::setID(int tileID)
{
	m_tileID = tileID;
}

void Sprite::setPosition(Vector2f position)
{
	m_position = position;
}

void Sprite::setSize(Vector2i size)
{
	assert(size.x <= m_texture.get().getTileSize());
	assert(size.y <= m_texture.get().getTileSize());
	m_size = size;
}