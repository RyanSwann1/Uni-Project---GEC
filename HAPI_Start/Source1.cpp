#include "Texture.h"

//constructer
Texture::Texture(const std::string &filepath, int numberOfFramesX, int numberOfFramesY) 
	: m_Filepath(filepath),
	m_NumberOfFramesX(numberOfFramesX), 
	m_NumberOfFramesY(numberOfFramesY), 
	m_FrameWidth(m_TextureWidth / numberOfFramesX), 
	m_FrameHeight(m_TextureHeight / numberOfFramesY)
{
}

//function to load texture
bool Texture::loadTexture()
{
	if (HAPI.LoadTexture(m_Filepath, &texture, m_TextureWidth, m_TextureHeight))
	{
		textureLoaded = true;
		m_FrameWidth = m_TextureWidth / m_NumberOfFramesX;
		m_FrameHeight = m_TextureHeight / m_NumberOfFramesY;
		return true;
	}
	else
	{
		HAPI.UserMessage("missing texture: " + m_Filepath, "failed to load texture");
		texture = new HAPISPACE::BYTE[m_TextureWidth * 4];
		HAPISPACE::BYTE *texturepnter = texture;
		for (int i = 0; i < m_TextureWidth; i++)
		{
			*(HAPISPACE::HAPI_TColour*)texturepnter = HAPISPACE::HAPI_TColour::HORRID_PINK;
			texturepnter += 4;
		}
		return true;
	}
}

//getter functions for width and height of texture
int Texture::getWidth() const
{
	return m_TextureWidth;
}

int Texture::getHeight() const
{
	return m_TextureHeight;
}

//render functions
void Texture::renderTexture(const int &windowWidth, const int &windowHeight, const int &x, const int &y, const int frameNumber)
{
	if (x >= 0 && y >= 0 && x + m_FrameWidth <= windowWidth && y + m_FrameHeight <= windowHeight)
	{
		HAPISPACE::BYTE *screen = HAPI.GetScreenPointer();
		HAPISPACE::BYTE *screenPnter = screen + (x + y * windowWidth) * 4;
		unsigned int frameOffsetX = 0;
		if (frameNumber % m_NumberOfFramesX != 0)
		{
			frameOffsetX = (frameNumber % m_NumberOfFramesX) - 1;
		}
		else
		{
			frameOffsetX = m_NumberOfFramesX - 1;
		}
		unsigned int frameOffsetY = 0;
		frameOffsetY = (int)std::floor((frameNumber - 1) / m_NumberOfFramesX);

		unsigned int texOffset = 0;
		if (textureLoaded)
		{
			texOffset = ((m_FrameWidth * frameOffsetX) + (m_FrameHeight * frameOffsetY) * m_TextureWidth) * 4;
		}
		HAPISPACE::BYTE *texturePnter = texture + texOffset;
		for (int y = 0; y < m_FrameHeight; y++)
		{
			memcpy(screenPnter, texturePnter, m_FrameWidth * 4);
			if (textureLoaded)
			{
				texturePnter += m_TextureWidth * 4;
			}
			screenPnter += windowWidth * 4;
		}
	}
	else
	{
		renderTextureByPixel(windowWidth, windowHeight, x, y, frameNumber);
	}
}

