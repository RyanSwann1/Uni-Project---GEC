#pragma once

#include <vector>
#include "Global.h"
#include "Entity.h"
#include "Timer.h"

class Window;
struct TileLayer
{
	TileLayer(std::vector<std::vector<int>>&& tileData);

	void render(Window& window, Vector2i levelSize) const;

	std::vector<std::vector<int>> m_tileData;
};

class Level
{
public:
	static std::unique_ptr<Level> loadLevel(const std::string& levelName);
	
	void addTurretAtPosition(Vector2i position, TurretType turretType);
	
	void update(float deltaTime);
	void render(Window& window);

private:
	Level();
	std::vector<TileLayer> m_tileLayers;
	std::vector<Vector2i> m_unitMovementPath;
	std::vector<Turret> m_turrets;
	std::vector<Unit> m_units;
	std::vector<Projectile> m_projectiles;
	Vector2i m_levelSize;
	Timer m_spawnTimer;
	int m_spawnedUnitCount;

	void spawnNextUnit();
	void handleInactiveEntities();
	void handleCollisions();
};