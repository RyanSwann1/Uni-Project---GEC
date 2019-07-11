#pragma once
#include <string>

const std::string DATA_DIRECTORY = "Data//";
constexpr int BYTES_PER_PIXEL = 4;

struct Vector2i
{
	Vector2i();
	Vector2i(int x, int y);

	int x;
	int y;
};

struct Rectangle
{
	Rectangle();
	Rectangle(Vector2i startingPosition, Vector2i startingSize);

	void clipTo(Rectangle rect);

	Vector2i m_position;
	Vector2i m_size;
};