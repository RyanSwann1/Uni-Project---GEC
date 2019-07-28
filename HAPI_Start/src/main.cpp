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

//http://www.alanzucconi.com/2016/02/03/2d-rotations/
//https://gamedev.stackexchange.com/questions/14602/what-are-atan-and-atan2-used-for-in-games

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
	
	eGameDifficulty gameDifficulty = eGameDifficulty::NORMAL;
	std::string levelName = "mapOne.tmx";
	std::unique_ptr<Level> level = Level::loadLevel(levelName, gameDifficulty);
	if (!level)
	{
		std::cout << "Couldn't load level\n";
		return;
	}

	auto& mouseData = HAPI.GetMouseData();
	auto& keyboardData = HAPI.GetKeyboardData();
	Sprite mouseRectSprite(Vector2f(), static_cast<int>(eTileID::SELECTOR));
	Vector2f mouseRectPosition;

	float frameStart = HAPI.GetTime();
	float lastFrameStart = HAPI.GetTime();
	float deltaTime = 0;

	int playerScore = PLAYER_STARTING_SCORE;
	const std::string scoreText("Player Score: ");
	bool gamePaused = false;
	bool resetCurrentGame = false;

	int tileSize = Textures::getInstance().getTexture().getTileSize();
	while (HAPI.Update())
	{	
		//Handle Input
		if (keyboardData.scanCode['P'])
		{
			gamePaused = true;
		}
		else if (keyboardData.scanCode['U'])
		{
			gamePaused = false;
		}
		else if (keyboardData.scanCode['R'])
		{
			if (gamePaused)
			{
				resetCurrentGame = true;
			}
		}

		//Update 
		frameStart = HAPI.GetTime();
		if (!gamePaused)
		{
			mouseRectPosition.x = (mouseData.x / tileSize) * tileSize;
			mouseRectPosition.y = (mouseData.y / tileSize) * tileSize;
			mouseRectSprite.setPosition(mouseRectPosition);

			if (mouseData.leftButtonDown)
			{
				level->addTurretAtPosition(mouseRectPosition, eTurretType::Cannon, playerScore);
			}

			deltaTime = static_cast<float>(frameStart - lastFrameStart) / 1000.f;
			level->update(deltaTime, playerScore, gameDifficulty);
		}

		//Render
		window->clearToBlack();
		level->render(*window);
		window->render(mouseRectSprite);
		HAPI.RenderText(500, 50, HAPISPACE::HAPI_TColour::WHITE, scoreText + std::to_string(playerScore), 26);

		//Handle Events
		if (resetCurrentGame || level->isEnded())
		{
			resetCurrentGame = false;
			gamePaused = false;
			level = Level::loadLevel(levelName, gameDifficulty);
			playerScore = PLAYER_STARTING_SCORE;
		}

		lastFrameStart = frameStart;
	}
}