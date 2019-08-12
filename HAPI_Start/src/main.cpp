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
const std::string MAP_ONE_NAME = "mapOne.tmx";
const std::string MAP_TWO_NAME = "mapTwo.tmx";
const std::string MAP_THREE_NAME = "mapThree.tmx";

void HAPI_Main()
{
	std::unique_ptr<Window> window = Window::create();
	if (!window)
	{
		std::cerr << "Failed to create window\n";
		return;
	}

	if (!Textures::getInstance().loadAllTextures())
	{
		std::cerr << "Couldn't load all textures\n";
		return;
	}
	
	eGameDifficulty gameDifficulty = eGameDifficulty::NORMAL;
	std::string levelName = "mapTwo.tmx";
	std::unique_ptr<Level> level;

	auto& mouseData = HAPI.GetMouseData();
	auto& keyboardData = HAPI.GetKeyboardData();
	Sprite mouseRectSprite(Textures::getInstance().getTileSheet(), Vector2f(), static_cast<int>(eTileID::SELECTOR));
	Vector2f mouseRectPosition;

	float frameStart = HAPI.GetTime();
	float lastFrameStart = HAPI.GetTime();
	float deltaTime = 0;

	int playerScore = PLAYER_STARTING_SCORE;
	const std::string scoreText("Player Score: ");
	bool gamePaused = false;
	bool resetCurrentGame = false;
	bool mainMenuActive = true;
	bool difficultySelected = false;
	const std::string levelOneText = "Level one: 'u'";
	const std::string levelTwoText = "Level two: 'i'";
	const std::string levelThreeText = "Level three: 'o'";

	const std::string difficultyEasyText = "Normal: 'h'";
	const std::string difficultyMediumText = "Hard: 'j'";
	const std::string difficultyHardText = "Extreme: 'k'";

	Vector2i tileSize = Textures::getInstance().getTileSheet().getTileSize();
	while (HAPI.Update())
	{
		if (!mainMenuActive)
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
		}

		//Difficulty Selection
		else if (keyboardData.scanCode['H'] && !difficultySelected)
		{
			difficultySelected = true;
			gameDifficulty = eGameDifficulty::NORMAL;
		}
		else if (keyboardData.scanCode['J'] && !difficultySelected)
		{
			difficultySelected = true;
			gameDifficulty = eGameDifficulty::HARD;
		}
		else if (keyboardData.scanCode['K'] && !difficultySelected)
		{
			difficultySelected = true;
			gameDifficulty = eGameDifficulty::EXTREME;
		}
		//Level Selection
		else if (keyboardData.scanCode['U'] && difficultySelected)
		{
			mainMenuActive = false;
			levelName = MAP_ONE_NAME;
			level = Level::loadLevel(levelName, gameDifficulty);
			if (!level)
			{
				std::cerr << "Couldn't load level\n";
				return;
			}
		}
		else if (keyboardData.scanCode['I'] && difficultySelected)
		{
			mainMenuActive = false;
			levelName = MAP_TWO_NAME;
			level = Level::loadLevel(levelName, gameDifficulty);
			if (!level)
			{
				std::cerr << "Couldn't load level\n";
				return;
			}
		}
		else if (keyboardData.scanCode['O'] && difficultySelected)
		{
			mainMenuActive = false;
			levelName = MAP_THREE_NAME;
			level = Level::loadLevel(levelName, gameDifficulty);
			if (!level)
			{
				std::cerr << "Couldn't load level\n";
				return;
			}
		}

		


		//Update 
		frameStart = static_cast<float>(HAPI.GetTime());
		if (!gamePaused && !mainMenuActive)
		{
			mouseRectPosition.x = static_cast<float>((mouseData.x / tileSize.x)) * tileSize.x;
			mouseRectPosition.y = static_cast<float>((mouseData.y / tileSize.y)) * tileSize.y;
			mouseRectSprite.setPosition(mouseRectPosition);

			if (mouseData.leftButtonDown)
			{
				level->addTurretAtPosition(mouseRectPosition, eTurretType::Cannon, playerScore);
			}

			deltaTime = static_cast<float>(frameStart - lastFrameStart) / 1000.f;
			level->update(deltaTime, playerScore, gameDifficulty, window->getSize());
		}

		//Render
		window->clearToBlack();
		if (mainMenuActive)
		{
			level->render(*window);
			window->render(mouseRectSprite);
		}
		
		//ui
		HAPI.RenderText(500, 50, HAPISPACE::HAPI_TColour::WHITE, scoreText + std::to_string(playerScore), 26);
		if (mainMenuActive)
		{
			if (difficultySelected)
			{
				//Display level Select
				HAPI.RenderText(350, 250, HAPISPACE::HAPI_TColour::WHITE, levelOneText, 26);
				HAPI.RenderText(350, 350, HAPISPACE::HAPI_TColour::WHITE, levelTwoText, 26);
				HAPI.RenderText(350, 450, HAPISPACE::HAPI_TColour::WHITE, levelThreeText, 26);
			}
			else
			{
				//Display level Select
				HAPI.RenderText(350, 250, HAPISPACE::HAPI_TColour::WHITE, difficultyEasyText, 26);
				HAPI.RenderText(350, 350, HAPISPACE::HAPI_TColour::WHITE, difficultyMediumText, 26);
				HAPI.RenderText(350, 450, HAPISPACE::HAPI_TColour::WHITE, difficultyHardText, 26);
			}

		}

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