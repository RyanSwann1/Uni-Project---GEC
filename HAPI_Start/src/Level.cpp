#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Window.h"

//Tile Layer
TileLayer::TileLayer(std::vector<std::vector<int>>&& tileData)
	: m_tileData(std::move(tileData))
{}

void TileLayer::render(Window & window, Vector2i levelSize, Texture& tileSheet) const 
{
	for (int y = 0; y < levelSize.y; ++y)
	{
		for (int x = 0; x < levelSize.x; ++x)
		{
			const int tileID = m_tileData[y][x];
			if (tileID >= 0)
			{
				Vector2i position(x * tileSheet.getTileSize(), y * tileSheet.getTileSize());
				Sprite sprite(tileSheet, position, tileID);
				window.render(sprite);
				return;
			}
		}
	}
}

//Level
Level::Level(Vector2i levelSize, int tileSize, const std::vector<TileLayer>& tileLayers, const std::vector<Vector2i>& entityPath)
	: m_levelSize(levelSize),
	m_tileSize(tileSize),
	m_tileLayers(tileLayers),
	m_entityPath(entityPath)
{}

void Level::render(Window & window, Texture& tileSheet)
{
	for (const auto& tileLayer : m_tileLayers)
	{
		tileLayer.render(window, m_levelSize, tileSheet);
	}
}