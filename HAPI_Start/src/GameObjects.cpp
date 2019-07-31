#include "GameObjects.h"
#include "Window.h"
#include <assert.h>
#include "Utilities/Math.h"
#include "Texture.h"

constexpr float TIME_BETWEEN_TURRET_SHOT = 2.5f;
constexpr float TIME_BETWEEN_UNIT_SHOT = 1.0f;

constexpr float TURRET_PROJECTLE_SPEED = 7.5f;
constexpr float TURRET_ATTACK_RANGE = 350.f;
constexpr int TURRET_MAX_HEALTH = 5;
constexpr int TURRET_DAMAGE = 1;

constexpr float UNIT_ATTACK_RANGE = 250.f;
constexpr float UNIT_PROJECTILE_SPEED = 2.5f;

constexpr int TANK_MAX_HEALTH = 3;
constexpr int TANK_DAMAGE_VALUE = 2;
constexpr float TANK_MOVEMENT_SPEED = 1.0f;

constexpr int SOILDER_MAX_HEALTH = 1;
constexpr float SOILDER_MOVEMENT_SPEED = 1.5f;

constexpr int PLANE_MAX_HEALTH = 2;
constexpr float PLANE_MOVEMENT_SPEED = 2.0f;

constexpr float GAME_DIFFICULTY_MODIFIER_EASY = 1.0f;
constexpr float GAME_DIFFICULTY_MODIFIER_MEDIUM = 1.2f;
constexpr float GAME_DIFFICULTY_MODIFIER_HARD = 1.4f;

constexpr int HEALTH_BAR_Y_OFFSET = 20;

int getHealthBarSize(int currentHealth, int maxHealth, int originalHealthBarWidth)
{
	float h = currentHealth;
	float mx = maxHealth;
	float orig = originalHealthBarWidth;
	float healthPercent = h / mx;
	float a = ((healthPercent / 100) * orig) * 100;

	return a;
}

//Projectile
Projectile::Projectile(Vector2f startingPosition, Vector2f startingDirection, eProjectileSender sentFrom, 
	int tileID, float speed, int damage)
	: m_position(startingPosition),
	m_sentFrom(sentFrom),
	m_speed(speed),
	m_sprite(Textures::getInstance().getTileSheet(), startingPosition, tileID),
	m_direction(startingDirection),
	m_damage(damage)
{}

int Projectile::getDamageValue() const
{
	return m_damage;
}

eProjectileSender Projectile::getSentFrom() const
{
	return m_sentFrom;
}

Vector2f Projectile::getPosition() const
{
	return m_position;
}

void Projectile::update(float deltaTime, const std::vector<Unit>& units)
{
	m_position.x += m_direction.x * m_speed;
	m_position.y += m_direction.y * m_speed;

	m_sprite.setPosition(m_position);
}

void Projectile::render(const Window & window) const
{
	window.render(m_sprite);
}

//Turret
Turret::Turret(Vector2f startingPosition)
	: m_position(startingPosition),
	m_baseSprite(Textures::getInstance().getTileSheet()),
	m_headSprite(Textures::getInstance().getTileSheet()),
	m_healthBarSprite(Textures::getInstance().getHealthBar(), Vector2f(startingPosition.x, startingPosition.y - HEALTH_BAR_Y_OFFSET), 
		static_cast<int>(eTileID::DEFAULT)),
	m_attackRange(TURRET_ATTACK_RANGE),
	m_fireTimer(TIME_BETWEEN_TURRET_SHOT, true),
	m_active(false),
	m_health(TURRET_MAX_HEALTH)
{}

void Turret::render(const Window & window) const
{
	if (m_active)
	{
		window.render(m_baseSprite);
		window.render(m_headSprite);
		window.render(m_healthBarSprite);
	}
}

bool Turret::isActive() const
{
	return m_active;
}

Vector2f Turret::getPosition() const
{
	return m_position;
}

