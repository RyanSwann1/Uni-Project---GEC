#pragma once

#include <string>
#include <vector>
#include <memory>

struct TileLayer;
struct Vector2i;
namespace XMLParser
{
	void parseTexture(int& tileSize, Vector2i& textureSize, int& columns, const std::string& fileName);

	bool parseLevel(const std::string& levelName, Vector2i& levelSize, std::vector<TileLayer>& tileLayers,
		std::vector<Vector2i>& entityPath, std::vector<Vector2i>& buildingPlacementPosition,
		int& soilderSpawnRate, int& tankSpawnRate, int& planeSpawnRate);
}