#include "pch.h"
#include "Prisoner.h"
#include "Game.h"
#include "Level.h"

Prisoner::Prisoner(Game* const game, const float left, const float bottom)
	: WorldDynamic(game, left, bottom)
	, m_State{ State::Idle }
	, m_Sprites{ { "Resources/Images/Prisoner/Idle.png", 5, 1, 1 / 5.0f },
				 { "Resources/Images/Prisoner/BreakingRope.png", 7, 1, 1 / 20.0f },
				 { "Resources/Images/Prisoner/Wandering.png", 12, 1, 1 / 12.0f },
				 { "Resources/Images/Prisoner/Dropping.png", 11, 1, 1 / 16.0f },
				 { "Resources/Images/Prisoner/Running.png", 8, 1, 1 / 20.0f }
			   }
	, m_StartingPoint{ left, bottom }
	, m_MaxWanderDistance{ 30.0f }
	, m_PickUpSpawnOffset{ 10.0f, 0.0f }
{
	m_pCurrentSprite = &m_Sprites[int(m_State)];
}

void Prisoner::Update(const float elapsedSec)
{
	switch (m_State)
	{
	case State::Idle:
		break;

	case State::BreakingRope:
		if (m_pCurrentSprite->GetCycle())
		{
			m_State = State::Wandering;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[int(m_State)];
			m_Velocity.x = m_HorizontalSpeed / 2;
		}
		break;

	case State::Wandering:
		if (fabsf(m_HitBox.left - m_StartingPoint.x) >= m_MaxWanderDistance)
			m_Velocity.x *= -1;
		WorldDynamic::Update(elapsedSec);
		m_pGame->GetLevel()->HandleCollision(this);
		break;

	case State::Dropping:
		if (m_pCurrentSprite->GetCycle())
		{
			m_State = State::Running;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[int(m_State)];
			m_Velocity.x = m_HorizontalSpeed;
			m_pGame->SpawnPickUp(Point2f{ m_HitBox.left, m_HitBox.bottom } + m_PickUpSpawnOffset);
		}
		break;

	case State::Running:
		WorldDynamic::Update(elapsedSec);
		m_pGame->GetLevel()->HandleCollision(this);
		break;
	}

	UpdateSprite(elapsedSec);
}

void Prisoner::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_HitBox.left, m_HitBox.bottom, 0);
		if (m_Velocity.x > 0)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_HitBox.width, 0, 0);
		}
		m_pCurrentSprite->Draw();
	}
	glPopMatrix();
}

Prisoner::State Prisoner::GetState() const
{
	return m_State;
}

void Prisoner::UpdateSprite(const float elapsedSec) const
{
	m_pCurrentSprite->Update(elapsedSec);
}

void Prisoner::ResetSprite() const
{
	m_pCurrentSprite->Reset();
}

void Prisoner::Free()
{
	m_State = State::BreakingRope;
	ResetSprite();
	m_pCurrentSprite = &m_Sprites[int(m_State)];
}

void Prisoner::OnOverlap()
{
	if (m_State == State::Wandering)
	{
		m_State = State::Dropping;
		ResetSprite();
		m_pCurrentSprite = &m_Sprites[int(m_State)];
		m_Velocity.x = 0;
	}
}
