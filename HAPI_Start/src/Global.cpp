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

Rectangle::Rectangle(int left, int right, int top, int bottom)
	: m_left(left),
	m_right(left + right),
	m_top(top),
	m_bottom(top + bottom)
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

bool Rectangle::intersect(Rectangle rect)
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