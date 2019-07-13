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

//Rectangle
Rectangle::Rectangle()
	: m_position(),
	m_size()
{}

Rectangle::Rectangle(Vector2i startingPosition, Vector2i startingSize)
	: m_position(startingPosition),
	m_size(startingSize)
{}

//clipped_rect.left = MAX(rect_a.left, rect_b.left);
//clipped_rect.right = MIN(rect_a.right, rect_b.right);
//clipped_rect.top = MIN(rect_a.top, rect_b.top);
//clipped_rect.bottom = MAX(rect_a.bottom, rect_b.bottom);

void Rectangle::clipTo(Rectangle rect)
{
	m_left = std::max(m_left, rect.m_left);
	m_top = std::max(m_top, rect.m_top);
	m_position.y = std::max(m_, rect.m_position.y);

	//Left
	if (m_position.x < rect.m_position.x)
	{
		m_position.x = rect.m_position.x;
	}
	if (m_size.x < rect.m_position.x)
	{
		m_size.x = rect.m_position.x;
	}

	//Right
	if (m_position.x > rect.m_size.x)
	{
		m_position.x = rect.m_size.x;
	}
	if (m_size.x > rect.m_size.x)
	{
		m_size.x = rect.m_size.x;
	}

	//Top
	if (m_position.y < rect.m_position.y)
	{
		m_position.y = rect.m_position.y;
	}
	if (m_size.y < rect.m_position.y)
	{
		m_size.y = rect.m_position.y;
	}

	//Bottom
	if (m_position.y > rect.m_size.y)
	{
		m_position.y = rect.m_size.y;
	}
	if (m_size.y > rect.m_size.y)
	{
		m_size.y = rect.m_size.y;
	}
}

void Rectangle::translate(int x, int y)
{
	m_position.x += x;
	m_position.y += y;
}