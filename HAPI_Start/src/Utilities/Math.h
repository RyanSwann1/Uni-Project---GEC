#pragma once

#include "../Global.h"
#include "../Entity.h"

namespace Math
{
	UnitMoveDirection getDirectionTowards(Vector2i currentPosition, Vector2i destination);
	Vector2f getDirection(Vector2i currentPosition, Vector2i destination);
	bool isWithinRange(Vector2i currentPosition, Vector2i targetPosition, float attackRange);
}