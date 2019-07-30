#include "Level.h"
#include "Sprite.h"
#include "Texture.h"
#include "Window.h"
#include <algorithm>
#include <assert.h>
#include "Utilities/XMLParser.h"

constexpr float TIME_BETWEEN_ENTITY_SPAWN = 3.5f;
constexpr int MAX_UNIT_SPAWN_COUNT = 10;

constexpr size_t MAX_PROJECTILES_COUNT = 100;
constexpr size_t MAX_PARTICLES_COUNT = 50;

constexpr int TURRET_PLACEMENT_COST = 5;
constexpr int UNIT_ELIMINATE_SCORE = 2;

constexpr int DIFFICULTY_MEDIUM_SPAWN_RATE_MODIFIER = -1;
constexpr int DIFFICULTY_HARD_SPAWN_RATE_MODIFIER = -2;

//Tile Layer
TileLayer::TileLayer(std::vector<std::vector<int>>&& tileData)
	: m_tileData(std::move(tileData))
{}

void TileLayer::render(Window & window, Vector2i levelSize) const 
{
	int tileSize = Textures::getInstance().getTexture().getTileSize();
	for (int y = 0; y < levelSize.y; ++y)
	{
		for (int x = 0; x < levelSize.x; ++x)
		{
			const int tileID = m_tileData[y][x];
			if (tileID > static_cast<int>(eTileID::INVALID))
			{
				Vector2f position(x * tileSize, y * tileSize);
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

std::unique_ptr<Level> Level::loadLevel(const std::string & levelName, eGameDifficulty gameDifficulty)
{
	Level level;
	std::vector<Vector2i> turretPlacementPositions;
	if (XMLParser::parseLevel(levelName, level.m_levelSize, level.m_tileLayers, level.m_unitMovementPath, turretPlacementPositions,
		level.m_tankSpawnRate, level.m_planeSpawnRate))
	{
		//Initialize all game objects - requiring no memory allocations at run time
		level.m_turrets.reserve(turretPlacementPositions.size());
		for (auto position : turretPlacementPositions)
		{
			level.m_turrets.emplace_back(Vector2f(position.x, position.y));
		}

		level.m_units.reserve(static_cast<size_t>(MAX_UNIT_SPAWN_COUNT));
		level.m_projectiles.reserve(MAX_PROJECTILES_COUNT);
		level.m_particles.reserve(MAX_PARTICLES_COUNT);

		assert(level.m_tankSpawnRate > 0);
		assert(level.m_planeSpawnRate > 0);
		if (gameDifficulty == eGameDifficulty::HARD)
		{
			level.m_tankSpawnRate -= DIFFICULTY_MEDIUM_SPAWN_RATE_MODIFIER;
			level.m_planeSpawnRate -= DIFFICULTY_MEDIUM_SPAWN_RATE_MODIFIER;
		}
		else if (gameDifficulty == eGameDifficulty::EXTREME)
		{
			level.m_tankSpawnRate -= DIFFICULTY_HARD_SPAWN_RATE_MODIFIER;
			level.m_planeSpawnRate -= DIFFICULTY_HARD_SPAWN_RATE_MODIFIER;
		}

		return std::make_unique<Level>(std::move(level));
	}
	else
	{
		return std::unique_ptr<Level>();
	}
}

void Level::addTurretAtPosition(Vector2f position, eTurretType turretType, int& playerScore)
{
	if (playerScore >= TURRET_PLACEMENT_COST)
	{
		auto iter = std::find_if(m_turrets.begin(), m_turrets.end(), [position](const auto& turret) { return turret.getPosition() == position; });
		if (iter != m_turrets.end())
		{
			if (!iter->isActive())
			{
				playerScore -= TURRET_PLACEMENT_COST;
			}

			iter->setTurret(turretType, position);
		}
	}
}

bool Level::isEnded() const
{
	return m_spawnedUnitCount >= MAX_UNIT_SPAWN_COUNT && m_units.empty();
}

void Level::update(float deltaTime, int& playerScore, eGameDifficulty gameDifficulty, Vector2i windowSize)
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
		spawnNextUnit(gameDifficulty);
	}

	handleInactiveEntities();
	handleCollisions(playerScore, windowSize);
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

void Level::spawnNextUnit(eGameDifficulty gameDifficulty)
{
	if (m_spawnedUnitCount < MAX_UNIT_SPAWN_COUNT)
	{
		++m_spawnedUnitCount;

		if (m_spawnedUnitCount % m_tankSpawnRate == 0)
		{
			++m_spawnedUnitCount;
			m_units.emplace_back(static_cast<int>(eTileID::TANK_BASE), static_cast<int>(eTileID::TANK_HEAD), 
				m_unitMovementPath, eUnitType::Vechile, gameDifficulty);
		}
		else if (m_spawnedUnitCount % m_planeSpawnRate == 0)
		{
			++m_spawnedUnitCount;
			m_units.emplace_back(static_cast<int>(eTileID::PLANE_SHADOW), static_cast<int>(eTileID::PLANE),
				m_unitMovementPath, eUnitType::Aircraft, gameDifficulty);
		}

		m_units.emplace_back(static_cast<int>(eTileID::SOILDER_GREEN), static_cast<int>(eTileID::INVALID),
			m_unitMovementPath, eUnitType::Footman, gameDifficulty);
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

void Level::handleCollisions(int& playerScore, Vector2i windowSize)
{
	int tileSize = Textures::getInstance().getTexture().getTileSize();
	for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();)
	{
		Vector2i projectilePosition(static_cast<int>(projectile->getPosition().x), static_cast<int>(projectile->getPosition().y));
		//Handle projectiles escaping window size
		if (projectile->getPosition().x < 0 || projectile->getPosition().x > windowSize.x ||
			projectile->getPosition().y < 15 || projectile->getPosition().y > windowSize.y)
		{
			projectile = m_projectiles.erase(projectile);
			continue;
		}


		//Detect Unit Collisions
		bool destroyProjectile = false;
		if (projectile->getSentFrom() == eProjectileSender::Turret)
		{
			Rectangle projectileAABB(projectilePosition.x, tileSize, projectilePosition.y, tileSize);
			for (auto unit = m_units.begin(); unit != m_units.end();)
			{
				Rectangle unitAABB(unit->getPosition().x, tileSize, unit->getPosition().y, tileSize);
				if (projectileAABB.intersects(unitAABB))
				{
					destroyProjectile = true;
					unit->damage(projectile->getDamageValue());
					if (!unit->isActive())
					{
						unit = m_units.erase(unit);
						playerScore += UNIT_ELIMINATE_SCORE;
						continue;
					}
				}

				++unit;
			}
		}

		//Detect Turret Collisions
		else if(projectile->getSentFrom() == eProjectileSender::Unit)
		{
			Rectangle projectileAABB(projectile->getPosition().x, tileSize, projectile->getPosition().y, tileSize);
			for (auto turret = m_turrets.begin(); turret != m_turrets.end();)
			{
				Rectangle turretAABB(static_cast<int>(turret->getPosition().x), tileSize, 
					static_cast<int>(turret->getPosition().y), tileSize);
				if (projectileAABB.intersects(turretAABB))
				{
					turret->damage(projectile->getDamageValue());
					m_particles.emplace_back(turret->getPosition(), static_cast<int>(eTileID::PARTICLE));
					destroyProjectile = true;
				}

				++turret;
			}
		}


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
		if (particle->isExpired())
		{
			particle = m_particles.erase(particle);
		}
		else
		{
			++particle;
		}
	}
}