void Turret::setTurret(eTurretType turretType, Vector2f position)
{
	m_active = true;

	assert(m_position == position);
	m_position = position;
	m_baseSprite.setPosition(position);
	m_headSprite.setPosition(position);

	switch (turretType)
	{
	case eTurretType::Cannon:
		m_baseSprite.setID(static_cast<int>(eTileID::TURRET_CANNON_BASE));
		m_headSprite.setID(static_cast<int>(eTileID::TURRET_CANNON_HEAD));
		break;

	case eTurretType::Missle:
		m_baseSprite.setID(static_cast<int>(eTileID::TURRET_MISSLE_BASE));
		m_headSprite.setID(static_cast<int>(eTileID::TURRET_MISSLE_HEAD));
		break;
	}
}

void Turret::damage(int damageValue)
{
	m_health -= damageValue;
	int size = getHealthBarSize(m_health, TURRET_MAX_HEALTH, m_healthBarSprite.getOriginalSize().x);
	m_healthBarSprite.setWidth( getHealthBarSize(m_health, TURRET_MAX_HEALTH, m_healthBarSprite.getOriginalSize().x));

	if (m_health <= 0)
	{
		m_health = 0;
		m_active = false;
	}
}

void Turret::update(float deltaTime, const std::vector<Unit>& units, std::vector<Projectile>& projectiles)
{
	if (m_active)
	{
		m_fireTimer.update(deltaTime);
		if (m_fireTimer.isExpired() && fire(units, projectiles))
		{
			m_fireTimer.reset();
		}
	}
}

bool Turret::fire(const std::vector<Unit>& units, std::vector<Projectile>& projectiles) const
{
	for (const auto& unit : units)
	{
		if (Math::isWithinRange(m_position, unit.getPosition(), m_attackRange))
		{
			int xOffset = 0;
			if (unit.getMoveDirection().x == 1)
			{
				xOffset += 40;
			}
			Vector2f dir = Math::getDirection(m_position, { unit.getPosition().x + xOffset, unit.getPosition().y });
			projectiles.emplace_back(m_position, dir, eProjectileSender::Turret, static_cast<int>(eTileID::PROJECTILE), TURRET_PROJECTLE_SPEED, TURRET_DAMAGE);
			return true;
		}
	}

	return false;
}

//Unit
Unit::Unit(int baseTileID, int headTileID, const std::vector<Vector2i>& movementPath, eUnitType unitType, eGameDifficulty gameDifficulty)
	: m_movementPath(movementPath),
	m_position(static_cast<float>(movementPath.back().x), static_cast<float>(movementPath.back().y)),
	m_baseSprite(Textures::getInstance().getTileSheet(), m_position, baseTileID),
	m_headSprite(Textures::getInstance().getTileSheet(), m_position, headTileID),
	m_healthBarSprite(Textures::getInstance().getHealthBar(), Vector2f(m_movementPath.back().x, m_movementPath.back().y - HEALTH_BAR_Y_OFFSET), 
		static_cast<int>(eTileID::DEFAULT)),
	m_active(true),
	m_speed(0),
	m_attackRange(UNIT_ATTACK_RANGE),
	m_fireTimer(TIME_BETWEEN_UNIT_SHOT, true),
	m_health(0),
	m_unitType(unitType),
	m_damage(0)
{
	switch (m_unitType)
	{
	case eUnitType::Footman :
		m_health = SOILDER_MAX_HEALTH;
		m_speed = SOILDER_MOVEMENT_SPEED;
		m_movementPath.pop_back();
		break;
	
	case eUnitType::Vechile :
		m_health = TANK_MAX_HEALTH;
		m_damage = TANK_DAMAGE_VALUE;
		m_speed = TANK_MOVEMENT_SPEED;
		m_movementPath.pop_back();
		break;
	
	case eUnitType::Aircraft :
		m_health = PLANE_MAX_HEALTH;
		m_speed = PLANE_MOVEMENT_SPEED;
		size_t movementPathSize = m_movementPath.size();
		for (int i = 0; i < movementPathSize - 1; ++i)
		{
			m_movementPath.pop_back();
		}
		break;
	}

	switch (gameDifficulty)
	{
	case eGameDifficulty::NORMAL:
		m_speed *= GAME_DIFFICULTY_MODIFIER_EASY;
		break;

	case eGameDifficulty::HARD:
		m_speed *= GAME_DIFFICULTY_MODIFIER_MEDIUM;
		break;

	case eGameDifficulty::EXTREME:
		m_speed *= GAME_DIFFICULTY_MODIFIER_HARD;
		break;
	}

	
	m_moveDirection = Math::getDirection(m_position, 
		Vector2f(m_movementPath.back().x, m_movementPath.back().y));
}

