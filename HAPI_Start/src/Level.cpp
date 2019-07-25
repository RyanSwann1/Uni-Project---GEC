#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Window.h"
#include <algorithm>
#include <assert.h>
#include "Utilities/XMLParser.h"

constexpr float TIME_BETWEEN_ENTITY_SPAWN = 1.0f;
constexpr int MAX_UNIT_SPAWN_COUNT = 20;

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
			if (tileID > static_cast<int>(TileID::INVALID))
			{
				Vector2i position(x * tileSize, y * tileSize);
				Sprite sprite(position, tileID);
				window.render(sprite);
			}
		}
	}
}

//Level
Level::Level()
	: m_tileLayers(),
	m_unitMovementPath(),
	m_turrets(),
	m_units(),
	m_projectiles(),
	m_particles(),
	m_levelSize(),
	m_spawnTimer(TIME_BETWEEN_ENTITY_SPAWN, true),
	m_spawnedUnitCount(0),
	m_unitsReachedDestination(0)
{}

std::unique_ptr<Level> Level::loadLevel(const std::string & levelName)
{
	Level level;
	std::vector<Vector2i> turretPlacementPositions;
	if (XMLParser::parseLevel(levelName, level.m_levelSize, level.m_tileLayers, level.m_unitMovementPath, turretPlacementPositions))
	{
		level.m_turrets.reserve(turretPlacementPositions.size());
		for (auto position : turretPlacementPositions)
		{
			level.m_turrets.emplace_back(position);
		}

		level.m_units.reserve(static_cast<size_t>(MAX_UNIT_SPAWN_COUNT));
		return std::make_unique<Level>(std::move(level));
	}
	else
	{
		return std::unique_ptr<Level>();
	}
}

void Level::addTurretAtPosition(Vector2i position, TurretType turretType)
{
	auto iter = std::find_if(m_turrets.begin(), m_turrets.end(), [position](const auto& turret) { return turret.getPosition() == position; });
	if (iter != m_turrets.end())
	{
		iter->setTurret(turretType, position);
	}
}

bool Level::isEnded() const
{
	return m_spawnedUnitCount == MAX_UNIT_SPAWN_COUNT && m_units.empty();
}

void Level::update(float deltaTime)
{
	for (auto& turret : m_turrets)
	{
		turret.update(deltaTime, m_units, m_projectiles);
	}

	for (auto& unit : m_units)
	{
		unit.update(deltaTime, m_turrets, m_projectiles);
	}

	for (auto& projectile : m_projectiles)
	{
		projectile.update(deltaTime, m_units);
	}

	for (auto& particle : m_particles)
	{
		particle.update(deltaTime);
	}

	m_spawnTimer.update(deltaTime);
	if (m_spawnTimer.isExpired())
	{
		m_spawnTimer.reset();
		spawnNextUnit();
	}

	handleInactiveEntities();
	handleCollisions();
	handleParticles();
}

void Level::render(Window & window)
{
	for (const auto& tileLayer : m_tileLayers)
	{
		tileLayer.render(window, m_levelSize);
	}

	for (const auto& turretPlacement : m_turrets)
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

	for (const auto& particle : m_particles)
	{
		particle.render(window);
	}

	HAPI.RenderText(850, 50, HAPISPACE::HAPI_TColour::WHITE, std::to_string(m_unitsReachedDestination), 26);
}

void Level::spawnNextUnit()
{
	++m_spawnedUnitCount;
	if (m_spawnedUnitCount < MAX_UNIT_SPAWN_COUNT)
	{
		//m_units.emplace_back(static_cast<int>(TileID::SOILDER_GREEN), static_cast<int>(TileID::INVALID), m_unitMovementPath);
		m_units.emplace_back(static_cast<int>(TileID::TANK_BASE), static_cast<int>(TileID::TANK_HEAD), m_unitMovementPath);
		//m_units.emplace_back
		//m_units.emplace_back
	}
}

void Level::handleInactiveEntities()
{
	for (auto iter = m_units.begin(); iter != m_units.end();)
	{
		if (!iter->isActive())
		{
			iter = m_units.erase(iter);
			++m_unitsReachedDestination;
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
		if (projectile->getPosition().y < 15)
		{
			projectile = m_projectiles.erase(projectile);
			continue;
		}

		bool destroyProjectile = false;
		//Detect Unit Collisions
		if (projectile->getSentFrom() == ProjectileSender::Turret)
		{
			Rectangle projectileAABB(projectile->getPosition().x, tileSize, projectile->getPosition().y, tileSize);
			for (auto unit = m_units.begin(); unit != m_units.end();)
			{
				Rectangle unitAABB(unit->getPosition().x, tileSize, unit->getPosition().y, tileSize);
				if (projectileAABB.intersects(unitAABB))
				{
					destroyProjectile = true;
					unit = m_units.erase(unit);
				}
				else
				{
					++unit;
				}
			}
		}
		//Detect Turret Collisions
		else
		{
			Rectangle projectileAABB(projectile->getPosition().x, tileSize, projectile->getPosition().y, tileSize);
			for (auto turret = m_turrets.begin(); turret != m_turrets.end();)
			{
				Rectangle turretAABB(turret->getPosition().x, tileSize, turret->getPosition().y, tileSize);
				if (projectileAABB.intersects(turretAABB))
				{
					m_particles.emplace_back(turret->getPosition(), static_cast<int>(TileID::PARTICLE));
					destroyProjectile = true;
				}

				++turret;
			}
		}

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

void Level::handleParticles()
{
	for (auto particle = m_particles.begin(); particle != m_particles.end();)
	{
		if (!particle->isActive())
		{
			particle = m_particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}