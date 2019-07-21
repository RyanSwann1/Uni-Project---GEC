#include "Texture.h"
#include "Global.h"
#include <assert.h>
#include "Utilities/XMLParser.h"

//Frame Details
Frame::Frame(int y, int x, bool alpha)
	: y(y),
	x(x),
	alpha(alpha)
{}

//Texture
Texture::Texture()
	: m_texture(nullptr),
	m_textureSize(0, 0)
{}

Texture::~Texture()
{
	delete[] m_texture;
}

Rectangle Texture::getFrameRect(int tileID) const
{
	return Rectangle(tileID % m_columns * m_tileSize, m_tileSize, 
		tileID / m_columns * m_tileSize, m_tileSize);
}

bool Texture::isAlpha() const
{
	return m_alpha;
}

bool Texture::load(const std::string& xmlFileName, const std::string& textureFileName)
{
	assert(!m_texture);
	XMLParser::parseTexture(m_tileSize, m_textureSize, m_columns, xmlFileName);
	bool textureLoaded = HAPI.LoadTexture(DATA_DIRECTORY + textureFileName, &m_texture, m_textureSize.x, m_textureSize.y);
	loadInFrames();
	if (!textureLoaded)
	{
		return false;
	}

	//const HAPISPACE::BYTE* currentTexturePixel = m_texture;
	//for (int i = 0; i < (m_width * m_height * BYTES_PER_PIXEL); i += BYTES_PER_PIXEL)
	//{
	//	if (currentTexturePixel[3] < 255)
	//	{
	//		m_alpha = true;
	//	}
	//	
	//	currentTexturePixel += BYTES_PER_PIXEL;
	//}

	return true;
}

HAPISPACE::BYTE * Texture::getTexture()
{
	assert(m_texture);
	return m_texture;
}

Vector2i Texture::getSize() const
{
	return m_textureSize;
}

void Texture::loadInFrames()
{
	int rows = m_textureSize.y / m_tileSize;
	m_frames.reserve(static_cast<size_t>(rows * m_columns));
	for (int frameID = 0; frameID < rows * m_columns; ++frameID)
	{
		Rectangle frameRect = getFrameRect(frameID);
		bool frameHasAlpha = false;
		int offset = frameID * m_tileSize;
		HAPISPACE::BYTE* currentPixel = m_texture + offset;

		for (int i = frameID * m_tileSize; i < frameRect.getRight() * frameRect.getBottom() * BYTES_PER_PIXEL; i += BYTES_PER_PIXEL)
		{
			*currentPixel = m_texture[i];
			
			if (currentPixel[3] < 255)
			{
				frameHasAlpha = true;
			}
		}

		m_frames.emplace_back(frameRect.m_left, frameRect.m_top, frameHasAlpha);
	}
}

bool Texture::isFrameAlpha(int ID) const
{
	assert(ID < m_frames.size());
	return m_frames[ID].alpha;
}
