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
#include "Window.h"
#include "Texture.h"
#include "Sprite.h"

/*
	rectangle texture
	rectangle screen
	if(screen.completelyContains(texture) && !alpha)
		fastBlit
	else
		blit
*/

//BYTE 1 byte, 8 bits
//WORD 2 bytes, 16 bits
//DWORD 4 bytes, 32 bits
//memset – sets all bytes in a block of memory to a provided value
//memcpy – copies bytes from one memory address to another

void HAPI_Main()
{
	Window window;
	if (!window.initialize())
	{
		std::cout << "Cannot initialize HAPI\n";
		return;
	}

	Texture texture1;
	if (!texture1.load("playerSprite.tga"))
	{
		std::cout << "Cannot load texture\n";
		return;
	}

	Sprite playerSprite(texture1, { 5, 5 });
	Vector2i playerPosition(100, 100);
	Vector2i moveSpeed(1, 1);


	auto& mouseData = HAPI.GetMouseData();

	while (HAPI.Update())
	{
		window.clearToBlack();

		const HAPISPACE::HAPI_TKeyboardData &keyData = HAPI.GetKeyboardData(); 
		if (keyData.scanCode[HK_LEFT])
		{
			playerPosition.x -= moveSpeed.x;
		}
		else if (keyData.scanCode[HK_RIGHT])
		{
			playerPosition.x += moveSpeed.x;
		}
		else if (keyData.scanCode[HK_UP])
		{
			playerPosition.y -= moveSpeed.y;
		}
		else if (keyData.scanCode[HK_DOWN])
		{
			playerPosition.y += moveSpeed.y;
		}

		playerSprite.draw(window, playerPosition);

		//render(texture, position, textureSize, windowSize);
		//render(playerTexture, playerTexturePosition, playerTextureSize, windowSize);
		//renderAlpha(playerTexture, position, playerTextureSize, windowSize);
		//renderAlpha(texture, position, textureSize, windowSize);
		//blit(playerTexture, windowSize, position, playerTextureSize);
		//renderByPixel(playerTexture, position, playerTextureSize, windowSize);
	}
}