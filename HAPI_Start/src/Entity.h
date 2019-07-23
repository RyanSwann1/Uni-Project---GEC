#pragma once

#include "Sprite.h"

enum class TurretType
{
	Cannon = 0,
	Missle
};

enum class EntityID
{
	TURRET_CANNON_BASE = 180,
	TURRET_CANNON_HEAD = 249,
	TURRET_MISSLE_BASE = 181,
	TURRET_MISSLE_HEAD = 204
};

class Window;
class Texture;
struct Turret
{
	Turret(Texture& tileSheet);

	void render(const Window& window) const;

	void setPosition(Vector2i position);

	Vector2i m_position;
	Sprite m_base;
	Sprite m_head;
};