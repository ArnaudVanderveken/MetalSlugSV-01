#include "pch.h"
#include "FinalBoss.h"
#include "Game.h"
#include "Level.h"

FinalBoss::FinalBoss(Game* const game)
	: m_pGame{ game }
	, m_HitBox{ 4023, 0, 75, 235 }
	, m_State{ State::Idle }
	, m_CanonBottomRight{ 4077, 160 }
	, m_CurrentStage{}
	, m_BGTextures{ Texture{ "Resources/Images/Buildings/FinalBoss/Stage1.png" },
		Texture{ "Resources/Images/Buildings/FinalBoss/Stage2.png" },
		Texture{ "Resources/Images/Buildings/FinalBoss/Stage3.png" }
	}
	, m_FGTextures{ Texture{ "Resources/Images/Buildings/FinalBoss/Stage1FG.png" },
		Texture{ "Resources/Images/Buildings/FinalBoss/Stage2FG.png" },
		Texture{ "Resources/Images/Buildings/FinalBoss/Stage3FG.png" }
	}
	, m_BGTextureOffset{ -270 , 112 }
	, m_FGTextureOffset{ 22, 160 }
	, m_CanonBallSpawnOffset{ -35, 62 }
	, m_LaserBallSpawnOffset{ -60, 10 }
	, m_CanonSprites{ { "Resources/Images/Buildings/FinalBoss/Canon/Idle.png", 1, 1, 1 },
		{ "Resources/Images/Buildings/FinalBoss/Canon/Opening.png", 9, 1, 1 / 20.0f },
		{ "Resources/Images/Buildings/FinalBoss/Canon/Closing.png", 9, 1, 1 / 20.0f },
		{ "Resources/Images/Buildings/FinalBoss/Canon/Firing.png", 16, 1, 1 / 20.0f },
		{ "Resources/Images/Buildings/FinalBoss/Canon/LoadingLaser.png", 5, 1, 1 / 20.0f },
		{ "Resources/Images/Buildings/FinalBoss/Canon/FiringLaser.png", 9, 1, 1 / 20.0f }
	}
	, m_IsSleeping{ true }
	, m_IsDead{ false }
	, m_TimeBetweenAttacks{ 2.0f }
	, m_TimeBetweenCanonBalls{ 0.2f }
	, m_Timer{}
	, m_CanonBallsPerAttack{ 3 }
	, m_CanonBallsShot{}
	, m_HitsPerStage{ 25 }
	, m_Damages{}
{
}

void FinalBoss::Update(const float elapsedSec)
{
	m_Timer += elapsedSec;
	switch (m_State)
	{
	case State::Idle:
		if (m_Timer >= m_TimeBetweenAttacks && !m_IsDead)
		{
			m_Timer = 0.0f;
			AI();
		}
		break;

	case State::Opening:
		if (m_CanonSprites[int(m_State)].GetCycle())
		{
			ResetSprite();
			m_CanonBallsShot = 0;
			m_State = State::Firing;
		}
		break;

	case State::Closing:
		if (m_CanonSprites[int(m_State)].GetCycle())
		{
			ResetSprite();
			m_Timer = 0.0f;
			m_State = State::Idle;
		}
		break;

	case State::Firing:
		if (m_CanonSprites[int(m_State)].GetCycle())
		{
			ResetSprite();
			m_State = State::Closing;
		}
		else if (m_Timer >= m_TimeBetweenCanonBalls && m_CanonBallsShot <= m_CanonBallsPerAttack)
		{
			m_Timer -= m_TimeBetweenCanonBalls;
			FireCanonBall();
		}
		break;

	case State::LoadingLaser:
		if (m_CanonSprites[int(m_State)].GetCycle() >= 4)
		{
			FireLaserBall();
			ResetSprite();
			m_State = State::FiringLaser;
		}
		break;

	case State::FiringLaser:
		if (m_CanonSprites[int(m_State)].GetCycle())
		{
			ResetSprite();
			m_State = State::Idle;
		}
		break;

	default:
		break;
	}

	UpdateSprite(elapsedSec);
}

void FinalBoss::Draw() const
{
	m_BGTextures[m_CurrentStage].Draw(Point2f{ m_HitBox.left, m_HitBox.bottom } + m_BGTextureOffset);
	glPushMatrix();
	{
		glTranslatef(m_CanonBottomRight.x, m_CanonBottomRight.y, 0);
		glScalef(-1, 1, 1);
		m_CanonSprites[int(m_State)].Draw();
	}
	glPopMatrix();
	m_FGTextures[m_CurrentStage].Draw(Point2f{ m_HitBox.left, m_HitBox.bottom } + m_FGTextureOffset);
}

Rectf FinalBoss::GetHitBox() const
{
	return m_HitBox;
}

bool FinalBoss::IsSleeping() const
{
	return m_IsSleeping;
}

void FinalBoss::WakeUp()
{
	m_IsSleeping = false;
}

void FinalBoss::Hit(const int damages)
{
	m_Damages += damages;
	if (m_Damages >= m_HitsPerStage && m_CurrentStage < m_NrStages - 1)
	{
		m_Damages -= m_HitsPerStage;
		++m_CurrentStage;
	}
	else if (m_CurrentStage == m_NrStages - 1)
	{
		m_IsDead = true;
		m_pGame->GetLevel()->DeleteFinalBossPlatforms();
		m_pGame->EndGame();
	}
}

void FinalBoss::UpdateSprite(const float elapsedSec)
{
	m_CanonSprites[int(m_State)].Update(elapsedSec);
}

void FinalBoss::ResetSprite()
{
	m_CanonSprites[int(m_State)].Reset();
}

void FinalBoss::AI()
{
	const float dstToCharacter{ m_HitBox.left - m_pGame->GetCharacter()->GetHitBox().left };
	if (dstToCharacter <= 50.0f) // 0 - 50
		m_State = (rand() % 5) ? State::LoadingLaser : State::Opening;

	else if (dstToCharacter <= 243.0f) // 50 - 243
		m_State = (rand() % 3) ? State::Opening : State::LoadingLaser;

	//else stay idle
}

void FinalBoss::FireCanonBall()
{
	const float ballVelocityX{ (m_pGame->GetCharacter()->GetHitBox().left - m_HitBox.left) / 0.7744f }; // 0.7744 isn't random, I've done some math ;)
	m_pGame->SpawnCanonBall(m_CanonBottomRight + m_CanonBallSpawnOffset, ballVelocityX);
	++m_CanonBallsShot;
}

void FinalBoss::FireLaserBall() const
{
	m_pGame->SpawnLaserBall(m_CanonBottomRight + m_LaserBallSpawnOffset);
}