void Texture::renderTextureByPixel(const int &windowWidth, const int &windowHeight, const int &x, const int &y, const int frameNumber)
{
	Rectangle screenRect(0, windowWidth, 0, windowHeight);
	Rectangle texRect(x, x + m_FrameWidth, y, y + m_FrameHeight);
	if (!texRect.isOnScreen(screenRect))
	{
		return;
	}
	else
	{
		texRect.clipTo(screenRect);
		HAPISPACE::BYTE *screen = HAPI.GetScreenPointer();
		unsigned int offset = (texRect.left + texRect.top * screenRect.right) * 4;
		unsigned int frameOffsetX = 0;
		if (frameNumber % m_NumberOfFramesX != 0)
		{
			frameOffsetX = (frameNumber % m_NumberOfFramesX) - 1;
		}
		else
		{
			frameOffsetX = m_NumberOfFramesX - 1;
		}
		unsigned int frameOffsetY = 0;
		frameOffsetY = (int)std::floor((frameNumber - 1) / m_NumberOfFramesX);

		unsigned int texOffset = 0;
		if (textureLoaded)
		{
			texOffset = (((texRect.left - x) + m_FrameWidth * frameOffsetX) + ((texRect.top - y) + m_FrameHeight * frameOffsetY) * m_TextureWidth) * 4;
		}
		HAPISPACE::BYTE *texturepnter = texture + texOffset;
		HAPISPACE::BYTE *pnter = screen + offset;
		for (int gy = texRect.top; gy < texRect.bottom; gy++)
		{
			for (int gx = texRect.left; gx < texRect.right; gx++)
			{
				*(HAPISPACE::HAPI_TColour*)pnter = *(HAPISPACE::HAPI_TColour*)texturepnter;
				pnter += 4;
				if (textureLoaded)
				{
					texturepnter += 4;
				}
			}
			pnter += (screenRect.right - texRect.Width()) * 4;
			if (textureLoaded)
			{
				texturepnter += (m_TextureWidth - texRect.Width()) * 4;
			}
		}
	}
}

void Texture::renderTextureWithAlpha(const int &windowWidth, const int &windowHeight, const int &x, const int &y, const int frameNumber)
{
	Rectangle screenRect(0, windowWidth, 0, windowHeight);
	Rectangle texRect(x, x + m_FrameWidth, y, y + m_FrameHeight);
	if (!texRect.isOnScreen(screenRect))
	{
		return;
	}
	else
	{
		texRect.clipTo(screenRect);
		HAPISPACE::BYTE *screen = HAPI.GetScreenPointer();
		unsigned int offset = (texRect.left + texRect.top * screenRect.right) * 4;
		unsigned int frameOffsetX = 0;
		if (frameNumber % m_NumberOfFramesX != 0)
		{
			frameOffsetX = (frameNumber % m_NumberOfFramesX) - 1;
		}
		else
		{
			frameOffsetX = m_NumberOfFramesX - 1;
		}
		unsigned int frameOffsetY = 0;
		frameOffsetY = (int)std::floor((frameNumber - 1) / m_NumberOfFramesX);

		unsigned int texOffset = 0;
		if (textureLoaded)
		{
			texOffset = (((texRect.left - x) + m_FrameWidth * frameOffsetX) + ((texRect.top - y) + m_FrameHeight * frameOffsetY) * m_TextureWidth) * 4;
		}

		HAPISPACE::BYTE *texturepnter = texture + texOffset;
		HAPISPACE::BYTE *pnter = screen + offset;
		for (int gy = texRect.top; gy < texRect.bottom; gy++)
		{
			for (int gx = texRect.left; gx < texRect.right; gx++)
			{
				if (texturepnter[3] == 0)
				{

				}
				else if (texturepnter[3] == 255)
				{
					*(HAPISPACE::HAPI_TColour*)pnter = *(HAPISPACE::HAPI_TColour*)texturepnter;
				}
				else
				{
					/*
					HAPISPACE::BYTE red = texturepnter[0];
					HAPISPACE::BYTE green = texturepnter[1];
					HAPISPACE::BYTE blue = texturepnter[2];
					HAPISPACE::BYTE alpha = texturepnter[3];
					*/
					pnter[0] = pnter[0] + ((texturepnter[3] * (texturepnter[0] - pnter[0])) >> 8);
					pnter[1] = pnter[1] + ((texturepnter[3] * (texturepnter[1] - pnter[1])) >> 8);
					pnter[2] = pnter[2] + ((texturepnter[3] * (texturepnter[2] - pnter[2])) >> 8);
				}
				pnter += 4;
				if (textureLoaded)
				{
					texturepnter += 4;
				}
			}
			pnter += (screenRect.right - texRect.Width()) * 4;
			if (textureLoaded)
			{
				texturepnter += (m_TextureWidth - texRect.Width()) * 4;
			}
		}
	}
}

Texture::~Texture()
{
	delete[] texture;
}
