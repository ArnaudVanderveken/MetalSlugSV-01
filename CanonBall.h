#pragma once
#include "WorldDynamic.h"

class Texture;

class CanonBall final : public WorldDynamic
{
public:
	CanonBall(Game* game, const Point2f& startPoint, Texture* texture, float velocityX);
	~CanonBall() override = default;
	CanonBall(const CanonBall&) = delete;
	CanonBall& operator=(const CanonBall&) = delete;
	CanonBall(CanonBall&&) noexcept = delete;
	CanonBall& operator=(CanonBall&&) noexcept = delete;

	virtual void Draw() const override;

	bool IsOnGround();

private:
	Texture* const m_pTexture;
};

