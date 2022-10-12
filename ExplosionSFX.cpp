#include "pch.h"
#include "ExplosionSFX.h"

ExplosionSFX::ExplosionSFX(const Point2f& pos, const float scale)
	: SFX{ "Resources/Images/SFX/Explosion.png", 28, 1, 1 / 40.0f, pos, scale }
{
}