#include "Entity.h"
#include "Window.h"
#include <assert.h>

//Turret
Turret::Turret(Texture & tileSheet)
	: m_position(),
	m_base(tileSheet),
	m_head(tileSheet)
{
}

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
	m_sprite(tileSheet),
	m_active(false)
{
	m_sprite.setID(tileID);
	m_position = m_entityPath.back();
	m_sprite.setPosition(m_entityPath.back());
	m_entityPath.pop_back();
}

void Entity::activate()
{
	assert(!m_active);
	m_active = true;
}

void Entity::update()
{
	if (m_active)
	{

	}
}

void Entity::render(const Window & window)
{
	if (m_active)
	{

	}
}
