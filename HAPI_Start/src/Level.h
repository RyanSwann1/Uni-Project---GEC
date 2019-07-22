#pragma once

#include <vector>
#include "Global.h"

class Texture;
class Window;
struct TileLayer
{
	TileLayer(std::vector<std::vector<int>>&& tileData);

	void render(Window& window, Vector2i levelSize, Texture& tileSheet) const;

	std::vector<std::vector<int>> m_tileData;
};

class Level
{
public:
	Level(Vector2i levelSize, int tileSize, const std::vector<TileLayer>& tileLayers, const std::vector<Vector2i>& entityPath);

	void render(Window& window, Texture& tileSheet);

private:
	std::vector<TileLayer> m_tileLayers;
	std::vector<Vector2i> m_entityPath;
	Vector2i m_levelSize;
	int m_tileSize;
};