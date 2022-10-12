#include "pch.h"
#include "GrenadeBlastSFX.h"

GrenadeBlastSFX::GrenadeBlastSFX(const Point2f& pos, const float scale)
	: SFX{ "Resources/Images/SFX/GrenadeBlast.png", 27, 1, 1 / 40.0f, pos, scale }
{
}