Vector2f Unit::getMoveDirection() const
{
	return m_moveDirection;
}

Vector2f Unit::getPosition() const
{
	return m_position;
}

bool Unit::isActive() const
{
	return m_active;
}

void Unit::damage(int damageValue)
{
	m_health -= damageValue;
	int i = getHealthBarSize(m_health, TURRET_MAX_HEALTH, m_healthBarSprite.getOriginalSize().x);
	m_healthBarSprite.setWidth(getHealthBarSize(m_health, TURRET_MAX_HEALTH, m_healthBarSprite.getOriginalSize().x));
	if (m_health <= 0)
	{
		m_health = 0;
		m_active = false;
	}
}

void Unit::update(float deltaTime, const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles)
{
	//Fire Weapon
	if (m_unitType == eUnitType::Vechile)
	{
		m_fireTimer.update(deltaTime);
		if (m_fireTimer.isExpired() && (fire(turrets, projectiles)))
		{
			m_fireTimer.reset();
		}
	}

	//Update Position
	m_position.x += m_moveDirection.x * m_speed;
	m_position.y += m_moveDirection.y * m_speed;
	
	m_baseSprite.setPosition(m_position);
	m_headSprite.setPosition(m_position);
	m_healthBarSprite.setPosition(m_position);
	
	if (m_unitType == eUnitType::Aircraft)
	{
		Vector2i tileSize = m_baseSprite.getTexture().getTileSize();
		Vector2f newBasePosition(m_position.x + tileSize.x, m_position.y + tileSize.y);
		m_baseSprite.setPosition(newBasePosition);
	}

	//Check if reached destination
	bool reachedDestination = false;
	if (m_moveDirection.x >= 0 && m_moveDirection.y <= 0 &&
		m_position.x >= m_movementPath.back().x && m_position.y <= m_movementPath.back().y)
	{
		reachedDestination = true;
	}
	else if (m_moveDirection.x >= 0 && m_moveDirection.y >= 0 &&
		m_position.x >= m_movementPath.back().x && m_position.y >= m_movementPath.back().y)
	{
		reachedDestination = true;
	}
	else if (m_moveDirection.x <= 0 && m_moveDirection.y >= 0 &&
		m_position.x >= m_movementPath.back().y && m_position.y >= m_movementPath.back().y)
	{
		reachedDestination = true;
	}
	else if (m_moveDirection.x <= 0 && m_moveDirection.y <= 0 &&
		m_position.x <= m_movementPath.back().y && m_position.y <= m_movementPath.back().y)
	{
		reachedDestination = true;
	}
	

	//Assign new destination
	if (reachedDestination)
	{
		m_position = Vector2f(m_movementPath.back().x, m_movementPath.back().y);
		m_baseSprite.setPosition(m_position);
		m_headSprite.setPosition(m_position);
		m_movementPath.pop_back();
		if (m_movementPath.empty())
		{
			m_active = false;
			return;
		}

		m_moveDirection = Math::getDirection(m_position, Vector2f(m_movementPath.back().x, m_movementPath.back().y));
	}
}

void Unit::render(const Window & window) const
{
	window.render(m_baseSprite);
	window.render(m_headSprite);
	window.render(m_healthBarSprite);
}

bool Unit::fire(const std::vector<Turret>& turrets, std::vector<Projectile>& projectiles) const
{
	for (const auto& turret : turrets)
	{
		if (turret.isActive() && Math::isWithinRange(m_position, turret.getPosition(), m_attackRange))
		{
			Vector2f dir = Math::getDirection(m_position, turret.getPosition());
			projectiles.emplace_back(m_position, dir, eProjectileSender::Unit, static_cast<int>(eTileID::PROJECTILE), UNIT_PROJECTILE_SPEED, m_damage);
			return true;
		}
	}

	return false;
}