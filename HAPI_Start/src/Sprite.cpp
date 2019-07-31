#include "Sprite.h"
#include "Texture.h"
#include <assert.h>

Sprite::Sprite(Texture& texture)
	: m_tileSheet(texture),
	m_position(),
	m_originalSize(texture.getTileSize().x, texture.getTileSize().y),
	m_size(m_originalSize),
	m_tileID(static_cast<int>(eTileID::INVALID))
{}

Sprite::Sprite(Texture& texture, Vector2f startingPosition, int tileID)
	: m_tileSheet(texture),
	m_position(startingPosition),
	m_originalSize(texture.getTileSize().x, texture.getTileSize().y),
	m_size(m_originalSize),
	m_tileID(tileID)
{}

int Sprite::getID() const
{
	return m_tileID;
}

Vector2i Sprite::getOriginalSize() const
{
	return m_originalSize;
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
	return m_tileSheet.get();
}

bool Sprite::isAlpha() const
{
	return m_tileSheet.get().getFrame(m_tileID).alpha;
}

Frame Sprite::getFrame() const
{
	return m_tileSheet.get().getFrame(m_tileID);
}

Rectangle Sprite::getFrameRect() const
{
	return m_tileSheet.get().getFrameRect(m_tileID);
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
	m_size = size;
}