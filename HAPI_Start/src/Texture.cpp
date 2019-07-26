#include "Texture.h"
#include "Global.h"
#include <assert.h>
#include "Utilities/XMLParser.h"

//Frame Details
Frame::Frame(int x, int y, bool alpha)
	: x(x),
	y(y),
	alpha(alpha)
{}

//Texture
Texture::Texture()
	: m_texture(nullptr),
	m_textureSize(0, 0)
{}

Texture::~Texture()
{
	delete m_texture;
}

Texture::Texture(Texture && other)
{
	m_texture = other.m_texture;
	m_frames = std::move(other.m_frames);
	m_textureSize = other.m_textureSize;
	m_columns = other.m_columns;
	m_tileSize = other.m_tileSize;

	other.m_texture = nullptr;
}

Frame Texture::getFrame(int ID) const
{
	assert(ID < m_frames.size());
	return m_frames[ID];
}

int Texture::getTileSize() const
{
	return m_tileSize;
}

Rectangle Texture::getFrameRect(int tileID) const
{
	return Rectangle(tileID % m_columns * m_tileSize, m_tileSize, tileID / m_columns * m_tileSize, m_tileSize);
}

std::unique_ptr<Texture> Texture::load(const std::string& xmlFileName, const std::string& textureFileName)
{
	Texture texture;
	if (texture.loadTexture(xmlFileName, textureFileName))
	{
		return std::make_unique<Texture>(std::move(texture));
	}
	else
	{
		return std::unique_ptr<Texture>();
	}
}

HAPISPACE::BYTE * Texture::getTexture() const 
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
		HAPISPACE::BYTE* currentPixel = m_texture + frameID * m_tileSize * BYTES_PER_PIXEL;

		for (int i = frameID * m_tileSize; i < frameRect.getRight() * frameRect.getBottom() * BYTES_PER_PIXEL; i += BYTES_PER_PIXEL)
		{
			*currentPixel = m_texture[i];

			if (currentPixel[3] < 255)
			{
				frameHasAlpha = true;
				break;
			}
		}

		m_frames.emplace_back(frameRect.m_left, frameRect.m_top, frameHasAlpha);
	}
}

bool Texture::loadTexture(const std::string & xmlFileName, const std::string & textureFileName)
{
	XMLParser::parseTexture(m_tileSize, m_textureSize, m_columns, xmlFileName);
	if (!HAPI.LoadTexture(DATA_DIRECTORY + textureFileName, &m_texture, m_textureSize.x, m_textureSize.y))
	{
		return false;
	}
	loadInFrames();
	
	return true;
}

bool Texture::isFrameAlpha(int ID) const
{
	assert(ID < m_frames.size());
	return m_frames[ID].alpha;
}

//Textures
bool Textures::loadAllTextures()
{
	assert(!m_texturesLoaded);
	m_texture = std::move(Texture::load("mapOne.tmx", "tilesheet.png"));
	if (!m_texture)
	{
		std::cout << "Tilesheet not loaded\n";
		return false;
	}

	m_texturesLoaded = true;
	return true;
}

Texture & Textures::getTexture()
{
	assert(m_texture);
	return *m_texture;
}
