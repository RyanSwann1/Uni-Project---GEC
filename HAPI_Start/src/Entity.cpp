#include "Entity.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"
#include "Texture.h"

constexpr float TIME_BETWEEN_TURRET_SHOT = 1.0f;

//Projectile
Projectile::Projectile(Vector2i startingPosition, Vector2f startingDirection, ProjectileSender sentFrom, int tileID)
	: m_position(startingPosition),
	m_sentFrom(sentFrom),
	m_speed(10.0f),
	m_sprite(startingPosition, tileID),
	m_direction(startingDirection)
{}

void Projectile::update(float deltaTime, const std::vector<Unit>& units)
{
	Vector2f position = Vector2f(m_position.x, m_position.y);
	//m_direction.x *= m_speed;
	//m_direction.y *= m_speed;

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
Turret::Turret()
	: m_position(),
	m_base(),
	m_head(),
	m_attackRange(250.0),
	m_fireTimer(TIME_BETWEEN_TURRET_SHOT, true)
{}

void Turret::render(const Window & window) const
{
	window.render(m_base);
	window.render(m_head);
}

void Turret::update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles)
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

bool Turret::fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const
{
	for (const auto& unit : units)
	{
		if (Math::isWithinRange(m_position, unit.getPosition(), m_attackRange))
		{
			Vector2f dir = Math::getDirection(m_position, unit.getPosition());
			projectiles.emplace_back(m_position, dir, ProjectileSender::Turret, static_cast<int>(EntityID::PROJECTILE));
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

//Entity
Unit::Unit(int tileID, const std::vector<Vector2i>& movementPath)
	: m_movementPath(movementPath),
	m_position(movementPath.back()),
	m_sprite(),
	m_active(true),
	m_speed(2.0f)
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

void Unit::update(float deltaTime)
{
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