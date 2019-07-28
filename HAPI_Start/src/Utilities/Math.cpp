#include "Math.h"

Vector2f Math::getDirection(Vector2f currentPosition, Vector2f destination)
{
	Vector2f vect = Vector2f(destination.x, destination.y) - Vector2f(currentPosition.x, currentPosition.y);
	float length = sqrt(vect.x * vect.x + vect.y * vect.y);

	vect.x /= length;
	vect.y /= length;

	return vect;
}

bool Math::isWithinRange(Vector2f currentPosition, Vector2f targetPosition, float attackRange)
{
	Vector2f vect = targetPosition - currentPosition;
	float length = sqrt(vect.x * vect.x + vect.y * vect.y);
	
	return length <= attackRange;
}