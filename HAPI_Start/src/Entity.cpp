#include "Entity.h"
#include "Window.h"

Turret::Turret()
	: m_position(),
	m_base(),
	m_head()
{}

Turret::Turret(int baseID, int headID, Texture & tileSheet, Vector2i startingPosition)
	: m_position(startingPosition),
	m_base(tileSheet, startingPosition, baseID),
	m_head(tileSheet, startingPosition, headID)
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