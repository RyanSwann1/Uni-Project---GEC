#include "Global.h"
#include <algorithm>

//Vector2i
Vector2i::Vector2i()
	:x(0),
	y(0)
{}

Vector2i::Vector2i(int x, int y)
	: x(x),
	y(y)
{}

bool Vector2i::operator==(Vector2i other)
{
	return x == other.x && y == other.y;
}

Vector2i Vector2i::operator-(Vector2i other)
{
	Vector2i v;
	v.x = x - other.x;
	v.y = y - other.y;

	return v;
}

Rectangle::Rectangle(int left, int width, int top, int height)
	: m_left(left),
	m_right(left + width),
	m_top(top),
	m_bottom(top + height)
{}

int Rectangle::getWidth() const
{
	return m_right - m_left;
}

int Rectangle::getHeight() const
{
	return m_bottom - m_top;
}

int Rectangle::getRight() const
{
	return m_right;
}

int Rectangle::getBottom() const
{
	return m_bottom;
}

int Rectangle::getTop() const
{
	return m_top;
}

bool Rectangle::intersects(Rectangle rect)
{
	if (std::max(m_left, rect.m_left) <= std::min(m_right, rect.m_right) 
		&& std::max(m_top, rect.m_top) <= std::min(m_bottom, rect.m_bottom))
		return true;

	return false;
}

void Rectangle::clipTo(Rectangle rect)
{
	m_left = std::max(m_left, rect.m_left);
	m_top = std::max(m_top, rect.m_top);
	m_right = std::min(m_right, rect.m_right);
	m_bottom = std::min(m_bottom, rect.m_bottom);
}

void Rectangle::translate(int x, int y)
{
	m_left += x;
	m_right += x;
	m_top += y;
	m_bottom += y;
}

Vector2f::Vector2f()
	: x(0),
	y(0)
{}

Vector2f::Vector2f(float x, float y)
	:x(x),
	y(y)
{}

Vector2f Vector2f::operator-(Vector2f other)
{
	Vector2f v;
	v.x = x - other.x;
	v.y = y - other.y;

	return v;
}

Vector2f Vector2f::operator*(float other)
{
	Vector2f v(x, y);
	v.x *= other;
	v.y *= other;

	return v;

	return Vector2f(x * other, y * other);
}

Vector2f Vector2f::operator*=(float amount)
{
	Vector2f vect;
	vect.x *= amount;
	vect.y *= amount;

	return vect;
}

Vector2f Vector2f::operator+=(Vector2f other)
{
	Vector2f vect;
	vect.x += other.x;
	vect.y += other.y;

	return vect;
}