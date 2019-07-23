#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Window.h"
#include <algorithm>
#include <assert.h>
#include "Utilities/XMLParser.h"

constexpr float TIME_BETWEEN_ENTITY_SPAWN = 2.5f;
constexpr int MAX_ENTITY_SPAWN_COUNT = 20;
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
Level::TurretPlacement::TurretPlacement(Vector2i position, Texture& tileSheet)
	: m_position(position),
	m_active(false),
	m_turret(tileSheet)
{}

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
	m_active = true;
	switch (turretType)
	{
	case TurretType::Cannon :
		m_turret.m_base.setID(static_cast<int>(EntityID::TURRET_CANNON_BASE));
		m_turret.m_base.setPosition(position);
		m_turret.m_head.setID(static_cast<int>(EntityID::TURRET_CANNON_HEAD));
		m_turret.m_head.setPosition(position);
		break;

	case TurretType::Missle :
		m_turret.m_base.setID(static_cast<int>(EntityID::TURRET_MISSLE_BASE));
		m_turret.m_base.setPosition(position);
		m_turret.m_head.setID(static_cast<int>(EntityID::TURRET_MISSLE_HEAD));
		m_turret.m_head.setPosition(position);
		break;
	}
}

void Level::TurretPlacement::render(const Window & window) const
{
	if (m_active)
	{
		m_turret.render(window);
	}
}

//Level
Level::Level()
	: m_tileLayers(),
	m_entityPath(),
	m_turretPlacements(),
	m_levelSize(),
	m_spawnTimer(TIME_BETWEEN_ENTITY_SPAWN, true),
	m_spawnedEntityCount(0)
{}

std::unique_ptr<Level> Level::loadLevel(const std::string & levelName, Texture& tileSheet)
{
	Level level;
	std::vector<Vector2i> turretPlacementPositions;
	if (XMLParser::parseLevel(levelName, level.m_levelSize, level.m_tileLayers, level.m_entityPath, turretPlacementPositions))
	{
		level.m_turretPlacements.reserve(turretPlacementPositions.size());
		for (auto position : turretPlacementPositions)
		{
			level.m_turretPlacements.emplace_back(position, tileSheet);
		}

		level.m_entities.reserve(static_cast<size_t>(MAX_ENTITY_SPAWN_COUNT));
		return std::make_unique<Level>(std::move(level));
	}
	else
	{
		return std::unique_ptr<Level>();
	}
}

void Level::addTurretAtPosition(Vector2i position, TurretType turretType)
{
	auto iter = std::find_if(m_turretPlacements.begin(), m_turretPlacements.end(), [position](const auto& turret) { return turret.getPosition() == position; });
	if (iter != m_turretPlacements.end())
	{
		iter->setTurret(turretType, position);
	}
}

void Level::update(float deltaTime, Texture& tileSheet)
{
	m_spawnTimer.update(deltaTime);
	if (m_spawnTimer.isExpired())
	{
		m_spawnTimer.reset();
		spawnNextEntity(tileSheet);
	}
}

void Level::render(Window & window, Texture& tileSheet)
{
	for (const auto& tileLayer : m_tileLayers)
	{
		tileLayer.render(window, m_levelSize, tileSheet);
	}

	for (const auto& turretPlacement : m_turretPlacements)
	{
		turretPlacement.render(window);
	}
}

void Level::spawnNextEntity(Texture& tileSheet)
{
	++m_spawnedEntityCount;
	if (m_spawnedEntityCount < MAX_ENTITY_SPAWN_COUNT)
	{
		m_entities.emplace_back(tileSheet, static_cast<int>(EntityID::SOILDER_GREEN), m_entityPath);
	}
}