#include "Particle.h"
#include "Texture.h"
#include "Window.h"

constexpr float EXPIRATION_TIME = 0.5f;

Particle::Particle(Vector2i startingPosition, int tileID)
	: m_active(true),
	m_sprite(startingPosition, tileID),
	m_position(startingPosition),
	m_aliveTimer(EXPIRATION_TIME, true)
{}

bool Particle::isActive() const
{
	return m_active;
}

void Particle::update(float deltaTime)
{
	m_aliveTimer.update(deltaTime);
	if (m_aliveTimer.isExpired())
	{
		m_active = false;
	}
}

void Particle::render(Window & window) const
{
	window.render(m_sprite);
}
