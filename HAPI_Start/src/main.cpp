/*
	HAPI Start
	----------
	This solution contains an already set up HAPI project and this main file

	The directory structure and main files are:

	HAPI_Start - contains the Visual Studio solution file (.sln)
		HAPI_Start - contains the Visual Studio HAPI_APP project file (.vcxproj) and source code
			HAPI - the directory with all the HAPI library files
			Data - a place to put your data files with a few sample ones provided

	Additionally in the top directory there is a batch file for creating a redistributable Demo folder

	For help using HAPI:
	https://scm-intranet.tees.ac.uk/users/u0018197/hapi.html
*/

#include "Global.h"
#include "HAPI_lib.h"
#include "Utilities/XMLParser.h"
#include <iostream>

//void render()
//{
//	//if(isInScreen())
//	{
//		//ifAlpha()
//		{
//		//by pixel
//		}
		//else
		//{
		//fast blit() - line by line renderin
		//}
//
//	}
//}

/*
	rectangle texture
	rectangle screen
	if(screen.completelyContains(texture) && !alpha)
		fastBlit
	else
		blit
*/

void clearScreenToBlack(Vector2i windowSize)
{
	HAPISPACE::BYTE* screen = HAPI.GetScreenPointer();
	for (int i = 0; i < windowSize.x * windowSize.y * BYTES_PER_PIXEL; i += BYTES_PER_PIXEL)
	{
		screen[i + 0] = 0;
		screen[i + 1] = 0;
		screen[i + 2] = 0;
	}
}

void clipBlit(HAPISPACE::BYTE* destination, Rectangle destRect, HAPISPACE::BYTE* source, Rectangle sourceRect, int posX, int posY)
{
	//The next steps(see the lecture slides for more detail) are to translate the rectangle into screen space.This will require we have a Translate function in our rectangle class.

	//	// Translate to screen space

	//	clippedRect.Translate(posX, posY);

	//// Do the clipping

	//clippedRect.ClipTo(destRect);

	//// Translate back into source space

	//clippedRect.Translate(-posX, -posY);

	//One further step is to clamp the screen position to 0 in case of negative values e.g.

	//	if (posX < 0) posX = 0;

	//// TODO: same for posY
}

bool isViewable(Rectangle windowRect, Rectangle textureRect)
{
	if(textureRect.m_right > windowRect.m_left &&
		textureRect.m_left < windowRect.m_right &&
		textureRect.m_top < windowRect.m_bottom &&
		textureRect.m_bottom > windowRect.m_top)
	{
		return true;
	}

	return false;
}

void blit(HAPISPACE::BYTE* texture, Vector2i windowSize, Vector2i position, Vector2i textureSize)
{
	HAPISPACE::BYTE* screenPnter = HAPI.GetScreenPointer() + (position.x + position.y * windowSize.x) * BYTES_PER_PIXEL;
	HAPISPACE::BYTE* texturePnter = texture;

	for (int y = 0; y < textureSize.y; y++)
	{
		//BYTE alpha=texturePnter[3];
		HAPISPACE::BYTE alphaValue = texturePnter[3];

		memcpy(screenPnter, texturePnter, textureSize.x * BYTES_PER_PIXEL);
		// Move texture pointer to next line
		texturePnter += textureSize.x * BYTES_PER_PIXEL;
		// Move screen pointer to next line
		screenPnter += windowSize.x * BYTES_PER_PIXEL;
	}
}

