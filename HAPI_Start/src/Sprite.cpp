#include "Sprite.h"
#include "Window.h"
#include "Texture.h"

Sprite::Sprite(Texture& texture, Vector2i startingPosition, Vector2i startingSize)
	: m_texture(texture),
	m_position(startingPosition),
	m_size(startingSize)
{}

void Sprite::draw(Window & window) const
{
	if (window.isSpriteFullyContained(m_position, m_size) && !m_texture.containsAlpha())
	{
		window.blit(*this);
	}
	else
	{

	}
}
