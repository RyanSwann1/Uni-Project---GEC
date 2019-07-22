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
	if (position.x >= 0 &&
		position.x + size.x < m_size.x &&
		position.y >= 0 &&
		position.y + size.y < m_size.y)
	{
		return true;
	}
	else
	{
		return false;
	}
	//return position.x >= 0 &&
	//	position.x + size.x < m_size.x &&
	//	position.y >= 0 &&
	//	position.y + size.y < m_size.y;
}

bool Window::isSpritePartiallyContained(Rectangle windowRect, Rectangle textureRect) const
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

void Window::render(const Sprite & sprite)
{
	if (isSpriteFullyContained(sprite.position, sprite.size))
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

void Window::blit(const Sprite& sprite)
{
	HAPISPACE::BYTE* screenPnter = m_window + (sprite.position.x * sprite.position.y) * BYTES_PER_PIXEL;
	Frame frame = sprite.getFrame();
	int offset = (sprite.getFrame().x * sprite.getFrame().y) * BYTES_PER_PIXEL;
	HAPISPACE::BYTE* texturePnter = sprite.texture.getTexture() + offset;
	Rectangle frameRect = sprite.getFrameRect();

	for (int y = 0; y < frameRect.getHeight(); ++y)
	{
		memcpy(screenPnter, texturePnter, frameRect.getWidth() * BYTES_PER_PIXEL);

		// Move texture pointer to next line
		texturePnter += sprite.texture.getSize().x * BYTES_PER_PIXEL;
		// Move screen pointer to next line
		screenPnter += m_size.x * BYTES_PER_PIXEL;
	}

	//for (int y = 0; y < sprite.texture.getSize().y; y++)
	//{
	//	memcpy(screenPnter, texturePnter, sprite.texture.getSize().x * BYTES_PER_PIXEL);
	//	// Move texture pointer to next line
	//	texturePnter += sprite.texture.getSize().x * BYTES_PER_PIXEL;
	//	// Move screen pointer to next line
	//	screenPnter += m_size.x * BYTES_PER_PIXEL;
	//}
}

void Window::blitAlpha(const Sprite & sprite)
{
	////if (x >= 0 && y >= 0 && x + m_FrameWidth <= windowWidth && y + m_FrameHeight <= windowHeight)
	//Rectangle windowRect(0, m_size.x, 0, m_size.y);
	//Rectangle textureRect(position.x, position.x + sprite.m_texture.getSize().x, 
	//	position.y, position.y + sprite.m_texture.getSize().y);
	//
	//if (!isSpritePartiallyContained(windowRect, textureRect))
	//{
	//	return;
	//}

	//textureRect.clipTo(windowRect);
	//int offset = (textureRect.m_left + textureRect.m_top * m_size.x) * BYTES_PER_PIXEL;

	//int textureOffset = (((textureRect.m_left - position.x)) +
	//	((textureRect.m_top - position.y)) * sprite.m_texture.getSize().x) * BYTES_PER_PIXEL;

	//HAPISPACE::BYTE* texturePointer = sprite.m_texture.getTexture() + textureOffset;
	//HAPISPACE::BYTE* screenPointer = HAPI.GetScreenPointer() + offset;

	//for (int y = textureRect.m_top; y < textureRect.m_bottom; y++)
	//{
	//	for (int x = textureRect.m_left; x < textureRect.m_right; x++)
	//	{
	//		if (texturePointer[3] == 255)
	//		{
	//			*(HAPISPACE::HAPI_TColour*)screenPointer = *(HAPISPACE::HAPI_TColour*)texturePointer;
	//		}
	//		else if (texturePointer[3] > 1)
	//		{
	//			screenPointer[0] = screenPointer[0] + ((texturePointer[3] * (texturePointer[0] - screenPointer[0])) >> 8);
	//			screenPointer[1] = screenPointer[1] + ((texturePointer[3] * (texturePointer[1] - screenPointer[1])) >> 8);
	//			screenPointer[2] = screenPointer[2] + ((texturePointer[3] * (texturePointer[2] - screenPointer[2])) >> 8);
	//		}

	//		screenPointer += BYTES_PER_PIXEL;
	//		texturePointer += BYTES_PER_PIXEL;
	//	}
	//	screenPointer += (windowRect.getWidth() - textureRect.getWidth()) * BYTES_PER_PIXEL;
	//	texturePointer += (sprite.m_texture.getSize().x - textureRect.getWidth()) * BYTES_PER_PIXEL;
	//}
}
