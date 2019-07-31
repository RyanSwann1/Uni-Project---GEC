#pragma once

#include <string>
#include <vector>
#include <memory>

struct TileLayer;
struct Vector2i;
namespace XMLParser
{
	void parseTexture(Vector2i& tileSize, Vector2i& textureSize, int& columns, const std::string& fileName, const std::string& textureFileName);

	bool parseLevel(const std::string& levelName, Vector2i& levelSize, std::vector<TileLayer>& tileLayers,
		std::vector<Vector2i>& entityPath, std::vector<Vector2i>& buildingPlacementPosition,
		int& tankSpawnRate, int& planeSpawnRate);
}