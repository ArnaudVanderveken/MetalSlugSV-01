#pragma once
#include "WorldDynamic.h"
#include "Sprite.h"
#include "Vector2f.h"

class Prisoner final : public WorldDynamic
{
public:
	enum class State
	{
		Idle, BreakingRope, Wandering, Dropping, Running
	};

	Prisoner(Game* game, float left, float bottom);
	~Prisoner() override = default;
	Prisoner(const Prisoner&) = delete;
	Prisoner& operator=(const Prisoner&) = delete;
	Prisoner(Prisoner&&) noexcept = delete;
	Prisoner& operator=(Prisoner&&) noexcept = delete;

	void Update(float elapsedSec) override;
	void Draw() const override;

	[[nodiscard]] State GetState() const;

	void Free();
	void OnOverlap();

private:
	State m_State;
	Sprite m_Sprites[5];
	Sprite* m_pCurrentSprite{};

	Point2f m_StartingPoint;
	float m_MaxWanderDistance;

	Vector2f m_PickUpSpawnOffset;

	void UpdateSprite(float elapsedSec) const;
	void ResetSprite() const;

};

