#include "Window.h"
#include <assert.h>
#include "Sprite.h"
#include "Texture.h"

const Vector2i FPS_DISPLAY_POSITION{ 50, 50 };

Window::Window()
	: m_window(nullptr),
	m_size(1000, 1000)
{}

bool Window::isSpriteFullyContained(const Sprite& sprite) const
{
	return sprite.position.x >= 0 &&
		sprite.position.x + sprite.size.x < m_size.x &&
		sprite.position.y >= 0 &&
		sprite.position.y + sprite.size.y < m_size.y;
}

bool Window::isSpriteViewable(Rectangle windowRect, Rectangle textureRect) const
{
	return textureRect.m_right > windowRect.m_left &&
		textureRect.m_left < windowRect.m_right &&
		textureRect.m_top < windowRect.m_bottom &&
		textureRect.m_bottom > windowRect.m_top;
}

bool Window::initialize()
{
	assert(!m_window);
	if (!HAPI.Initialise(m_size.x, m_size.y, "HAPI_WINDOW", HAPISPACE::eDefaults))
	{
		return false;
	}
	HAPI.SetShowFPS(true, FPS_DISPLAY_POSITION.x, FPS_DISPLAY_POSITION.y);
	m_window = HAPI.GetScreenPointer();
	
	return true;
}

void Window::render(const Sprite & sprite) const
{
	if (isSpriteFullyContained(sprite) && !sprite.isAlpha())
	{
		blit(sprite);
	}
	else
	{
		blitAlpha(sprite);
	}
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

void Window::blit(const Sprite& sprite) const
{
	HAPISPACE::BYTE* screenPnter = m_window + (sprite.position.x + (sprite.position.y * m_size.x)) * BYTES_PER_PIXEL;
	int textureOffset = (sprite.getFrame().x + (sprite.texture.getSize().x) * sprite.getFrame().y) * BYTES_PER_PIXEL;
	HAPISPACE::BYTE* texturePnter = sprite.texture.getTexture() + textureOffset;
	Rectangle frameRect = sprite.getFrameRect();

	for (int y = 0; y < frameRect.getHeight(); ++y)
	{
		memcpy(screenPnter, texturePnter, frameRect.getWidth() * BYTES_PER_PIXEL);

		// Move texture pointer to next line
		texturePnter += sprite.texture.getSize().x * BYTES_PER_PIXEL;
		// Move screen pointer to next line
		screenPnter += m_size.x * BYTES_PER_PIXEL;
	}
}

void Window::blitAlpha(const Sprite & sprite) const
{
	Rectangle windowRect(0, m_size.x, 0, m_size.y);
	Rectangle spriteRect(sprite.position.x, sprite.texture.getTileSize(), sprite.position.y, 
		sprite.texture.getTileSize());

	if (!spriteRect.intersect(windowRect))
	{
		return;
	}

	spriteRect.clipTo(windowRect);

	int textureOffset = (sprite.getFrame().x + (sprite.texture.getSize().x) * sprite.getFrame().y) * BYTES_PER_PIXEL;
	HAPISPACE::BYTE* texturePointer = sprite.texture.getTexture() + textureOffset;
	HAPISPACE::BYTE* screenPointer = m_window + (sprite.position.x + (sprite.position.y * m_size.x)) * BYTES_PER_PIXEL;

	for (int y = spriteRect.m_top; y < spriteRect.m_bottom; y++)
	{
		for (int x = spriteRect.m_left; x < spriteRect.m_right; x++)
		{
			if (texturePointer[3] == 255)
			{
				*(HAPISPACE::HAPI_TColour*)screenPointer = *(HAPISPACE::HAPI_TColour*)texturePointer;
			}
			else if (texturePointer[3] > 1)
			{
				screenPointer[0] = screenPointer[0] + ((texturePointer[3] * (texturePointer[0] - screenPointer[0])) >> 8);
				screenPointer[1] = screenPointer[1] + ((texturePointer[3] * (texturePointer[1] - screenPointer[1])) >> 8);
				screenPointer[2] = screenPointer[2] + ((texturePointer[3] * (texturePointer[2] - screenPointer[2])) >> 8);
			}

			screenPointer += BYTES_PER_PIXEL;
			texturePointer += BYTES_PER_PIXEL;
		}

		screenPointer += (windowRect.getWidth() - spriteRect.getWidth()) * BYTES_PER_PIXEL;
		texturePointer += (sprite.texture.getSize().x - spriteRect.getWidth()) * BYTES_PER_PIXEL;
	}
}
