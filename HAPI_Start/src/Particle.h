#pragma once

#include "Sprite.h"
#include "Timer.h"

class Window;
class Particle
{
public:
	Particle(Vector2i startingPosition, int tileID);

	bool isActive() const;

	void update(float deltaTime);
	void render(Window& window) const;

private:
	bool m_active;
	Sprite m_sprite;
	Vector2i m_position;
	Timer m_aliveTimer;
};