#include "XMLParser.h"
#include "Base64.h"
#include "tinyxml.h"
#include <assert.h>
#include <vector>
#include "../Level.h"
#include "../Texture.h"

std::vector<TileLayer> parseTileLayers(const TiXmlElement& rootElement, Vector2i levelSize);
std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, Vector2i levelSize);
std::vector<Vector2i> parseEntityPath(const TiXmlElement & rootElement, int tileSize);

void XMLParser::parseTexture(int& tileSize, Vector2i& textureSize, int& columns, const std::string& fileName)
{
	TiXmlDocument file;
	bool fileLoaded = file.LoadFile(DATA_DIRECTORY + fileName);
	assert(fileLoaded);

	const auto& rootElement = file.RootElement();
	for (const auto* tileSheetElement = rootElement->FirstChildElement();
		tileSheetElement != nullptr; tileSheetElement = tileSheetElement->NextSiblingElement())
	{
		if (tileSheetElement->Value() != std::string("tileset"))
		{
			continue;
		}

		tileSheetElement->FirstChildElement()->Attribute("width", &textureSize.x);
		tileSheetElement->FirstChildElement()->Attribute("height", &textureSize.y);
		tileSheetElement->Attribute("tilewidth", &tileSize);
		columns = textureSize.x / tileSize;
	}
}

Level XMLParser::parseLevel(const std::string& levelName)
{
	TiXmlDocument file;
	bool mapLoaded = file.LoadFile(DATA_DIRECTORY + levelName);
	assert(mapLoaded);

	const auto& rootElement = file.RootElement();
	int tileSize = 0;
	Vector2i levelSize;
	rootElement->Attribute("width", &levelSize.x);
	rootElement->Attribute("height", &levelSize.y);
	rootElement->Attribute("tilewidth", &tileSize);

	return Level(levelSize, tileSize, parseTileLayers(*rootElement, levelSize), parseEntityPath(*rootElement, tileSize));
}

std::vector<Vector2i> parseEntityPath(const TiXmlElement & rootElement, int tileSize)
{
	std::vector<Vector2i> entityPath;
	for (const auto* entityElementRoot = rootElement.FirstChildElement(); entityElementRoot != nullptr; entityElementRoot = entityElementRoot->NextSiblingElement())
	{
		if (entityElementRoot->Value() != std::string("objectgroup") || entityElementRoot->Attribute("name") != std::string("Entity Layer"))
		{
			continue;
		}

		for (const auto* entityElement = entityElementRoot->FirstChildElement(); entityElement != nullptr; entityElement = entityElement->NextSiblingElement())
		{
			Vector2i startingPosition;
			entityElement->Attribute("x", &startingPosition.x);
			entityElement->Attribute("y", &startingPosition.y);
			startingPosition.y -= tileSize; //Tiled Hack

			entityPath.emplace_back(startingPosition.x, startingPosition.y);
		}
	}

	assert(!entityPath.empty());
	return entityPath;
}

std::vector<std::vector<int>> decodeTileLayer(const TiXmlElement & tileLayerElement, Vector2i levelSize)
{
	std::vector<std::vector<int>> tileData;
	tileData.reserve(levelSize.y);

	std::string decodedIDs; //Base64 decoded information
	const TiXmlElement* dataNode = nullptr; //Store our node once we find it
	for (const TiXmlElement* e = tileLayerElement.FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("data"))
		{
			dataNode = e;
		}
	}
	assert(dataNode);

	Base64 base64;
	const TiXmlText* text = dataNode->FirstChild()->ToText();
	const std::string t = text->Value();
	decodedIDs = base64.base64_decode(t);

	const std::vector<int> layerColumns(levelSize.x);
	for (int i = 0; i < levelSize.y; ++i)
	{
		tileData.push_back(layerColumns);
	}

	for (int rows = 0; rows < levelSize.y; ++rows)
	{
		for (int cols = 0; cols < levelSize.x; ++cols)
		{
			tileData[rows][cols] = *((int*)decodedIDs.data() + rows * levelSize.x + cols) - 1;
		}
	}

	return tileData;
}

std::vector<TileLayer> parseTileLayers(const TiXmlElement & rootElement, Vector2i levelSize)
{
	std::vector<TileLayer> tileLayers;
	for (const auto* tileLayerElement = rootElement.FirstChildElement();
		tileLayerElement != nullptr; tileLayerElement = tileLayerElement->NextSiblingElement())
	{
		if (tileLayerElement->Value() != std::string("layer"))
		{
			continue;
		}

		std::vector<std::vector<int>> tileMap = decodeTileLayer(*tileLayerElement, levelSize);
		tileLayers.emplace_back(std::move(tileMap));
	}

	assert(!tileLayers.empty());
	return tileLayers;
}