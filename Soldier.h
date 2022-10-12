#pragma once
#include "WorldDynamic.h"
#include "Vector2f.h"
#include "Sprite.h"

class Soldier final : public WorldDynamic
{
public:
	enum class State
	{
		Idle = 0, Running = 1, Attacking = -2, Attacked, Fleeing = -3, Dying = 4, Dead = 5
	};
	enum class Aim
	{
		Left = 1, Right = -1
	};

	Soldier(Game* game, float left, float bottom);
	~Soldier() override = default;
	Soldier(const Soldier&) = delete;
	Soldier& operator=(const Soldier&) = delete;
	Soldier(Soldier&&) noexcept = delete;
	Soldier& operator=(Soldier&&) noexcept = delete;

	void Update(float elapsedSec) override;
	void Draw() const override;

	[[nodiscard]] State GetState() const;
	void Kill();

	static constexpr int s_Score{ 2000 };

private:
	State m_State;
	Aim m_Aim;
	Sprite m_Sprites[6];
	Sprite* m_pCurrentSprite{};

	Vector2f m_GrenadeSpawnOffset;
	int m_ThrowGrenadeAtFrame;

	void UpdateSprite(float elapsedSec) const;
	void ResetSprite() const;

	void AI();
};

