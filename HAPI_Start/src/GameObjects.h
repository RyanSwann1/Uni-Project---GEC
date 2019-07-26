#pragma once

#include "Sprite.h"
#include <vector>
#include "Timer.h"
#include <array>

enum class TurretType
{
	Cannon = 0,
	Missle
};

enum class ProjectileSender
{
	Unit = 0,
	Turret
};

enum class UnitType
{
	Soilder = 0,
	Tank
};

class Unit;
class Window;
class Projectile
{
public:
	Projectile(Vector2i startingPosition, Vector2f startingDirection, ProjectileSender sentFrom, 
		int tileID, float speed, int damage);

	int getDamageValue() const;
	ProjectileSender getSentFrom() const;
	Vector2i getPosition() const;

	void update(float deltaTime, const std::vector<Unit>& units);
	void render(const Window& window) const;

private:
	Vector2i m_position;
	ProjectileSender m_sentFrom;
	float m_speed;
	Sprite m_sprite;
	Vector2f m_direction;
	int m_damage;
};

class Texture;
struct Turret
{
public:
	Turret(Vector2i startingPosition);

	bool isActive() const;
	Vector2i getPosition() const;
	void setTurret(TurretType turretType, Vector2i position);

	void damage(int damageValue);
	void update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles);
	void render(const Window& window) const;

private:
	Vector2i m_position;
	Sprite m_baseSprite;
	Sprite m_headSprite;
	float m_attackRange;
	Timer m_fireTimer;
	bool m_active;
	int m_health;

	bool fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const;
	void setPosition(Vector2i position);
};

class Unit
{
public:
	Unit(int baseTileID, int headTileID, const std::vector<Vector2i>& movementPath, UnitType unitType);

	Vector2f getMoveDirection() const;
	Vector2i getPosition() const;
	bool isActive() const;

	void damage(int damageValue);
	void update(float deltaTime, const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles);
	void render(const Window& window) const;

private:
	std::vector<Vector2i> m_movementPath;
	Vector2i m_position;
	Sprite m_baseSprite;
	Sprite m_headSprite;
	bool m_active;
	float m_speed;
	float m_attackRange;
	Vector2f m_moveDirection;
	Timer m_fireTimer;
	int m_health;
	UnitType m_unitType;
	int m_damage;

	bool fire(const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles) const;
};