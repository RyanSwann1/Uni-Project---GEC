#include "Global.h"
#include "HAPI_lib.h"
#include "Utilities/XMLParser.h"
#include <iostream>
#include "Window.h"
#include "Texture.h"
#include "Sprite.h"
#include "Level.h"
#include <string>

//BYTE 1 byte, 8 bits
//WORD 2 bytes, 16 bits
//DWORD 4 bytes, 32 bits
//memset – sets all bytes in a block of memory to a provided value
//memcpy – copies bytes from one memory address to another

//https://en.cppreference.com/w/cpp/language/delete
//https://en.cppreference.com/w/cpp/language/rule_of_three

constexpr int PLAYER_STARTING_SCORE = 5;

void HAPI_Main()
{
	std::unique_ptr<Window> window = Window::create();
	if (!window)
	{
		std::cout << "Failed to create window\n";
		return;
	}

	Textures::getInstance().loadAllTextures();

	std::unique_ptr<Level> level = Level::loadLevel("mapThree.tmx");
	if (!level)
	{
		std::cout << "Couldn't load level\n";
		return;
	}

	auto& mouseData = HAPI.GetMouseData();
	Sprite mouseRectSprite(Vector2i(), 15);
	Vector2i mouseRectPosition;

	float frameStart = HAPI.GetTime();
	float lastFrameStart = HAPI.GetTime();
	float deltaTime = 0;

	int playerScore = PLAYER_STARTING_SCORE;
	const std::string scoreText("Player Score: ");

	int tileSize = Textures::getInstance().getTexture().getTileSize();
	while (HAPI.Update())
	{	
		frameStart = HAPI.GetTime();
		mouseRectPosition.x = (mouseData.x / tileSize) * tileSize;
		mouseRectPosition.y = (mouseData.y / tileSize) * tileSize;
		mouseRectSprite.setPosition(mouseRectPosition);

		if (mouseData.leftButtonDown)
		{
			level->addTurretAtPosition(mouseRectPosition, TurretType::Cannon, playerScore);
		}

		deltaTime = static_cast<float>(frameStart - lastFrameStart) / 1000.f;
		level->update(deltaTime, playerScore);

		window->clearToBlack();
		level->render(*window);
		window->render(mouseRectSprite);

		HAPI.RenderText(500, 50, HAPISPACE::HAPI_TColour::WHITE, scoreText + std::to_string(playerScore), 26);

		if (level->isEnded())
		{
			level = Level::loadLevel("mapThree.tmx");
			playerScore = PLAYER_STARTING_SCORE;
		}

		lastFrameStart = frameStart;
	}
}