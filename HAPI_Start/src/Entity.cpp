#include "Entity.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"

//Projectile
Projectile::Projectile(Vector2i startingPosition, ProjectileSender sentFrom, int damage, int tileID)
	: m_position(startingPosition),
	m_sentFrom(sentFrom),
	m_damage(damage)
{}

//Turret
Turret::Turret()
	: m_position(),
	m_base(),
	m_head(),
	m_attackRange(250.0)
{}

void Turret::render(const Window & window) const
{
	window.render(m_base);
	window.render(m_head);
}

void Turret::update(const std::vector<Unit>& entities, float deltaTime)
{
	for (const auto& entity : entities)
	{
		if (Math::isWithinRange(m_position, entity.getPosition(), m_attackRange))
		{
			Vector2f dir = Math::getDirection(m_position, entity.getPosition());

		}
	}
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