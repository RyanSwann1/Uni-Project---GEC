#include "Particle.h"
#include "Texture.h"
#include "Window.h"

constexpr float EXPIRATION_TIME = 0.5f;

Particle::Particle(Vector2f startingPosition, int tileID)
	: m_sprite(startingPosition, tileID),
	m_position(startingPosition),
	m_aliveTimer(EXPIRATION_TIME, true)
{}

bool Particle::isExpired() const
{
	return m_aliveTimer.isExpired();
}

void Particle::update(float deltaTime)
{
	m_aliveTimer.update(deltaTime);
}

void Particle::render(Window & window) const
{
	window.render(m_sprite);
}