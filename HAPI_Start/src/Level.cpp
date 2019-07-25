#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Window.h"
#include <algorithm>
#include <assert.h>
#include "Utilities/XMLParser.h"

constexpr float TIME_BETWEEN_ENTITY_SPAWN = 1.0f;
constexpr int MAX_ENTITY_SPAWN_COUNT = 20;

//Tile Layer
TileLayer::TileLayer(std::vector<std::vector<int>>&& tileData)
	: m_tileData(std::move(tileData))
{}

void TileLayer::render(Window & window, Vector2i levelSize) const 
{
	int tileSize = Textures::getInstance().texture->getTileSize();
	for (int y = 0; y < levelSize.y; ++y)
	{
		for (int x = 0; x < levelSize.x; ++x)
		{
			const int tileID = m_tileData[y][x];
			if (tileID >= 0)
			{
				Vector2i position(x * tileSize, y * tileSize);
				Sprite sprite(position, tileID);
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
	assert(m_position == position);
	m_turret.setPosition(position);
	switch (turretType)
	{
	case TurretType::Cannon :
		m_turret.m_base.setID(static_cast<int>(EntityID::TURRET_CANNON_BASE));
		m_turret.m_head.setID(static_cast<int>(EntityID::TURRET_CANNON_HEAD));
		break;

	case TurretType::Missle :
		m_turret.m_base.setID(static_cast<int>(EntityID::TURRET_MISSLE_BASE));
		m_turret.m_head.setID(static_cast<int>(EntityID::TURRET_MISSLE_HEAD));
		break;
	}
}

void Level::TurretPlacement::update(const std::vector<Unit>& units, std::vector<Projectile>& projectiles, float deltaTime)
{
	if (m_active)
	{
		m_turret.update(deltaTime, units, projectiles);
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
	m_unitMovementPath(),
	m_turretPlacements(),
	m_levelSize(),
	m_spawnTimer(TIME_BETWEEN_ENTITY_SPAWN, true),
	m_spawnedUnitCount(0)
{}

std::unique_ptr<Level> Level::loadLevel(const std::string & levelName)
{
	Level level;
	std::vector<Vector2i> turretPlacementPositions;
	if (XMLParser::parseLevel(levelName, level.m_levelSize, level.m_tileLayers, level.m_unitMovementPath, turretPlacementPositions))
	{
		level.m_turretPlacements.reserve(turretPlacementPositions.size());
		for (auto position : turretPlacementPositions)
		{
			level.m_turretPlacements.emplace_back(position);
		}

		level.m_units.reserve(static_cast<size_t>(MAX_ENTITY_SPAWN_COUNT));
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

void Level::update(float deltaTime)
{
	for (auto& turret : m_turretPlacements)
	{
		turret.update(m_units, m_projectiles, deltaTime);
	}

	for (auto& unit : m_units)
	{
		unit.update(deltaTime);
	}

	for (auto& projectile : m_projectiles)
	{
		projectile.update(deltaTime, m_units);
	}

	m_spawnTimer.update(deltaTime);
	if (m_spawnTimer.isExpired())
	{
		m_spawnTimer.reset();
		spawnNextUnit();
	}

	handleInactiveEntities();
	handleCollisions();
}

void Level::render(Window & window)
{
	for (const auto& tileLayer : m_tileLayers)
	{
		tileLayer.render(window, m_levelSize);
	}

	for (const auto& turretPlacement : m_turretPlacements)
	{
		turretPlacement.render(window);
	}

	for (const auto& unit : m_units)
	{
		unit.render(window);
	}

	for (const auto& projectile : m_projectiles)
	{
		projectile.render(window);
	}
}

void Level::spawnNextUnit()
{
	++m_spawnedUnitCount;
	if (m_spawnedUnitCount < MAX_ENTITY_SPAWN_COUNT)
	{
		m_units.emplace_back(static_cast<int>(EntityID::SOILDER_GREEN), m_unitMovementPath);
	}
}

void Level::handleInactiveEntities()
{
	for (auto iter = m_units.begin(); iter != m_units.end();)
	{
		if (!iter->isActive())
		{
			iter = m_units.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void Level::handleCollisions()
{
	int tileSize = Textures::getInstance().texture->getTileSize();
	for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();)
	{
		if (projectile->m_position.y < 15)
		{
			projectile = m_projectiles.erase(projectile);
			continue;
		}

		bool destroyProjectile = false;
		//Detect Unit Collisions
		Rectangle projectileAABB(projectile->m_position.x, tileSize, projectile->m_position.y, tileSize);
		for (auto unit = m_units.begin(); unit != m_units.end();)
		{
			Rectangle unitAABB(unit->getPosition().x, tileSize, unit->getPosition().y, tileSize);
			if (projectileAABB.intersect(unitAABB))
			{
				destroyProjectile = true;
				unit = m_units.erase(unit);
			}
			else
			{
				++unit;
			}
		}

		//Detect Turret Collisions


		//Destory Projectile on Collision
		if (destroyProjectile)
		{
			projectile = m_projectiles.erase(projectile);
		}
		else
		{
			++projectile;
		}
	}
}