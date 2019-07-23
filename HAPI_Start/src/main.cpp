#include "Global.h"
#include "HAPI_lib.h"
#include "Utilities/XMLParser.h"
#include <iostream>
#include "Window.h"
#include "Texture.h"
#include "Sprite.h"
#include "Level.h"

//BYTE 1 byte, 8 bits
//WORD 2 bytes, 16 bits
//DWORD 4 bytes, 32 bits
//memset – sets all bytes in a block of memory to a provided value
//memcpy – copies bytes from one memory address to another

//https://en.cppreference.com/w/cpp/language/delete
//https://en.cppreference.com/w/cpp/language/rule_of_three

void HAPI_Main()
{
	std::unique_ptr<Window> window = Window::create();
	if (!window)
	{
		std::cout << "Failed to create window\n";
		return;
	}

	std::unique_ptr<Texture> texture = Texture::load("mapOne.tmx", "tilesheet.png");
	if (!texture)
	{
		std::cout << "Tilesheet not loaded\n";
		return;
	}

	std::unique_ptr<Level> level = Level::loadLevel("mapOne.tmx", *texture);
	if (!level)
	{
		std::cout << "Couldn't load level\n";
		return;
	}

	auto& mouseData = HAPI.GetMouseData();
	Sprite mouseRectSprite(*texture, Vector2i(50, 50), 15);
	Vector2i mousePosition(mouseData.x, mouseData.y);
	Vector2i mouseRectPosition;
	const HAPISPACE::HAPI_TKeyboardData &keyData = HAPI.GetKeyboardData();

	while (HAPI.Update())
	{	
		mouseRectPosition.x = (mouseData.x / texture->getTileSize()) * texture->getTileSize();
		mouseRectPosition.y = (mouseData.y / texture->getTileSize()) * texture->getTileSize();
		mouseRectSprite.setPosition(mouseRectPosition);

		if (mouseData.leftButtonDown)
		{
			level->addTurretAtPosition(mouseRectPosition, TurretType::Cannon);
		}

		window->clearToBlack();
		level->render(*window, *texture);
		window->render(mouseRectSprite);
	}
}