#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Window.h"
#include <algorithm>
#include <assert.h>
#include "Utilities/XMLParser.h"

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
			}
		}
	}
}

//Turret Placement
Level::TurretPlacement::TurretPlacement(Vector2i position)
	: m_position(position),
	m_active(false),
	m_turret()
{
}

Vector2i Level::TurretPlacement::getPosition() const
{
	return m_position;
}

bool Level::TurretPlacement::isActive() const
{
	return m_active;
}

void Level::TurretPlacement::setTurret(TurretType turretType, Vector2i position)
{

}

//Level
Level::Level()
	: m_tileLayers(),
	m_turrets(),
	m_entityPath(),
	m_levelSize()
{}

std::unique_ptr<Level> Level::loadLevel(const std::string & levelName)
{
	Level level;
	std::vector<Vector2i> turretPlacementPositions;
	if (XMLParser::parseLevel(levelName, level.m_levelSize, level.m_tileLayers, level.m_entityPath, turretPlacementPositions))
	{
		level.m_turrets.reserve(turretPlacementPositions.size());
		for (auto position : turretPlacementPositions)
		{
			level.m_turrets.emplace_back(position);
		}

		return std::make_unique<Level>(std::move(level));
	}
	else
	{
		return std::unique_ptr<Level>();
	}
}

void Level::addTurret(Vector2i position, TurretType turretType)
{
	auto iter = std::find_if(m_turrets.begin(), m_turrets.end(), [position](const auto& turret) { return turret.getPosition() == position; });
	if (iter != m_turrets.end())
	{
		iter->setTurret(turretType, position);
	}
}

void Level::render(Window & window, Texture& tileSheet)
{
	for (const auto& tileLayer : m_tileLayers)
	{
		tileLayer.render(window, m_levelSize, tileSheet);
	}
}

void Level::reset()
{
	m_tileLayers.clear();
	m_turrets.clear();
	m_entityPath.clear();
	m_levelSize = Vector2i();
}
