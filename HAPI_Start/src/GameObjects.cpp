#include "GameObjects.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"
#include "Texture.h"

constexpr float TIME_BETWEEN_TURRET_SHOT = 1.0f;
constexpr float TIME_BETWEEN_UNIT_SHOT = 1.0f;

constexpr float TURRET_PROJECTLE_SPEED = 5.0f;
constexpr float TURRET_ATTACK_RANGE = 250.f;
constexpr int TURRET_HEALTH = 5;
constexpr int TURRET_DAMAGE = 1;

constexpr float UNIT_ATTACK_RANGE = 250.f;
constexpr float UNIT_PROJECTILE_SPEED = 2.5f;
constexpr float UNIT_SPEED = 5.0f;
constexpr int UNIT_HEALTH = 1;

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

int Projectile::getDamage() const
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
	m_active(false)
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

void Turret::update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles)
{
	if (m_active)
	{
		m_fireTimer.update(deltaTime);
		if (m_fireTimer.isExpired())
		{
			if (fire(units, projectiles))
			{
				m_fireTimer.reset();
			}
		}
	}
}

bool Turret::fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const
{
	for (const auto& unit : units)
	{
		if (Math::isWithinRange(m_position, unit.getPosition(), m_attackRange))
		{
			Vector2f dir = Math::getDirection(m_position, unit.getPosition());
			projectiles.emplace_back(m_position, dir, ProjectileSender::Turret, static_cast<int>(TileID::PROJECTILE), TURRET_PROJECTLE_SPEED);
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
Unit::Unit(int baseTileID, int headTileID, const std::vector<Vector2i>& movementPath)
	: m_movementPath(movementPath),
	m_position(movementPath.back()),
	m_baseSprite(m_position, baseTileID),
	m_headSprite(m_position, headTileID),
	m_active(true),
	m_speed(UNIT_SPEED),
	m_attackRange(UNIT_ATTACK_RANGE),
	m_fireTimer(TIME_BETWEEN_UNIT_SHOT, true)
{
	m_movementPath.pop_back();
	m_moveDirection = Math::getDirectionTowards(m_position, m_movementPath.back());
}

Vector2i Unit::getPosition() const
{
	return m_position;
}

bool Unit::isActive() const
{
	return m_active;
}

void Unit::update(float deltaTime, const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles)
{
	m_fireTimer.update(deltaTime);
	if (m_fireTimer.isExpired())
	{
		if (fire(turrets, projectiles))
		{
			m_fireTimer.reset();
		}
	}

	//Move to destination
	bool reachedDestination = false;
	switch (m_moveDirection)
	{
	case UnitMoveDirection::Up:
		m_position.y -= m_speed;
		if (m_position.y <= m_movementPath.back().y)
		{
			reachedDestination = true;
		}
		break;

	case UnitMoveDirection::Right:
		m_position.x += m_speed;
		if (m_position.x >= m_movementPath.back().x)
		{
			reachedDestination = true;
		}
		break;
	}

	m_baseSprite.setPosition(m_position);
	m_headSprite.setPosition(m_position);
	
	//Assign new destination
	if (reachedDestination)
	{
		m_position = m_movementPath.back();
		m_movementPath.pop_back();
		if (m_movementPath.empty())
		{
			m_active = false;
			return;
		}

		m_moveDirection = Math::getDirectionTowards(m_position, m_movementPath.back());
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
			projectiles.emplace_back(m_position, dir, ProjectileSender::Unit, static_cast<int>(TileID::PROJECTILE), UNIT_PROJECTILE_SPEED);
			return true;
		}
	}

	return false;
}