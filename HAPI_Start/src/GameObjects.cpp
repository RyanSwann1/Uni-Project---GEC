#include "GameObjects.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"
#include "Texture.h"

constexpr float TIME_BETWEEN_TURRET_SHOT = 2.5f;
constexpr float TIME_BETWEEN_UNIT_SHOT = 1.0f;

constexpr float TURRET_PROJECTLE_SPEED = 7.5f;
constexpr float TURRET_ATTACK_RANGE = 350.f;
constexpr int TURRET_MAX_HEALTH = 3;
constexpr int TURRET_DAMAGE = 1;

constexpr float UNIT_ATTACK_RANGE = 250.f;
constexpr float UNIT_PROJECTILE_SPEED = 2.5f;

constexpr int TANK_MAX_HEALTH = 3;
constexpr int TANK_DAMAGE_VALUE = 2;
constexpr float TANK_MOVEMENT_SPEED = 1.0f;

constexpr int SOILDER_MAX_HEALTH = 1;
constexpr float SOILDER_MOVEMENT_SPEED = 1.5f;

constexpr int PLANE_MAX_HEALTH = 2;
constexpr float PLANE_MOVEMENT_SPEED = 2.0f;

constexpr float GAME_DIFFICULTY_MODIFIER_EASY = 1.0f;
constexpr float GAME_DIFFICULTY_MODIFIER_MEDIUM = 1.2f;
constexpr float GAME_DIFFICULTY_MODIFIER_HARD = 1.4f;

//Projectile
Projectile::Projectile(Vector2i startingPosition, Vector2f startingDirection, ProjectileSender sentFrom, 
	int tileID, float speed, int damage)
	: m_position(startingPosition),
	m_sentFrom(sentFrom),
	m_speed(speed),
	m_sprite(startingPosition, tileID),
	m_direction(startingDirection),
	m_damage(damage)
{}

int Projectile::getDamageValue() const
{
	return m_damage;
}

ProjectileSender Projectile::getSentFrom() const
{
	return m_sentFrom;
}

Vector2i Projectile::getPosition() const
{
	return m_position;
}

void Projectile::update(float deltaTime, const std::vector<Unit>& units)
{
	Vector2f position = Vector2f(m_position.x, m_position.y);

	position.x += m_direction.x * m_speed;
	position.y += m_direction.y * m_speed;
	
	m_position.x = position.x;
	m_position.y = position.y;

	m_sprite.setPosition(m_position);
}

void Projectile::render(const Window & window) const
{
	window.render(m_sprite);
}

//Turret
Turret::Turret(Vector2i startingPosition)
	: m_position(startingPosition),
	m_baseSprite(),
	m_headSprite(),
	m_attackRange(TURRET_ATTACK_RANGE),
	m_fireTimer(TIME_BETWEEN_TURRET_SHOT, true),
	m_active(false),
	m_health(TURRET_MAX_HEALTH)
{}

void Turret::render(const Window & window) const
{
	if (m_active)
	{
		window.render(m_baseSprite);
		window.render(m_headSprite);
	}
}

bool Turret::isActive() const
{
	return m_active;
}

Vector2i Turret::getPosition() const
{
	return m_position;
}

void Turret::setTurret(TurretType turretType, Vector2i position)
{
	m_active = true;

	assert(m_position == position);
	setPosition(position);
	switch (turretType)
	{
	case TurretType::Cannon:
		m_baseSprite.setID(static_cast<int>(TileID::TURRET_CANNON_BASE));
		m_headSprite.setID(static_cast<int>(TileID::TURRET_CANNON_HEAD));
		break;

	case TurretType::Missle:
		m_baseSprite.setID(static_cast<int>(TileID::TURRET_MISSLE_BASE));
		m_headSprite.setID(static_cast<int>(TileID::TURRET_MISSLE_HEAD));
		break;
	}
}

void Turret::damage(int damageValue)
{
	m_health -= damageValue;
	if (m_health <= 0)
	{
		m_health = 0;
		m_active = false;
	}
}

void Turret::update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles)
{
	if (m_active)
	{
		m_fireTimer.update(deltaTime);
		if (m_fireTimer.isExpired() && fire(units, projectiles))
		{
			m_fireTimer.reset();
		}
	}
}

bool Turret::fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const
{
	for (const auto& unit : units)
	{
		if (Math::isWithinRange(m_position, unit.getPosition(), m_attackRange))
		{
			int xOffset = 0;
			if (unit.getMoveDirection().x == 1)
			{
				xOffset += 40;
			}
			Vector2f dir = Math::getDirection(m_position, { unit.getPosition().x + xOffset, unit.getPosition().y });
			projectiles.emplace_back(m_position, dir, ProjectileSender::Turret, static_cast<int>(TileID::PROJECTILE), TURRET_PROJECTLE_SPEED, TURRET_DAMAGE);
			return true;
		}
	}

	return false;
}

