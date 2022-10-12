#pragma once
#include "PickUp.h"
#include "Sprite.h"

class PickUpAmmo : public PickUp
{
public:
	PickUpAmmo(float left, float bottom, float width, float height, Game* const game);
	~PickUpAmmo() override = default;
	PickUpAmmo(const PickUpAmmo& other) = delete;
	PickUpAmmo& operator=(const PickUpAmmo& other) = delete;
	PickUpAmmo(PickUpAmmo&& other) noexcept = delete;
	PickUpAmmo& operator=(PickUpAmmo&& other) noexcept = delete;

	virtual void OnOverlap(WorldDynamic * wd) override;
	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;

private:
	Sprite m_Sprite;
	static constexpr int m_Grenades{ 10 };
};

