#include "Entity.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"

//Turret
Turret::Turret(Texture & tileSheet)
	: m_position(),
	m_base(tileSheet),
	m_head(tileSheet)
{}

void Turret::render(const Window & window) const
{
	window.render(m_base);
	window.render(m_head);
}

void Turret::setPosition(Vector2i position)
{
	m_position = position;
	m_base.setPosition(position);
	m_head.setPosition(position);
}

//Entity
Entity::Entity(Texture & tileSheet, int tileID, const std::vector<Vector2i>& entityPath)
	: m_entityPath(entityPath),
	m_position(entityPath.back()),
	m_sprite(tileSheet),
	m_active(true),
	m_speed(2.0f)
{
	m_entityPath.pop_back();
	m_moveDirection = Math::getDirectionTowards(m_position, m_entityPath.back());
	m_sprite.setID(tileID);
	m_sprite.setPosition(m_position);
}

bool Entity::isActive() const
{
	return m_active;
}

void Entity::update(float deltaTime)
{
	//Move to destination
	bool reachedDestination = false;
	switch (m_moveDirection)
	{
	case EntityMoveDirection::Up:
		m_position.y -= m_speed;
		m_sprite.setPosition(m_position);
		if (m_position.y <= m_entityPath.back().y)
		{
			reachedDestination = true;
		}
		break;

	case EntityMoveDirection::Right:
		m_position.x += m_speed;
		m_sprite.setPosition(m_position);
		if (m_position.x >= m_entityPath.back().x)
		{
			reachedDestination = true;
		}
		break;
	}
	
	//Assign new destination
	if (reachedDestination)
	{
		m_position = m_entityPath.back();
		m_entityPath.pop_back();
		if (m_entityPath.empty())
		{
			m_active = false;
			return;
		}

		m_moveDirection = Math::getDirectionTowards(m_position, m_entityPath.back());
	}
}

void Entity::render(const Window & window) const
{
	window.render(m_sprite);	
}