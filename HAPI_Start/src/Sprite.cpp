#include "Sprite.h"
#include "Texture.h"
#include <assert.h>

Sprite::Sprite()
	: m_texture(Textures::getInstance().getTexture()),
	m_position(),
	m_size(Textures::getInstance().getTexture().getTileSize(), Textures::getInstance().getTexture().getTileSize()),
	m_tileID(static_cast<int>(TileID::INVALID))
{}

Sprite::Sprite(Vector2i startingPosition, int tileID)
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

Vector2i Sprite::getPosition() const
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

void Sprite::setPosition(Vector2i position)
{
	m_position = position;
}