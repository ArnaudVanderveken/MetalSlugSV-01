#pragma once
#include "BossBuilding.h"
#include "Sprite.h"

class Game;

class FinalBoss final
{
public:
	enum class State
	{
		Idle, Opening, Closing, Firing, LoadingLaser, FiringLaser, SIZE
	};
	FinalBoss(Game* game);
	~FinalBoss() = default;
	FinalBoss(const FinalBoss&) = delete;
	FinalBoss& operator=(const FinalBoss&) = delete;
	FinalBoss(FinalBoss&&) noexcept = delete;
	FinalBoss& operator=(FinalBoss&&) noexcept = delete;

	void Update(float elapsedSec);
	void Draw() const;

	[[nodiscard]] Rectf GetHitBox() const;

	[[nodiscard]] bool IsSleeping() const;
	void WakeUp();
	void Hit(int damages);

private:
	Game* m_pGame;
	Rectf m_HitBox;
	State m_State;
	Point2f m_CanonBottomRight;

	int m_CurrentStage;
	static constexpr int m_NrStages{ 3 };
	Texture m_BGTextures[m_NrStages], m_FGTextures[m_NrStages];
	Vector2f m_BGTextureOffset, m_FGTextureOffset, m_CanonBallSpawnOffset, m_LaserBallSpawnOffset;
	Sprite m_CanonSprites[int(State::SIZE)];

	bool m_IsSleeping, m_IsDead;
	const float m_TimeBetweenAttacks, m_TimeBetweenCanonBalls;
	float m_Timer;
	const int m_CanonBallsPerAttack;
	int m_CanonBallsShot;

	const int m_HitsPerStage;
	int m_Damages;

	void UpdateSprite(float elapsedSec);
	void ResetSprite();

	void AI();
	void FireCanonBall();
	void FireLaserBall() const;
};

