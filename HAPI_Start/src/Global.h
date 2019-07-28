#pragma once

#include <string>

const std::string DATA_DIRECTORY = "Data//";
constexpr int BYTES_PER_PIXEL = 4;

enum class eTileID
{
	INVALID = -1,
	TURRET_CANNON_BASE = 180,
	TURRET_CANNON_HEAD = 249,
	TURRET_MISSLE_BASE = 181,
	TURRET_MISSLE_HEAD = 204,
	SOILDER_GREEN = 245,
	TANK_BASE = 268,
	TANK_HEAD = 291,
	PLANE = 271,
	PLANE_SHADOW = 294,
	PROJECTILE = 272,
	PARTICLE = 295,
	SELECTOR = 15
};

enum class eGameDifficulty
{
	NORMAL = 0,
	HARD,
	EXTREME
};

struct Vector2i
{
	Vector2i();
	Vector2i(int x, int y);
	bool operator==(Vector2i other);
	Vector2i operator-(Vector2i other);

	int x;
	int y;
};

struct Vector2f
{
	Vector2f();
	Vector2f(float x, float y);
	Vector2f operator-(Vector2f);
	Vector2f operator*(float);
	Vector2f operator*=(float);
	Vector2f operator+=(Vector2f);
	bool operator==(Vector2f);

	float x;
	float y;
};

struct Rectangle
{
	Rectangle(int left = 0, int width = 0, int top = 0, int height = 0);

	int getWidth() const;
	int getHeight() const;
	int getRight() const;
	int getBottom() const;
	int getTop() const;

	bool intersects(Rectangle rect);
	void clipTo(Rectangle rect);
	void translate(int x, int y);
	
	int m_left;
	int m_right;
	int m_top;
	int m_bottom;
};