void blitAlpha(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* texture, Vector2i windowSize, Vector2i position, Vector2i textureSize)
{
	HAPISPACE::BYTE* screenPnter = screen + (position.x + position.y * windowSize.x) * BYTES_PER_PIXEL;
	const int endOfLineScreenIncrement = (windowSize.x - textureSize.x) * 4;
	for (int y = 0; y < textureSize.y; ++y)
	{
		for (int x = 0; x < textureSize.x * 4; x += 4)
		{
			const HAPISPACE::BYTE blue = texture[0];
			const HAPISPACE::BYTE green = texture[1];   
			const HAPISPACE::BYTE red = texture[2];
			const HAPISPACE::BYTE alpha = texture[3];
			const float mod = alpha / 255;
			screen[0] = (HAPISPACE::BYTE)(mod*blue + (1.0f - mod) * screen[0]);
			screen[1] = (HAPISPACE::BYTE)(mod*green + (1.0f - mod) * screen[1]);
			screen[2] = (HAPISPACE::BYTE)(mod*red + (1.0f - mod) * screen[2]);

			screen += 4;
			texture += 4;
		}
		screen += endOfLineScreenIncrement;
	}
}

	//HAPISPACE::BYTE *screen = HAPI.GetScreenPointer();
	//HAPISPACE::BYTE *screenPnter = screen + (x + y * windowWidth) * 4;
	//unsigned int frameOffsetX = 0;
	//if (frameNumber % m_NumberOfFramesX != 0)
	//{
	//	frameOffsetX = (frameNumber % m_NumberOfFramesX) - 1;
	//}
	//else
	//{
	//	frameOffsetX = m_NumberOfFramesX - 1;
	//}
	//unsigned int frameOffsetY = 0;
	//frameOffsetY = (int)std::floor((frameNumber - 1) / m_NumberOfFramesX);
	//unsigned int texOffset = 0;
	//if (textureLoaded)
	//{
	//	texOffset = ((m_FrameWidth * frameOffsetX) + (m_FrameHeight * frameOffsetY) * m_TextureWidth) * 4;
	//}
	//HAPISPACE::BYTE *texturePnter = texture + texOffset;
	//for (int y = 0; y < m_FrameHeight; y++)
	//{
	//	memcpy(screenPnter, texturePnter, m_FrameWidth * 4);
	//	if (textureLoaded)
	//	{
	//		texturePnter += m_TextureWidth * 4;
	//	}
	//	screenPnter += windowWidth * 4;
	//}

void render(HAPISPACE::BYTE* texture, Vector2i position, Vector2i textureSize, Vector2i windowSize)
{
	//Is texture in bounds of screen
	if (position.x >= 0 && position.y >= 0 && position.x + textureSize.x <= windowSize.x &&
		position.y + textureSize.y <= windowSize.y)
	{
		HAPISPACE::BYTE* screenPointer = HAPI.GetScreenPointer() + 
			(position.x + position.y * windowSize.x) * BYTES_PER_PIXEL;

		HAPISPACE::BYTE* texturePointer = texture;
		for (int y = 0; y < textureSize.y; y++)
		{
			memcpy(screenPointer, texturePointer, textureSize.x * BYTES_PER_PIXEL);

			texturePointer += textureSize.x * BYTES_PER_PIXEL;
			screenPointer += windowSize.x * BYTES_PER_PIXEL;
		}
	}
	//Texture partially out of screen
	else
	{
	}
}
//
//void Texture::BlitAlpha(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const
//{
//	const BYTE* currentTexturePixel = m_texture;
//	int offset = ((int)area.Left + (m_width * (int)area.Top));
//	currentTexturePixel += offset * 4;
//	const Vector2i center_screen = screenSize / 2;
//
//	const int width = (int)area.Right - (int)area.Left;
//	const int height = (int)area.Bottom - (int)area.Top;
//
//	BYTE* currentScreenPixel = screen + (std::clamp(pos.x, 0, screenSize.x) + std::clamp(pos.y, 0, screenSize.y) * screenSize.x) * 4;
//	int endOfLineScreenIncrement = (screenSize.x - width) * 4;
//
//	for (int i = 0; i < (width*height * 4); i += 4)
//	{
//		if (i != 0 && i % (width * 4) == 0)
//		{
//			currentScreenPixel += endOfLineScreenIncrement;
//			//add the rest of the sprite width to get to the next line of the texture
//			currentTexturePixel += (m_width - width) * 4;
//		}
//
//		//Get screen and texture pointer
//		BYTE blue = currentTexturePixel[0];
//		BYTE green = currentTexturePixel[1];
//		BYTE red = currentTexturePixel[2];
//		BYTE alpha = currentTexturePixel[3];
//
//		if (alpha == (BYTE)0)
//		{
//			//do nothing
//		}
//		else if (alpha == (BYTE)255)
//		{
//			//set screen pixel to texture
//			currentScreenPixel[0] = currentTexturePixel[0];
//			currentScreenPixel[1] = currentTexturePixel[1];
//			currentScreenPixel[2] = currentTexturePixel[2];
//		}
//		else
//		{
//			currentScreenPixel[0] = currentScreenPixel[0] + ((alpha*(blue - currentScreenPixel[0])) >> 8);
//			currentScreenPixel[1] = currentScreenPixel[1] + ((alpha*(green - currentScreenPixel[1])) >> 8);
//			currentScreenPixel[2] = currentScreenPixel[2] + ((alpha*(red - currentScreenPixel[2])) >> 8);
//		}
//
//		currentScreenPixel += 4;
//		currentTexturePixel += 4;
//	}
//}

