#include "Texture.h"
#include "Global.h"
#include <assert.h>

Texture::Texture()
	: m_texture(nullptr),
	m_width(0),
	m_height(0),
	m_alpha(false)
{}

bool Texture::load(const std::string & fileName)
{
	assert(!m_texture);

	bool textureLoaded = HAPI.LoadTexture(fileName, &m_texture, m_width, m_height);
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