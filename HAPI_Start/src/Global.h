#pragma once
#include <string>

const std::string DATA_DIRECTORY = "Data//";
constexpr int BYTES_PER_PIXEL = 4;


struct Vector2i
{
	Vector2i();
	Vector2i(int x, int y);
	bool operator==(Vector2i other);


	int x;
	int y;
};

struct Rectangle
{
	Rectangle(int left = 0, int width = 0, int top = 0, int height = 0);

	int getWidth() const;
	int getHeight() const;
	int getRight() const;
	int getBottom() const;
	int getTop() const;

	bool intersect(Rectangle rect);
	void clipTo(Rectangle rect);
	void translate(int x, int y);
	
	int m_left;
	int m_right;
	int m_top;
	int m_bottom;
};