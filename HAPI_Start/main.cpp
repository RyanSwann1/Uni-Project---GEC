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
#include <HAPI_lib.h>
#include <iostream>

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

void blit(HAPISPACE::BYTE* screen, HAPISPACE::BYTE* texture, Vector2i windowSize, Vector2i position, Vector2i textureSize)
{
	HAPISPACE::BYTE* screenPnter = screen + (position.x + position.y * windowSize.x) * BYTES_PER_PIXEL;
	HAPISPACE::BYTE* texturePnter = texture;

	for (int y = 0; y < textureSize.y; y++)
	{
		memcpy(screenPnter, texturePnter, textureSize.x * BYTES_PER_PIXEL);
		// Move texture pointer to next line
		texturePnter += textureSize.x * BYTES_PER_PIXEL;
		// Move screen pointer to next line
		screenPnter += windowSize.x * BYTES_PER_PIXEL;
	}
}

//BYTE 1 byte, 8 bits
//WORD 2 bytes, 16 bits
//DWORD 4 bytes, 32 bits
//memset – sets all bytes in a block of memory to a provided value
//memcpy – copies bytes from one memory address to another

void HAPI_Main()
{
	Vector2i textureSize;
	Vector2i windowSize(640, 480);
	HAPISPACE::BYTE* texture = nullptr;
	HAPISPACE::BYTE* screen = nullptr;
	if (!HAPI.Initialise(windowSize.x, windowSize.y, "HAPI_WINDOW", HAPISPACE::eDefaults))
	{
		std::cout << "Cannot initialize HAPI\n";
		return;
	}

	if (!HAPI.LoadTexture(DATA_DIRECTORY + "playerSprite.tga", &texture, textureSize.x, textureSize.y))
	{
		std::cout << "Couldn't load texture\n";
	}


	while (HAPI.Update())
	{
		clearScreenToBlack(windowSize);

		blit(HAPI.GetScreenPointer(), texture, windowSize, { 100, 100 }, textureSize);
	}

	delete[] texture;
}