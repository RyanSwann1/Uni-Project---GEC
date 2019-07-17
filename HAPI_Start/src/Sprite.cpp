#include "Sprite.h"
#include "Window.h"
#include "Texture.h"

Sprite::Sprite(Texture& texture, Vector2i startingSize)
	: m_texture(texture),
	m_size(startingSize)
{}

void Sprite::draw(Window & window, Vector2i position) const
{
	if (window.isSpriteFullyContained(position, m_size) && !m_texture.isAlpha())
	{
		window.blit(*this, position);
	}
	else
	{
		window.fastBlit(*this, position);
	}
}