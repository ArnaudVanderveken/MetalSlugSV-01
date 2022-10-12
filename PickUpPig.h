#pragma once
#include "PickUp.h"
#include "Sprite.h"

class PickUpPig final : public PickUp
{
public:
	PickUpPig(float left, float bottom, float width, float height, Game* const game);
	~PickUpPig() override = default;
	PickUpPig(const PickUpPig& other) = delete;
	PickUpPig& operator=(const PickUpPig& other) = delete;
	PickUpPig(PickUpPig&& other) noexcept = delete;
	PickUpPig& operator=(PickUpPig&& other) noexcept = delete;

	virtual void OnOverlap(WorldDynamic* wd) override;
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

	static constexpr int s_Score{ 1000 };

private:
	Sprite m_Sprite;
};

