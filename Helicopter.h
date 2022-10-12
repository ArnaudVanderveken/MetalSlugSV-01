#pragma once
#include "Vector2f.h"
#include "Sprite.h"

class Texture;
class Game;

class Helicopter final
{
public:
	enum class State
	{
		Flying = 0, Turning = 1, Entering, Leaving
	};

	Helicopter(float left, float bottom, bool isWorldBoss, Game* game);
	~Helicopter() = default;
	Helicopter(const Helicopter&) = delete;
	Helicopter& operator=(const Helicopter&) = delete;
	Helicopter(Helicopter&&) noexcept = delete;
	Helicopter& operator=(Helicopter&&) noexcept = delete;

	void Update(const Rectf& target, float elapsedSec);
	void Draw() const;

	bool IsWorldBoss() const;
	int GetHP() const;
	[[nodiscard]] Rectf GetHitBox() const;
	[[nodiscard]] State GetState() const;

	void Hit(int damages);
	void Leave();

private:
	State m_State;
	Rectf m_HitBox;
	Game* m_pGame;
	Sprite m_Sprites[2];
	Sprite* m_pCurrentSprite{};
	const float m_CruiseHeight, m_MaxSpeed, m_IdleHeight, m_MaxDistanceToTarget, m_LeaveSpeedCoeff;
	Vector2f m_Velocity;
	bool m_Attacking, m_GoingLeft;
	const bool m_IsWorldBoss;
	int m_HitPoints;
	const float m_TimeBetweenAttacks, m_TimeBetweenBombs;
	const int m_BombsPerAttack;
	float m_Timer;
	int m_BombsDropped;
	Vector2f m_BombSpawnOffset;
	
	void UpdateSprite(float elapsedSec) const;
	void ResetSprite() const;
};

