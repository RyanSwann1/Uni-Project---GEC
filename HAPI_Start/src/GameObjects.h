#pragma once

#include "Sprite.h"
#include <vector>
#include "Timer.h"
#include <array>

enum class eTurretType
{
	Cannon = 0,
	Missle
};

enum class eProjectileSender
{
	Unit = 0,
	Turret
};

enum class eUnitType
{
	Footman = 0,
	Vechile, 
	Aircraft
};

class Unit;
class Window;
class Projectile
{
public:
	Projectile(Vector2f startingPosition, Vector2f startingDirection, eProjectileSender sentFrom, 
		int tileID, float speed, int damage);

	int getDamageValue() const;
	eProjectileSender getSentFrom() const;
	Vector2f getPosition() const;

	void update(float deltaTime, const std::vector<Unit>& units);
	void render(const Window& window) const;

private:
	Vector2f m_position;
	eProjectileSender m_sentFrom;
	float m_speed;
	Sprite m_sprite;
	Vector2f m_direction;
	int m_damage;
};

class Texture;
struct Turret
{
public:
	Turret(Vector2f startingPosition);

	bool isActive() const;
	Vector2f getPosition() const;
	void setTurret(eTurretType turretType, Vector2f position);

	void damage(int damageValue);
	void update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles);
	void render(const Window& window) const;

private:
	Vector2f m_position;
	Sprite m_baseSprite;
	Sprite m_headSprite;
	float m_attackRange;
	Timer m_fireTimer;
	bool m_active;
	int m_health;

	bool fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const;
};

class Unit
{
public:
	Unit(int baseTileID, int headTileID, const std::vector<Vector2i>& movementPath, eUnitType unitType, eGameDifficulty gameDifficulty);

	Vector2f getMoveDirection() const;
	Vector2f getPosition() const;
	bool isActive() const;

	void damage(int damageValue);
	void update(float deltaTime, const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles);
	void render(const Window& window) const;

private:
	std::vector<Vector2i> m_movementPath;
	Vector2f m_position;
	Sprite m_baseSprite;
	Sprite m_headSprite;
	bool m_active;
	float m_speed;
	float m_attackRange;
	Vector2f m_moveDirection;
	Timer m_fireTimer;
	int m_health;
	eUnitType m_unitType;
	int m_damage;

	bool fire(const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles) const;
};