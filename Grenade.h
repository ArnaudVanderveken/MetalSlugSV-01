#pragma once
#include "WorldDynamic.h"

class Texture;

class Grenade final : public WorldDynamic
{
public:
	Grenade(Game* game, const Point2f& startPoint, Texture* texture, int direction);
	~Grenade() override = default;
	Grenade(const Grenade&) = delete;
	Grenade& operator=(const Grenade&) = delete;
	Grenade(Grenade&&) noexcept = delete;
	Grenade& operator=(Grenade&&) noexcept = delete;

	void Update(float elapsedSec) override;
	void Draw() const override;

	bool IsOnGround();

	static constexpr int s_Dmg{ 5 };

private:
	Texture* const m_pTexture;
	const float m_AngularVelocity;
	float m_Angle;
};

