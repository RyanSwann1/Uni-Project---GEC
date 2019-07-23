#pragma once

#include "Sprite.h"

enum class TurretType
{
	Cannon = 0,
	Missle
};

class Window;
class Texture;
struct Turret
{
	Turret();
	Turret(int baseID, int headID, Texture& tileSheet, Vector2i startingPosition);

	void render(const Window& window) const;

	void setPosition(Vector2i position);

	Vector2i m_position;
	Sprite m_base;
	Sprite m_head;
};