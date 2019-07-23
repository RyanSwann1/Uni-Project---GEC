#pragma once

#include <vector>
#include "Global.h"
#include "Entity.h"

class Texture;
class Window;
struct TileLayer
{
	TileLayer(std::vector<std::vector<int>>&& tileData);

	void render(Window& window, Vector2i levelSize, Texture& tileSheet) const;

	std::vector<std::vector<int>> m_tileData;
};

class Level
{
	class TurretPlacement
	{
	public:
		TurretPlacement(Vector2i position, Texture& tileSheet);
		
		Vector2i getPosition() const;
		bool isActive() const;
		void setTurret(TurretType turretType, Vector2i position);

		void render(const Window& window) const;

	private:
		bool m_active;
		Vector2i m_position;
		Turret m_turret;
	};

public:

	static std::unique_ptr<Level> loadLevel(const std::string& levelName, Texture& tileSheet);
	
	void addTurretAtPosition(Vector2i position, TurretType turretType);
	
	void update(float deltaTime);
	void render(Window& window, Texture& tileSheet);

private:
	Level();
	std::vector<TileLayer> m_tileLayers;
	std::vector<TurretPlacement> m_turretPlacements;
	std::vector<Entity> m_entities;
	Vector2i m_levelSize;
	float m_timeElasped = 0;

	//void addEntity()
};