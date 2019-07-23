#include "Entity.h"
#include "Window.h"

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