void renderByPixel(HAPISPACE::BYTE* texture, Vector2i position, Vector2i textureSize, Vector2i windowSize)
{
	//if (x >= 0 && y >= 0 && x + m_FrameWidth <= windowWidth && y + m_FrameHeight <= windowHeight)
	Rectangle windowRect(0, windowSize.x, 0, windowSize.y);
	Rectangle textureRect(position.x, position.x + textureSize.x, position.y, position.y + textureSize.y);
	if (!isViewable(windowRect, textureRect))
	{
		return;
	}

	textureRect.clipTo(windowRect);
	int offset = (textureRect.m_left + textureRect.m_top * windowSize.x) * BYTES_PER_PIXEL;

	int textureOffset = (((textureRect.m_left - position.x)) +
		((textureRect.m_top - position.y)) * textureSize.x) * BYTES_PER_PIXEL;

	HAPISPACE::BYTE* texturePointer = texture + textureOffset;
	HAPISPACE::BYTE* screenPointer = HAPI.GetScreenPointer() + offset;

	for (int y = textureRect.m_top; y < textureRect.m_bottom; y++)
	{
		for (int x = textureRect.m_left; x < textureRect.m_right; x++)
		{
			//if (texturePointer[3] == 255)
			//{
			//	*(HAPISPACE::HAPI_TColour*)screenPointer = *(HAPISPACE::HAPI_TColour*)texturePointer;
			//}
			//else if (texturePointer[3] == 0)
			//{

			//}
			//else
			//{
			//	//pnter[0] = pnter[0] + ((texturepnter[3] * (texturepnter[0] - pnter[0])) >> 8);
			//	//pnter[1] = pnter[1] + ((texturepnter[3] * (texturepnter[1] - pnter[1])) >> 8);
			//	//pnter[2] = pnter[2] + ((texturepnter[3] * (texturepnter[2] - pnter[2])) >> 8);

			//	screenPointer[0] = screenPointer[0] + ((texturePointer[3] * (texturePointer[0] - screenPointer[0])) >> 8);
			//	screenPointer[1] = screenPointer[1] + ((texturePointer[3] * (texturePointer[1] - screenPointer[1])) >> 8);
			//	screenPointer[2] = screenPointer[2] + ((texturePointer[3] * (texturePointer[2] - screenPointer[2])) >> 8);
			//}
			screenPointer += BYTES_PER_PIXEL;
			texturePointer += BYTES_PER_PIXEL;

			//pnter += (screenRect.right - texRect.Width()) * 4;
			//if (textureLoaded)
			//{
			//	texturepnter += (m_TextureWidth - texRect.Width()) * 4;
			//}


		}
		screenPointer += (windowRect.getWidth() - textureRect.getWidth()) * BYTES_PER_PIXEL;
		texturePointer += (textureSize.x - textureRect.getWidth()) * BYTES_PER_PIXEL;
	}
}

