#pragma once

#include "../Global.h"
#include "../GameObjects.h"

namespace Math
{
	Vector2f getDirection(Vector2f currentPosition, Vector2f destination);
	bool isWithinRange(Vector2f currentPosition, Vector2f targetPosition, float attackRange);
}