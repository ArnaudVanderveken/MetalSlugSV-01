#pragma once
#include "WorldDynamic.h"
#include "Sprite.h"
#include "Vector2f.h"

class Level;

class Character final : public WorldDynamic
{
public:
	enum class State
	{
		Idle, Moving, Jumping, Dying, Dead
	};
	enum class Aim
	{
		Up, Left, Right
	};
	enum class Attack
	{
		None, Bullet, Grenade, Melee
	};

	Character(Game* game, float left, float bottom, float width, float height, float horizontalSpeed, float jumpSpeed);
	~Character() override = default;
	Character(const Character& other) = delete;
	Character& operator=(const Character& other) = delete;
	Character(Character&& other) noexcept = delete;
	Character& operator=(Character&& other) noexcept = delete;
	
	void Update(float elapsedSec) override;
	void Draw() const override;

	[[nodiscard]] Aim GetAim() const;
	[[nodiscard]] bool IsCrouching() const;

	void AddScore(int score);
	void AddGrenades(int grenades);
	void IncPrisonerFreed();
	[[nodiscard]] int GetGrenadeCount() const;
	[[nodiscard]] int GetUp() const;
	[[nodiscard]] int GetScore() const;
	[[nodiscard]] int GetPrisonersFreed() const;

	void FireBullet();
	void FireGrenade();
	void Melee();

	void Kill();
	[[nodiscard]] bool IsVulnerable() const;
	[[nodiscard]] bool IsAlive() const;

private:
	State m_State;
	Aim m_Aim, m_SavedAim;
	Attack m_Attack;
	bool m_IsCrouching;
	Sprite m_StandingSprites[20], m_CrouchingSprites[5];
	Sprite* m_pCurrentSprite{};

	int m_Score, m_Up, m_PrisonersFreed;
	float m_Timer;
	const float m_GracePeriod, m_DeathTime;
	int m_GrenadeCount;
	Vector2f m_GrenadeSpawnOffset;

	void HandleInputs();
	void HandleCollisions();
	void UpdateSprite(float elapsedSec) const;
	void ResetSprite() const;

	void Revive();
};

