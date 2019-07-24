#pragma once

#include "../Global.h"
#include "../Entity.h"

namespace Math
{
	EntityMoveDirection getDirectionTowards(Vector2i currentPosition, Vector2i destination);
}