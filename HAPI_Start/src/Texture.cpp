#include "Texture.h"
#include "Global.h"
#include <assert.h>

Texture::Texture()
	: m_texture(nullptr),
	m_width(0),
	m_height(0),
	m_alpha(false)
{}

bool Texture::containsAlpha() const
{
	return m_alpha;
}

bool Texture::load(const std::string & fileName)
{
	assert(!m_texture);

	bool textureLoaded = HAPI.LoadTexture(DATA_DIRECTORY + fileName, &m_texture, m_width, m_height);
	if (!textureLoaded)
	{
		return false;
	}

	const HAPISPACE::BYTE* currentTexturePixel = m_texture;
	for (int i = 0; i < (m_width * m_height * BYTES_PER_PIXEL); i += BYTES_PER_PIXEL)
	{
		if (currentTexturePixel[3] < 255)
		{
			m_alpha = true;
		}
		
		currentTexturePixel += BYTES_PER_PIXEL;
	}

	return true;
}

HAPISPACE::BYTE * Texture::getTexture()
{
	assert(m_texture);
	return m_texture;
}

int Texture::getWidth() const
{
	return m_width;
}

int Texture::getHeight() const
{
	return m_height;
}
