#include "pch.h"
#include "Helicopter.h"
#include "Game.h"

Helicopter::Helicopter(const float left, const float bottom, const bool isWorldBoss, Game* const game)
	: m_State{ State::Entering }
	, m_pGame{ game }
	, m_Sprites{ { "Resources/Images/Helicopter/Fly.png", 11, 1, 1 / 6.0f },
		{ "Resources/Images/Helicopter/Turn.png" , 13, 1, 1 / 20.0f },
	}
	, m_CruiseHeight{ 150.0f }
	 ,m_MaxSpeed{ 82.0f }
	, m_IdleHeight{ bottom }
	, m_MaxDistanceToTarget{ 50.0f }
	, m_LeaveSpeedCoeff{ 1.1f }
	, m_Attacking{ false }
	, m_GoingLeft{ true }
	, m_IsWorldBoss{ isWorldBoss }
	, m_HitPoints{ 30 }
	, m_TimeBetweenAttacks{ 1.0f }
	, m_TimeBetweenBombs{ 0.25f }
	, m_BombsPerAttack{ 4 }
	, m_Timer{}
	, m_BombsDropped{}
	, m_BombSpawnOffset{ 32.5f, 0.0f }
{
	
	m_pCurrentSprite = m_Sprites;
	m_HitBox = Rectf{ left, bottom, m_pCurrentSprite->GetFrameWidth(), m_pCurrentSprite->GetFrameHeight() };
}

void Helicopter::Update(const Rectf& target, const float elapsedSec)
{
	switch (m_State)
	{
	case State::Turning:
		if (m_Sprites[int(State::Turning)].GetCycle())
		{
			ResetSprite();
			m_State = State::Flying;
			m_pCurrentSprite = &m_Sprites[int(m_State)];
			m_Velocity.x < 0 ? m_GoingLeft = true : m_GoingLeft = false;
		}
		m_Velocity.x = m_MaxSpeed * sinf(PI / 4 * (target.left + target.width / 2 - m_HitBox.left - m_HitBox.width / 2) / m_MaxDistanceToTarget);
		m_HitBox.left += m_Velocity.x * elapsedSec;
		break;

	case State::Flying:
		m_Velocity.x = m_MaxSpeed * sinf(PI / 4 * (target.left + target.width / 2 - m_HitBox.left - m_HitBox.width / 2) / m_MaxDistanceToTarget);
		m_HitBox.left += m_Velocity.x * elapsedSec;

		if ((m_Velocity.x < 0 && !m_GoingLeft) || (m_Velocity.x > 0 && m_GoingLeft))
		{
			ResetSprite();
			m_State = State::Turning;
			m_pCurrentSprite = &m_Sprites[int(m_State)];
		}
		break;

	case State::Entering:
		m_Velocity.y = -m_MaxSpeed * sinf(PI / 4 * (m_HitBox.bottom - m_CruiseHeight* 0.75f)  / m_IdleHeight) * 4;
		m_HitBox.bottom += m_Velocity.y * elapsedSec;

		if (m_HitBox.bottom - m_CruiseHeight < 0.1f)
		{
			m_Velocity.y = 0;
			m_HitBox.bottom = m_CruiseHeight;
			m_State = State::Flying;
		}
		break;

	case State::Leaving:
		m_Velocity.y *= m_LeaveSpeedCoeff;
		m_HitBox.left += m_Velocity.x * elapsedSec;
		m_HitBox.bottom += m_Velocity.y * elapsedSec;
		break;
	}

	if (m_State == State::Flying || m_State == State::Turning)
	{
		if (m_Attacking)
		{
			m_Timer += elapsedSec;
			if (m_Timer >= m_TimeBetweenBombs)
			{
				m_Timer -= m_TimeBetweenBombs;
				m_pGame->SpawnBomb(Point2f{ m_HitBox.left, m_HitBox.bottom } + m_BombSpawnOffset);
				++m_BombsDropped;
				if (m_BombsDropped >= m_BombsPerAttack) m_Attacking = false;
			}
		}
		else
		{
			m_Timer += elapsedSec;
			if (m_Timer >= m_TimeBetweenAttacks)
			{
				m_Timer -= m_TimeBetweenAttacks;
				m_Attacking = true;
				m_BombsDropped = 0;
			}
		}
	}

	UpdateSprite(elapsedSec);
}

void Helicopter::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_HitBox.left, m_HitBox.bottom, 0);
		if (m_GoingLeft)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_HitBox.width, 0, 0);
		}
		m_pCurrentSprite->Draw();
	}
	glPopMatrix();
}

bool Helicopter::IsWorldBoss() const
{
	return m_IsWorldBoss;
}

int Helicopter::GetHP() const
{
	return m_HitPoints;
}

Rectf Helicopter::GetHitBox() const
{
	return m_HitBox;
}

Helicopter::State Helicopter::GetState() const
{
	return m_State;
}

void Helicopter::Hit(const int damages)
{
	m_HitPoints -= damages;
}

void Helicopter::Leave()
{
	m_Velocity.x = m_MaxSpeed;
	m_Velocity.y = m_MaxSpeed / 15.0f;
	m_State = State::Leaving;
}

void Helicopter::UpdateSprite(const float elapsedSec) const
{
	m_pCurrentSprite->Update(elapsedSec);
}

void Helicopter::ResetSprite() const
{
	m_pCurrentSprite->Reset();
}
