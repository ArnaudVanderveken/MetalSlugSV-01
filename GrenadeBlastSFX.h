#pragma once
#include "SFX.h"

class GrenadeBlastSFX final : public SFX
{
public:
	GrenadeBlastSFX(const Point2f& pos, float scale = 1.0f);
	~GrenadeBlastSFX() override = default;
	GrenadeBlastSFX(const GrenadeBlastSFX& other) = delete;
	GrenadeBlastSFX& operator=(const GrenadeBlastSFX& other) = delete;
	GrenadeBlastSFX(GrenadeBlastSFX&& other) noexcept = delete;
	GrenadeBlastSFX& operator=(GrenadeBlastSFX&& other) noexcept = delete;
};