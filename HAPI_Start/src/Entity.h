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

enum class TileID
{
	TURRET_CANNON_BASE = 180,
	TURRET_CANNON_HEAD = 249,
	TURRET_MISSLE_BASE = 181,
	TURRET_MISSLE_HEAD = 204,
	SOILDER_GREEN = 245,
	PROJECTILE = 272
};

enum class ProjectileSender
{
	Unit = 0,
	Turret
};

enum class UnitMoveDirection
{
	Left = 0,
	Right,
	Up,
	Down
};

class Unit;
class Window;
class Projectile
{
public:
	Projectile(Vector2i startingPosition, Vector2f startingDirection, ProjectileSender sentFrom, int tileID, float speed);

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
};

class Texture;
struct Turret
{
public:
	Turret(Vector2i startingPosition);

	bool isActive() const;
	Vector2i getPosition() const;
	void setTurret(TurretType turretType, Vector2i position);

	void update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles);
	void render(const Window& window) const;

private:
	Vector2i m_position;
	Sprite m_base;
	Sprite m_head;
	float m_attackRange;
	Timer m_fireTimer;
	bool m_active;

	bool fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const;
	void setPosition(Vector2i position);
};

class Unit
{
public:
	Unit(int tileID, const std::vector<Vector2i>& movementPath);

	Vector2i getPosition() const;
	bool isActive() const;

	void update(float deltaTime, const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles);
	void render(const Window& window) const;

private:
	std::vector<Vector2i> m_movementPath;
	Vector2i m_position;
	Sprite m_sprite;
	bool m_active;
	float m_speed;
	float m_attackRange;
	UnitMoveDirection m_moveDirection;
	Timer m_fireTimer;

	bool fire(const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles) const;
};