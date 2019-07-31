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
	: m_tileSheet(nullptr),
	m_frames(),
	m_textureSize(0, 0),
	m_columns(0),
	m_tileSize()
{}

Texture::~Texture()
{
	delete m_tileSheet;
}

Texture::Texture(Texture && other)
{
	m_tileSheet = other.m_tileSheet;
	m_frames = std::move(other.m_frames);
	m_textureSize = other.m_textureSize;
	m_columns = other.m_columns;
	m_tileSize = other.m_tileSize;

	other.m_tileSheet = nullptr;
}

Frame Texture::getFrame(int ID) const
{
	assert(ID < m_frames.size());
	return m_frames[ID];
}

Vector2i Texture::getTileSize() const
{
	return m_tileSize;
}

Rectangle Texture::getFrameRect(int tileID) const
{
	return Rectangle(tileID % m_columns * m_tileSize.x, m_tileSize.x, tileID / m_columns * m_tileSize.x, m_tileSize.y);
}

std::unique_ptr<Texture> Texture::load(const std::string& xmlFileName, const std::string& imageName,
	const std::string& textureFileName)
{
	Texture texture;
	if (texture.loadXMLTexture(xmlFileName, imageName, textureFileName))
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
	assert(m_tileSheet);
	return m_tileSheet;
}

Vector2i Texture::getSize() const
{
	return m_textureSize;
}

void Texture::loadInFrames()
{
	int rows = m_textureSize.y / m_tileSize.x;
	if (rows == 0)
	{
		m_frames.emplace_back(0, 0, true);
	}
	else
	{
		m_frames.reserve(static_cast<size_t>(rows * m_columns));
		for (int frameID = 0; frameID < rows * m_columns; ++frameID)
		{
			Rectangle frameRect = getFrameRect(frameID);
			bool frameHasAlpha = false;
			HAPISPACE::BYTE* currentPixel = m_tileSheet + frameID * m_tileSize.y * BYTES_PER_PIXEL;

			for (int i = frameID * m_tileSize.y; i < frameRect.getRight() * frameRect.getBottom() * BYTES_PER_PIXEL; i += BYTES_PER_PIXEL)
			{
				*currentPixel = m_tileSheet[i];

				if (currentPixel[3] < 255)
				{
					frameHasAlpha = true;
					break;
				}
			}

			m_frames.emplace_back(frameRect.m_left, frameRect.m_top, frameHasAlpha);
		}
	}
}

bool Texture::loadXMLTexture(const std::string & xmlFileName, const std::string& imageName,
	const std::string & textureFileName)
{
	XMLParser::parseTexture(m_tileSize, m_textureSize, m_columns, xmlFileName, imageName);
	if (!HAPI.LoadTexture(DATA_DIRECTORY + textureFileName, &m_tileSheet, m_textureSize.x, m_textureSize.y))
	{
		return false;
	}
	loadInFrames();
	
	return true;
}

bool Texture::loadTexture(const std::string & textureFileName)
{
	return HAPI.LoadTexture(DATA_DIRECTORY + textureFileName, &m_tileSheet, m_textureSize.x, m_textureSize.y);
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
	m_tileSheet = Texture::load("Textures.tmx", "tilesheet", "tilesheet.png");
	if (!m_tileSheet)
	{
		std::cout << "Tilesheet not loaded\n";
		return false;
	}

	m_healthBarTexture = Texture::load("Textures.tmx", "healthbar", "healthbar.png");
	if (!m_healthBarTexture)
	{
		std::cout << "Health bar texture not loaded.";
		return false;
	}

	m_texturesLoaded = true;
	return true;
}

Texture & Textures::getTileSheet()
{
	assert(m_tileSheet);
	return *m_tileSheet;
}

Texture & Textures::getHealthBar()
{
	assert(m_healthBarTexture);
	return *m_healthBarTexture;
}