void renderAlpha(HAPISPACE::BYTE* texture, Vector2i position, Vector2i textureSize, Vector2i windowSize)
{
	//if (x >= 0 && y >= 0 && x + m_FrameWidth <= windowWidth && y + m_FrameHeight <= windowHeight)
	Rectangle windowRect(0, windowSize.x, 0, windowSize.y);
	Rectangle textureRect(position.x, position.x + textureSize.x, position.y, position.y + textureSize.y);
	if (!isViewable(windowRect, textureRect))
	{
		return;
	}

	textureRect.clipTo(windowRect);
	int offset = (textureRect.m_left + textureRect.m_top * windowSize.x) * BYTES_PER_PIXEL;

	int textureOffset = (((textureRect.m_left - position.x)) +
		((textureRect.m_top - position.y)) * textureSize.x) * BYTES_PER_PIXEL;

	HAPISPACE::BYTE* texturePointer = texture + textureOffset;
	HAPISPACE::BYTE* screenPointer = HAPI.GetScreenPointer() + offset;

	for (int y = textureRect.m_top; y < textureRect.m_bottom; y++)
	{
		for (int x = textureRect.m_left; x < textureRect.m_right; x++)
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
		screenPointer += (windowRect.getWidth() - textureRect.getWidth()) * BYTES_PER_PIXEL;
		texturePointer += (textureSize.x - textureRect.getWidth()) * BYTES_PER_PIXEL;
	}
}

//BYTE 1 byte, 8 bits
//WORD 2 bytes, 16 bits
//DWORD 4 bytes, 32 bits
//memset � sets all bytes in a block of memory to a provided value
//memcpy � copies bytes from one memory address to another

void HAPI_Main()
{

	Vector2i textureSize;
	Vector2i windowSize(800, 600);
	Vector2i moveSpeed(1, 1);
	Vector2i position(100, 100);
	HAPISPACE::BYTE* texture = nullptr;
	HAPISPACE::BYTE* playerTexture = nullptr;
	Vector2i playerTextureSize;
	Vector2i playerTexturePosition;
	HAPISPACE::BYTE* screen = nullptr;
	HAPI.SetShowFPS(true, 50, 50);

	if (!HAPI.Initialise(windowSize.x, windowSize.y, "HAPI_WINDOW", HAPISPACE::eDefaults))
	{
		std::cout << "Cannot initialize HAPI\n";
		return;
	}
	if (!HAPI.LoadTexture(DATA_DIRECTORY + "playerSprite.tga", &playerTexture, playerTextureSize.x, playerTextureSize.y))
	{
		std::cout << "Couldn't load texture\n";
	}

	if (!HAPI.LoadTexture(DATA_DIRECTORY + "Selector.PNG", &texture, textureSize.x, textureSize.y))
	{
		std::cout << "Couldn't load texture\n";
	}

	auto& mouseData = HAPI.GetMouseData();

	while (HAPI.Update())
	{
		clearScreenToBlack(windowSize);

		const HAPISPACE::HAPI_TKeyboardData &keyData = HAPI.GetKeyboardData(); 
		if (keyData.scanCode[HK_LEFT])
		{
			position.x -= moveSpeed.x;
		}
		else if (keyData.scanCode[HK_RIGHT])
		{
			position.x += moveSpeed.x;
		}
		else if (keyData.scanCode[HK_UP])
		{
			position.y -= moveSpeed.y;
		}
		else if (keyData.scanCode[HK_DOWN])
		{
			position.y += moveSpeed.y;
		}

		//render(texture, position, textureSize, windowSize);
		//render(playerTexture, playerTexturePosition, playerTextureSize, windowSize);
		//renderAlpha(playerTexture, position, playerTextureSize, windowSize);
		//renderAlpha(texture, position, textureSize, windowSize);
		blit(playerTexture, windowSize, position, playerTextureSize);
		renderByPixel(playerTexture, position, playerTextureSize, windowSize);
	}

	delete[] texture;
}