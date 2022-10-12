#include "pch.h"
#include "BombImpactSFX.h"

BombImpactSFX::BombImpactSFX(const Point2f& pos, const float scale)
	: SFX{ "Resources/Images/SFX/BombImpact.png", 8, 1, 1 / 24.0f, pos, scale }
{
}