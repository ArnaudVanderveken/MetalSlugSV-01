#include "pch.h"
#include "PickUp.h"

PickUp::PickUp(const float left, const float bottom, const float width, const float height, Game* const game)
	: WorldStatic(left, bottom, width, height, game)
{
}
