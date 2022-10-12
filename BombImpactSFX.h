#pragma once
#include "SFX.h"

class BombImpactSFX final : public SFX
{
public:
	BombImpactSFX(const Point2f& pos, float scale = 1.0f);
	~BombImpactSFX() override = default;
	BombImpactSFX(const BombImpactSFX& other) = delete;
	BombImpactSFX& operator=(const BombImpactSFX& other) = delete;
	BombImpactSFX(BombImpactSFX&& other) noexcept = delete;
	BombImpactSFX& operator=(BombImpactSFX&& other) noexcept = delete;
};

