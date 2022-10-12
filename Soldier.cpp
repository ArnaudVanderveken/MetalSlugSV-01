#include "pch.h"
#include "Soldier.h"
#include "Game.h"
#include "Level.h"

Soldier::Soldier(Game* const game, const float left, const float bottom)
	: WorldDynamic(game, left, bottom)
	, m_State{ State::Idle }
	, m_Aim{ Aim::Left }
	, m_Sprites{ { "Resources/Images/Soldier/Idle.png", 6, 1, 1 / 6.0f },
				 { "Resources/Images/Soldier/Run.png", 12, 1, 1 / 20.0f },
				 { "Resources/Images/Soldier/Attack.png", 14, 1, 1 / 20.0f },
				 { "Resources/Images/Soldier/Flee.png", 14, 1, 1 / 20.0f },
				 { "Resources/Images/Soldier/Death.png", 11, 1, 1 / 20.0f },
				 { "Resources/Images/Soldier/Dead.png", 1, 1, 1 },
			   }
	, m_GrenadeSpawnOffset{ 20.0f, 40.0f }
	, m_ThrowGrenadeAtFrame{ 11 }
{
	m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
}

void Soldier::Update(const float elapsedSec)
{
	WorldDynamic::Update(elapsedSec);
	m_pGame->GetLevel()->HandleCollision(this);
	switch (m_State)
	{
	case State::Idle:
		AI();
		break;

	case State::Running:
		if (m_pCurrentSprite->GetCycle())
		{
			m_State = State::Idle;
			m_pGame->GetCharacter()->GetHitBox().left < m_HitBox.left ? m_Aim = Aim::Left : m_Aim = Aim::Right;
			m_Velocity.x = 0;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
		break;

	case State::Attacking:
		if (m_pCurrentSprite->GetFrameNumber() >= m_ThrowGrenadeAtFrame)
		{
			m_State = State::Attacked;
			m_pGame->SpawnGrenade(Point2f{ m_HitBox.left, m_HitBox.bottom } + m_GrenadeSpawnOffset, -int(m_Aim));
		}
		break;

	case State::Attacked:
		if (m_pCurrentSprite->GetCycle())
		{
			m_State = State::Idle;
			m_pGame->GetCharacter()->GetHitBox().left < m_HitBox.left ? m_Aim = Aim::Left : m_Aim = Aim::Right;
			m_Velocity.x = 0;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
		break;

	case State::Fleeing:
		if (m_pCurrentSprite->GetCycle() >= 2)
		{
			m_State = State::Idle;
			m_pGame->GetCharacter()->GetHitBox().left < m_HitBox.left ? m_Aim = Aim::Left : m_Aim = Aim::Right;
			m_Velocity.x = 0;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
		break;

	case State::Dying:
		if (m_pCurrentSprite->GetCycle())
		{
			m_State = State::Dead;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
		break;

	case State::Dead:
		break;
	}

	UpdateSprite(elapsedSec);
}

void Soldier::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_HitBox.left, m_HitBox.bottom, 0);
		if (int(m_Aim) != (int(m_State) == 0 ? 1 : (int(m_State) / abs(int(m_State)))))
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_HitBox.width, 0, 0);
		}
		m_pCurrentSprite->Draw();
	}
	glPopMatrix();
}

Soldier::State Soldier::GetState() const
{
	return m_State;
}

void Soldier::Kill()
{
	m_State = State::Dying;
	m_Velocity.x = 0;
	ResetSprite();
	m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
}

void Soldier::UpdateSprite(const float elapsedSec) const
{
	m_pCurrentSprite->Update(elapsedSec);
}

void Soldier::ResetSprite() const
{
	m_pCurrentSprite->Reset();
}

void Soldier::AI()
{
	const float absDistanceToCharacter{ fabsf(m_pGame->GetCharacter()->GetHitBox().left - m_HitBox.left) };
	const int test{ rand() % 1000 };
	if (absDistanceToCharacter < 50.0f) // 0 - 49
	{
		if (test < 10) // 1%
		{
			m_State = State::Fleeing;
			m_Aim = Aim::Right;
			m_Velocity.x = 1.5f * m_HorizontalSpeed * int(m_Aim);
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
	}
	else if (absDistanceToCharacter < 90.0f) // 50 - 89
	{
		if (test < 10) // 1%
		{
			m_State = State::Running;
			rand() % 2 ? m_Aim = Aim::Left : m_Aim = Aim::Right; // 50% left - 50% right
			m_Velocity.x = m_HorizontalSpeed * float(int(m_Aim));
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
	}
	else if (absDistanceToCharacter < 98.0f) // 90 - 97
	{
		if (test < 10) // 1%
		{
			m_State = State::Attacking;
			m_pGame->GetCharacter()->GetHitBox().left < m_HitBox.left ? m_Aim = Aim::Left : m_Aim = Aim::Right;
			m_Velocity.x = 0;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
		else if (test < 13) // 1.3%
		{
			m_State = State::Running;
			rand() % 2 ? m_Aim = Aim::Left : m_Aim = Aim::Right; // 50% left - 50% right
			m_Velocity.x = m_HorizontalSpeed * int(m_Aim);
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
	}
	else if (absDistanceToCharacter < 103.0f) // 98 - 102
	{
		if (test < 20) // 2%
		{
			m_State = State::Attacking;
			m_pGame->GetCharacter()->GetHitBox().left < m_HitBox.left ? m_Aim = Aim::Left : m_Aim = Aim::Right;
			m_Velocity.x = 0;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
	}
	else if (absDistanceToCharacter < 110.0f) // 93 - 109
	{
		if (test < 10) // 1%
		{
			m_State = State::Attacking;
			m_pGame->GetCharacter()->GetHitBox().left < m_HitBox.left ? m_Aim = Aim::Left : m_Aim = Aim::Right;
			m_Velocity.x = 0;
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
		else if (test < 13) // 1.3%
		{
			m_State = State::Running;
			rand() % 2 ? m_Aim = Aim::Left : m_Aim = Aim::Right; // 50% left - 50% right
			m_Velocity.x = m_HorizontalSpeed * int(m_Aim);
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
	}
	else if (absDistanceToCharacter < 200) // 110 - 199
	{
		if (test < 10) // 1%
		{
			m_State = State::Running;
			rand() % 4 ? m_Aim = Aim::Left : m_Aim = Aim::Right; // 75% left - 25% right
			m_Velocity.x = m_HorizontalSpeed * int(m_Aim);
			ResetSprite();
			m_pCurrentSprite = &m_Sprites[abs(int(m_State))];
		}
	}
	// else idle
}
