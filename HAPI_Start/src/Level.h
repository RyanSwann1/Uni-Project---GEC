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
		TurretPlacement(Vector2i position);
		
		Vector2i getPosition() const;
		bool isActive() const;
		void setTurret(TurretType turretType, Vector2i position);

	private:
		bool m_active;
		Vector2i m_position;
		Turret m_turret;
	};

public:

	static std::unique_ptr<Level> loadLevel(const std::string& levelName);
	
	void addTurret(Vector2i position, TurretType turretType);
	void render(Window& window, Texture& tileSheet);

private:
	Level();
	std::vector<TileLayer> m_tileLayers;
	std::vector<TurretPlacement> m_turrets;
	std::vector<Vector2i> m_entityPath; 
	Vector2i m_levelSize;

	void reset();
};