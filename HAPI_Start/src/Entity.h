#pragma once

#include "Sprite.h"
#include <vector>
#include "Timer.h"
#include <array>

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
	TURRET_MISSLE_HEAD = 204,
	SOILDER_GREEN = 245
};

enum class EntityMoveDirection
{
	Left = 0,
	Right,
	Up,
	Down
};

class Window;
class Texture;
struct Turret
{
	Turret(Texture& tileSheet);

	void setPosition(Vector2i position);

	void render(const Window& window) const;

	Vector2i m_position;
	Sprite m_base;
	Sprite m_head;
};

class Entity
{
public:
	Entity(Texture& tileSheet, int tileID, const std::vector<Vector2i>& entityPath);

	bool isActive() const;

	void update(float deltaTime);
	void render(const Window& window) const;

private:
	std::vector<Vector2i> m_entityPath;
	Vector2i m_position;
	Sprite m_sprite;
	bool m_active;
	float m_speed;
	EntityMoveDirection m_moveDirection;
};