void Turret::setPosition(Vector2i position)
{
	m_position = position;
	m_baseSprite.setPosition(position);
	m_headSprite.setPosition(position);
}

//Unit
Unit::Unit(int baseTileID, int headTileID, const std::vector<Vector2i>& movementPath, UnitType unitType, GameDifficulty gameDifficulty)
	: m_movementPath(movementPath),
	m_position(movementPath.back()),
	m_baseSprite(m_position, baseTileID),
	m_headSprite(m_position, headTileID),
	m_active(true),
	m_speed(0),
	m_attackRange(UNIT_ATTACK_RANGE),
	m_fireTimer(TIME_BETWEEN_UNIT_SHOT, true),
	m_health(0),
	m_unitType(unitType),
	m_damage(0)
{
	switch (m_unitType)
	{
	case UnitType::Soilder :
		m_health = SOILDER_MAX_HEALTH;
		m_speed = SOILDER_MOVEMENT_SPEED;
		break;
	
	case UnitType::Tank :
		m_health = TANK_MAX_HEALTH;
		m_damage = TANK_DAMAGE_VALUE;
		m_speed = TANK_MOVEMENT_SPEED;
		break;
	
	case UnitType::Plane :
		m_health = PLANE_MAX_HEALTH;
		m_speed = PLANE_MOVEMENT_SPEED;
		break;
	}

	switch (gameDifficulty)
	{
	case GameDifficulty::NORMAL:
		m_speed *= GAME_DIFFICULTY_MODIFIER_EASY;
		break;

	case GameDifficulty::HARD:
		m_speed *= GAME_DIFFICULTY_MODIFIER_MEDIUM;
		break;

	case GameDifficulty::EXTREME:
		m_speed *= GAME_DIFFICULTY_MODIFIER_HARD;
		break;
	}

	m_movementPath.pop_back();
	m_moveDirection = Math::getDirection(m_position, m_movementPath.back());
}

Vector2f Unit::getMoveDirection() const
{
	return m_moveDirection;
}

Vector2i Unit::getPosition() const
{
	return m_position;
}

bool Unit::isActive() const
{
	return m_active;
}

void Unit::damage(int damageValue)
{
	m_health -= damageValue;
	if (m_health <= 0)
	{
		m_health = 0;
		m_active = false;
	}
}

void Unit::update(float deltaTime, const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles)
{
	//Fire Weapon
	if (m_unitType == UnitType::Tank)
	{
		m_fireTimer.update(deltaTime);
		if (m_fireTimer.isExpired() && (fire(turrets, projectiles)))
		{
			m_fireTimer.reset();
		}
	}

	//Update position
	Vector2f position = Vector2f(m_position.x, m_position.y);

	position.x += m_moveDirection.x * m_speed;
	position.y += m_moveDirection.y * m_speed;

	m_position.x = position.x;
	m_position.y = position.y;

	m_baseSprite.setPosition(m_position);
	m_headSprite.setPosition(m_position);

	//Up
	bool reachedDestination = false;
	if (m_moveDirection.y == -1 && m_position.y <= m_movementPath.back().y)
	{
		reachedDestination = true;
	}
	//Right
	else if (m_moveDirection.x == 1 && m_position.x >= m_movementPath.back().x)
	{
		reachedDestination = true;
	}
	//Left
	else if (m_moveDirection.x == -1 && m_position.x <= m_movementPath.back().x)
	{
		reachedDestination = true;
	}
	//Down
	else if (m_moveDirection.y == 1 && m_position.y >= m_movementPath.back().y)
	{
		reachedDestination = true;
	}
	
	//Assign new destination
	if (reachedDestination)
	{
		m_position = m_movementPath.back();
		m_baseSprite.setPosition(m_position);
		m_headSprite.setPosition(m_position);
		m_movementPath.pop_back();
		if (m_movementPath.empty())
		{
			m_active = false;
			return;
		}

		m_moveDirection = Math::getDirection(m_position, m_movementPath.back());
	}
}

void Unit::render(const Window & window) const
{
	window.render(m_baseSprite);
	window.render(m_headSprite);
}

bool Unit::fire(const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles) const
{
	for (const auto& turret : turrets)
	{
		if (Math::isWithinRange(m_position, turret.getPosition(), m_attackRange) && turret.isActive())
		{
			Vector2f dir = Math::getDirection(m_position, turret.getPosition());
			projectiles.emplace_back(m_position, dir, ProjectileSender::Unit, static_cast<int>(TileID::PROJECTILE), UNIT_PROJECTILE_SPEED, m_damage);
			return true;
		}
	}

	return false;
}