#pragma once
#include "SFX.h"

class ExplosionSFX final : public SFX
{
public:
	ExplosionSFX(const Point2f& pos, const float scale = 1.0f);
	~ExplosionSFX() override = default;
	ExplosionSFX(const ExplosionSFX& other) = delete;
	ExplosionSFX& operator=(const ExplosionSFX& other) = delete;
	ExplosionSFX(ExplosionSFX&& other) noexcept = delete;
	ExplosionSFX& operator=(ExplosionSFX&& other) noexcept = delete;
};

