#pragma once

#include <string>
#include <vector>

struct Vector2i;
struct Frame;
class Level;
namespace XMLParser
{
	void parseTexture(int& tileSize, Vector2i& textureSize, int& columns, const std::string& fileName);
	Level parseLevel(const std::string& levelName);
}