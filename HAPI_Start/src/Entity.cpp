#include "Entity.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"
#include "Texture.h"

constexpr float TIME_BETWEEN_TURRET_SHOT = 1.0f;
constexpr float TIME_BETWEEN_UNIT_SHOT = 1.0f;

//Projectile
Projectile::Projectile(Vector2i startingPosition, Vector2f startingDirection, ProjectileSender sentFrom, int tileID, float speed)
	: m_position(startingPosition),
	m_sentFrom(sentFrom),
	m_speed(speed),
	m_sprite(startingPosition, tileID),
	m_direction(startingDirection)
{}

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
	m_base(),
	m_head(),
	m_attackRange(250.0),
	m_fireTimer(TIME_BETWEEN_TURRET_SHOT, true),
	m_active(false)
{}

void Turret::render(const Window & window) const
{
	if (m_active)
	{
		window.render(m_base);
		window.render(m_head);
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
		m_base.setID(static_cast<int>(TileID::TURRET_CANNON_BASE));
		m_head.setID(static_cast<int>(TileID::TURRET_CANNON_HEAD));
		break;

	case TurretType::Missle:
		m_base.setID(static_cast<int>(TileID::TURRET_MISSLE_BASE));
		m_head.setID(static_cast<int>(TileID::TURRET_MISSLE_HEAD));
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
			projectiles.emplace_back(m_position, dir, ProjectileSender::Turret, static_cast<int>(TileID::PROJECTILE));
			return true;
		}
	}

	return false;
}

void Turret::setPosition(Vector2i position)
{
	m_position = position;
	m_base.setPosition(position);
	m_head.setPosition(position);
}

//Unit
Unit::Unit(int tileID, const std::vector<Vector2i>& movementPath)
	: m_movementPath(movementPath),
	m_position(movementPath.back()),
	m_sprite(),
	m_active(true),
	m_speed(2.0f),
	m_attackRange(250.0f),
	m_fireTimer(TIME_BETWEEN_UNIT_SHOT, true)
{
	m_movementPath.pop_back();
	m_moveDirection = Math::getDirectionTowards(m_position, m_movementPath.back());
	m_sprite.setID(tileID);
	m_sprite.setPosition(m_position);
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
		m_sprite.setPosition(m_position);
		if (m_position.y <= m_movementPath.back().y)
		{
			reachedDestination = true;
		}
		break;

	case UnitMoveDirection::Right:
		m_position.x += m_speed;
		m_sprite.setPosition(m_position);
		if (m_position.x >= m_movementPath.back().x)
		{
			reachedDestination = true;
		}
		break;
	}
	
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
	window.render(m_sprite);	
}

bool Unit::fire(const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles) const
{
	for (const auto& turret : turrets)
	{
		if (Math::isWithinRange(m_position, turret.getPosition(), m_attackRange) && turret.isActive())
		{
			Vector2f dir = Math::getDirection(m_position, turret.getPosition());
			projectiles.emplace_back(m_position, dir, ProjectileSender::Unit, static_cast<int>(TileID::PROJECTILE));
			return true;
		}
	}

	return false;
}