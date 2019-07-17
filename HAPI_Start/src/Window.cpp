#include "Window.h"
#include <assert.h>
#include "Sprite.h"
#include "Texture.h"

const Vector2i FPS_DISPLAY_POSITION{ 50, 50 };

Window::Window()
	: m_window(nullptr),
	m_size(640, 480)
{}

bool Window::isSpriteFullyContained(Vector2i position, Vector2i size) const
{
	return position.x > 0 &&
		position.x + size.x < m_size.x &&
		position.y > 0 &&
		position.y + size.y < m_size.y;
}

bool Window::isSpriteViewable(Rectangle windowRect, Rectangle textureRect) const
{
	return textureRect.m_right > 0 &&
		textureRect.m_left < m_size.x &&
		textureRect.m_top < 0 &&
		textureRect.m_bottom > m_size.y;
}

bool Window::init()
{
	assert(!m_window);
	return HAPI.Initialise(m_size.x, m_size.y, "HAPI_WINDOW", HAPISPACE::eDefaults);
	HAPI.SetShowFPS(true, FPS_DISPLAY_POSITION.x, FPS_DISPLAY_POSITION.y);
	m_window = HAPI.GetScreenPointer();
}

void Window::clearToBlack()
{
	for (int i = 0; i < m_size.x * m_size.y * BYTES_PER_PIXEL; i += BYTES_PER_PIXEL)
	{
		m_window[i + 0] = 0;
		m_window[i + 1] = 0;
		m_window[i + 2] = 0;
	}
}

void Window::blit(const Sprite& sprite)
{
	HAPISPACE::BYTE* screenPnter = m_window + (sprite.m_position.x + sprite.m_position.y * m_size.x) * BYTES_PER_PIXEL;
	HAPISPACE::BYTE* texturePnter = sprite.m_texture.getTexture();

	for (int y = 0; y < sprite.m_texture.getHeight(); y++)
	{
		memcpy(screenPnter, texturePnter, sprite.m_texture.getWidth() * BYTES_PER_PIXEL);
		// Move texture pointer to next line
		texturePnter += sprite.m_texture.getWidth() * BYTES_PER_PIXEL;
		// Move screen pointer to next line
		screenPnter += m_size.x * BYTES_PER_PIXEL;
	}
}