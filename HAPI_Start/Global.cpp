#include "Global.h"

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

void Rectangle::clipTo(Rectangle rect)
{
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
