#include "pch.h"
#include "Character.h"
#include "Game.h"
#include "Level.h"

#include <iostream>

#define IDLE 0
#define RUN 1
#define IDLE_FIRE_BULLET 2
#define IDLE_FIRE_NADE 3
#define IDLE_MELEE 4
#define RUN_FIRE_BULLET 5
#define RUN_FIRE_NADE 6
#define IDLE_AIM_UP 7
#define RUN_AIM_UP 8
#define IDLE_FIRE_BULLET_UP 9
#define RUN_FIRE_BULLET_UP 10
#define JUMP 11
#define FALL 12
#define RUN_JUMP 13
#define JUMP_FIRE_BULLET 14
#define JUMP_FIRE_NADE 15
#define JUMP_MELEE 16
#define JUMP_FIRE_BULLET_UP 17
#define DYING 18
#define DEAD 19

Character::Character(Game* game, const float left, const float bottom, const float width, const float height, const float horizontalSpeed, const float jumpSpeed)
	: WorldDynamic(game, left, bottom, width, height, horizontalSpeed, jumpSpeed)
	, m_State{ State::Idle }
	, m_Aim{ Aim::Right }
	, m_Attack{ Attack::None }
	, m_IsCrouching{ false }
	, m_StandingSprites{ { "Resources/Images/Character/Idle.png", 6, 1, 1 / 6.0f },
						 { "Resources/Images/Character/Run.png", 12, 1, 1 / 20.0f },
						 { "Resources/Images/Character/IdleFireBullet.png", 10, 1, 1 / 20.0f },
						 { "Resources/Images/Character/IdleFireGrenade.png", 7, 1, 1 / 20.0f },
						 { "Resources/Images/Character/IdleMelee.png", 8, 1, 1 / 20.0f },
						 { "Resources/Images/Character/RunFireBullet.png", 10, 1, 1 / 20.0f },
						 { "Resources/Images/Character/RunFireGrenade.png", 12, 1, 1 / 20.0f },
						 { "Resources/Images/Character/IdleAimUp.png", 6, 1, 1 / 6.0f },
						 { "Resources/Images/Character/RunAimUp.png", 11, 1, 1 / 20.0f },
						 { "Resources/Images/Character/IdleFireBulletUp.png", 10, 1, 1 / 20.0f },
						 { "Resources/Images/Character/RunFireBulletUp.png", 17, 1, 1 / 20.0f },
						 { "Resources/Images/Character/Jump.png", 13, 1, 1 / 20.0f },
						 { "Resources/Images/Character/Falling.png", 1, 1, 1 },
						 { "Resources/Images/Character/RunJump.png", 13, 1, 1 / 20.0f },
						 { "Resources/Images/Character/JumpFireBullet.png", 14, 1, 1 / 20.0f },
						 { "Resources/Images/Character/JumpFireGrenade.png", 10, 1, 1 / 20.0f },
						 { "Resources/Images/Character/JumpMelee.png", 8, 1, 1 / 20.0f },
						 { "Resources/Images/Character/JumpFireBulletUp.png", 12, 1, 1 / 20.0f },
						 { "Resources/Images/Character/Dying.png", 16, 1, 1 / 20.0f },
						 { "Resources/Images/Character/Dead.png", 1, 1, 1 }
					   }
	, m_CrouchingSprites{ { "Resources/Images/Character/CrouchIdle.png", 7, 1, 1 / 7.0f },
						  { "Resources/Images/Character/CrouchRun.png", 7, 1, 1 / 12.0f },
						  { "Resources/Images/Character/CrouchIdleFireBullet.png", 10, 1, 1 / 20.0f },
						  { "Resources/Images/Character/CrouchIdleFireGrenade.png", 12, 1, 1 / 20.0f },
						  { "Resources/Images/Character/CrouchIdleMelee.png", 7, 1, 1 / 20.0f }
						}
	, m_Score{}
	, m_Up{ 2 }
	, m_PrisonersFreed{}
	, m_Timer{}
	, m_GracePeriod{ 3.0f }
	, m_DeathTime{ 1.0f }
	, m_GrenadeCount{ 10 }
	, m_GrenadeSpawnOffset{ width / 2, height }
{
	m_pCurrentSprite = &m_StandingSprites[IDLE];
	m_SavedAim = m_Aim;
}

void Character::Update(const float elapsedSec)
{
	m_Timer += elapsedSec;
	switch (m_State)
	{
	case State::Dying:
		if (m_pCurrentSprite->GetCycle())
		{
			m_State = State::Dead;
			ResetSprite();
			m_pCurrentSprite = &m_StandingSprites[DEAD];
			m_Timer = 0.0f;
		}
		break;

	case State::Dead:
		if (m_Timer >= m_DeathTime)
		{
			if (m_Up)
			{
				--m_Up;
				Revive();
			}
			else
				m_pGame->PlayGameOver();
		}
		break;

	default:
		break;
	}

	HandleInputs();
	WorldDynamic::Update(elapsedSec);
	HandleCollisions();

	UpdateSprite(elapsedSec);
}

void Character::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_HitBox.left, m_HitBox.bottom, 0);
		if (m_Aim == Aim::Up ? m_SavedAim == Aim::Left : m_Aim == Aim::Left)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_HitBox.width, 0, 0);
		}
		m_pCurrentSprite->Draw(Point2f{ -5, 0 });
	}
	glPopMatrix();
}

Character::Aim Character::GetAim() const
{
	return m_Aim;
}

bool Character::IsCrouching() const
{
	return m_IsCrouching;
}

void Character::AddScore(const int score)
{
	m_Score += score;
}

void Character::AddGrenades(const int grenades)
{
	m_GrenadeCount += grenades;
}

void Character::IncPrisonerFreed()
{
	++m_PrisonersFreed;
}

int Character::GetGrenadeCount() const
{
	return m_GrenadeCount;
}

int Character::GetUp() const
{
	return m_Up;
}

int Character::GetScore() const
{
	return m_Score;
}

int Character::GetPrisonersFreed() const
{
	return m_PrisonersFreed;
}

void Character::HandleInputs()
{
	const Uint8* keyboard{ SDL_GetKeyboardState(nullptr) };
	switch (m_State)
	{
	case State::Idle: // Character is on ground
	{
		// Crouching
		if (keyboard[SDL_SCANCODE_S] && !m_IsCrouching)
		{
			m_IsCrouching = true;
			m_pCurrentSprite = &m_CrouchingSprites[IDLE];
		}
		else if (!keyboard[SDL_SCANCODE_S] && m_IsCrouching)
		{
			m_IsCrouching = false;
			m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? IDLE_AIM_UP : IDLE];
		}
		// Ignore move when crouch attacking
		if (!(m_IsCrouching && m_Attack != Attack::None))
		{
			// Horizontal Move
			if (keyboard[SDL_SCANCODE_D])
			{
				m_IsCrouching ? m_Velocity.x = m_HorizontalSpeed / 2 : m_Velocity.x = m_HorizontalSpeed;
				m_State = State::Moving;
				m_Aim == Aim::Up ? m_SavedAim = Aim::Right : m_Aim = Aim::Right;
				ResetSprite();
				m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[RUN] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? RUN_AIM_UP : RUN];
			}
			else if (keyboard[SDL_SCANCODE_A])
			{
				m_IsCrouching ? m_Velocity.x = -m_HorizontalSpeed / 2 : m_Velocity.x = -m_HorizontalSpeed;
				m_State = State::Moving;
				m_Aim == Aim::Up ? m_SavedAim = Aim::Left : m_Aim = Aim::Left;
				ResetSprite();
				m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[RUN] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? RUN_AIM_UP : RUN];
			}
			// Jump
			if (keyboard[SDL_SCANCODE_SPACE])
			{
				m_Velocity.y = m_JumpSpeed;
				m_State = State::Jumping;
				ResetSprite();
				m_pCurrentSprite = &m_StandingSprites[JUMP];
			}
		}
		
		// Aim up
		if (keyboard[SDL_SCANCODE_W] && m_Aim != Aim::Up && !m_IsCrouching)
		{
			m_SavedAim = m_Aim;
			m_Aim = Aim::Up;
			ResetSprite();
			m_pCurrentSprite = &m_StandingSprites[IDLE_AIM_UP];
		}
		else if (!keyboard[SDL_SCANCODE_W] && m_Aim == Aim::Up)
		{
			m_Aim = m_SavedAim;
			m_pCurrentSprite = &m_StandingSprites[IDLE];
		}
		// Finished anim fire bullet
		if (m_Attack != Attack::None && m_pCurrentSprite->GetCycle())
		{
			m_Attack = Attack::None;
			ResetSprite();
			m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[IDLE] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? IDLE_AIM_UP : IDLE];
		}
		break;
	}

	case State::Moving: // Character is on ground
	{
		// Crouching
		if (keyboard[SDL_SCANCODE_S] && !m_IsCrouching)
		{
			m_IsCrouching = true;
			m_pCurrentSprite = &m_CrouchingSprites[RUN];
		}
		else if (!keyboard[SDL_SCANCODE_S] && m_IsCrouching)
		{
			m_IsCrouching = false;
			m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? RUN_AIM_UP : RUN];
		}

		// Horizontal Move
		if (keyboard[SDL_SCANCODE_D])
		{
			m_Aim == Aim::Up ? m_SavedAim = Aim::Right : m_Aim = Aim::Right;
			m_IsCrouching ? m_Velocity.x = m_HorizontalSpeed / 2 : m_Velocity.x = m_HorizontalSpeed;
		}
		else if (keyboard[SDL_SCANCODE_A])
		{
			m_Aim == Aim::Up ? m_SavedAim = Aim::Left : m_Aim = Aim::Left;
			m_IsCrouching ? m_Velocity.x = -m_HorizontalSpeed / 2 : m_Velocity.x = -m_HorizontalSpeed;
		}
		else if (!(keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_D]))
		{
			m_Velocity.x = 0;
		}

		// Jump
		if (keyboard[SDL_SCANCODE_SPACE])
		{
			m_Velocity.y = m_JumpSpeed;
			m_State = State::Jumping;
			ResetSprite();
			m_pCurrentSprite = &m_StandingSprites[RUN_JUMP];
		}
		//Idle
		else if (!(keyboard[SDL_SCANCODE_A] || keyboard[SDL_SCANCODE_D] || keyboard[SDL_SCANCODE_SPACE]))
		{
			m_State = State::Idle;
			ResetSprite();
			m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[IDLE] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? IDLE_AIM_UP : IDLE];
		}
		// Aim up
		if (keyboard[SDL_SCANCODE_W] && m_Aim != Aim::Up && !m_IsCrouching)
		{
			m_SavedAim = m_Aim;
			m_Aim = Aim::Up;
			ResetSprite();
			m_pCurrentSprite = &m_StandingSprites[RUN_AIM_UP];
		}
		else if (!keyboard[SDL_SCANCODE_W] && m_Aim == Aim::Up)
		{
			m_Aim = m_SavedAim;
			m_pCurrentSprite = &m_StandingSprites[RUN];
		}
		// Finished anim fire bullet
		if (m_Attack != Attack::None && m_pCurrentSprite->GetCycle())
		{
			m_Attack = Attack::None;
			ResetSprite();
			m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[RUN] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? RUN_AIM_UP : RUN];
		}
		break;
	}

	case State::Jumping: // Character is NOT on ground
	{
		// Air control
		if (keyboard[SDL_SCANCODE_D])
		{
			m_Aim == Aim::Up ? m_SavedAim = Aim::Right : m_Aim = Aim::Right;
			m_Velocity.x = m_HorizontalSpeed;
		}
		else if (keyboard[SDL_SCANCODE_A])
		{
			m_Aim == Aim::Up ? m_SavedAim = Aim::Left : m_Aim = Aim::Left;
			m_Velocity.x = -m_HorizontalSpeed;
		}
		// Finished jump anim
		if (m_pCurrentSprite->GetCycle())
		{
			ResetSprite();
			m_pCurrentSprite = &m_StandingSprites[FALL];
		}
		// Landed
		if (m_pGame->GetLevel()->IsOnGround(this))
		{
			m_State = State::Moving;
			ResetSprite();
			m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[RUN] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? RUN_AIM_UP : RUN];
		}
		// Aim up
		if (keyboard[SDL_SCANCODE_W] && m_Aim != Aim::Up && !m_IsCrouching)
		{
			m_SavedAim = m_Aim;
			m_Aim = Aim::Up;
		}
		else if (!keyboard[SDL_SCANCODE_W] && m_Aim == Aim::Up)
		{
			m_Aim = m_SavedAim;
		}
		// Finished anim fire bullet
		if (m_Attack != Attack::None && m_pCurrentSprite->GetCycle())
		{
			m_Attack = Attack::None;
			ResetSprite();
			m_pCurrentSprite = &m_StandingSprites[JUMP];
		}
		break;
	}

	default:
		break;
	}
}

void Character::HandleCollisions()
{
	if (m_HitBox.left < m_pGame->GetLevel()->GetBoundaries().left) m_HitBox.left = m_pGame->GetLevel()->GetBoundaries().left;
	else if (m_HitBox.left + m_HitBox.width > m_pGame->GetLevel()->GetBoundaries().left + m_pGame->GetLevel()->GetBoundaries().width) m_HitBox.left = m_pGame->GetLevel()->GetBoundaries().left + m_pGame->GetLevel()->GetBoundaries().width - m_HitBox.width;

	if (m_HitBox.bottom < m_pGame->GetLevel()->GetBoundaries().bottom) m_HitBox.bottom = m_pGame->GetLevel()->GetBoundaries().bottom;
	else if (m_HitBox.bottom + m_HitBox.height > m_pGame->GetLevel()->GetBoundaries().bottom + m_pGame->GetLevel()->GetBoundaries().height) m_HitBox.bottom = m_pGame->GetLevel()->GetBoundaries().bottom + m_pGame->GetLevel()->GetBoundaries().height - m_HitBox.height;

	m_pGame->GetLevel()->HandleCollision(this);
}

void Character::UpdateSprite(const float elapsedSec) const
{
	m_pCurrentSprite->Update(elapsedSec);
}

void Character::ResetSprite() const
{
	m_pCurrentSprite->Reset();
}

void Character::Revive()
{
	m_Timer = 0.0f;
	m_State = State::Jumping;
	ResetSprite();
	m_pCurrentSprite = &m_StandingSprites[FALL];
	m_HitBox.bottom += 100.0f;
}

void Character::FireBullet()
{
	m_Attack = Attack::Bullet;
	ResetSprite();

	switch (m_State)
	{
	case State::Idle:
		m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[IDLE_FIRE_BULLET] : m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? IDLE_FIRE_BULLET_UP : IDLE_FIRE_BULLET];
		break;

	case State::Moving:
		if (m_IsCrouching)
		{
			m_State = State::Idle;
			m_Velocity.x = 0;
			m_pCurrentSprite = &m_CrouchingSprites[IDLE_FIRE_BULLET];
		}
		else
		{
			m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? RUN_FIRE_BULLET_UP : RUN_FIRE_BULLET];
		}
		break;

	case State::Jumping:
		if (m_IsCrouching)
		{
			m_IsCrouching = false;
			m_Velocity.x = m_HorizontalSpeed;
		}
		m_pCurrentSprite = &m_StandingSprites[m_Aim == Aim::Up ? JUMP_FIRE_BULLET_UP : JUMP_FIRE_BULLET];
		break;

	default:
		break;
	}
}

void Character::FireGrenade()
{
	m_Attack = Attack::Grenade;
	--m_GrenadeCount;
	if (m_Aim == Aim::Up) m_Aim = m_SavedAim;
	switch (m_State)
	{
	case State::Idle:
		ResetSprite();
		m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[IDLE_FIRE_NADE] : m_pCurrentSprite = &m_StandingSprites[IDLE_FIRE_NADE];
		break;

	case State::Moving:
		ResetSprite();
		if (m_IsCrouching)
		{
			m_State = State::Idle;
			m_Velocity.x = 0;
			m_pCurrentSprite = &m_CrouchingSprites[IDLE_FIRE_NADE];
		}
		else
		{
			m_pCurrentSprite = &m_StandingSprites[RUN_FIRE_NADE];
		}
		break;

	case State::Jumping:
		if (m_IsCrouching)
		{
			m_IsCrouching = false;
			m_Velocity.x = m_HorizontalSpeed;
		}
		m_pCurrentSprite = &m_StandingSprites[JUMP_FIRE_NADE];
		break;

	default:
		break;
	}

	m_pGame->SpawnGrenade(Point2f{ m_HitBox.left, m_HitBox.bottom } + m_GrenadeSpawnOffset, m_Aim == Aim::Left ? -1 : 1);
}

void Character::Melee()
{
	m_Attack = Attack::Melee;
	if (m_Aim == Aim::Up) m_Aim = m_SavedAim;
	switch (m_State)
	{
	case State::Idle:
		ResetSprite();
		m_IsCrouching ? m_pCurrentSprite = &m_CrouchingSprites[IDLE_MELEE] : m_pCurrentSprite = &m_StandingSprites[IDLE_MELEE];
		break;

	case State::Moving:
		ResetSprite();
		if (m_IsCrouching)
		{
			m_State = State::Idle;
			m_Velocity.x = 0;
			m_pCurrentSprite = &m_CrouchingSprites[IDLE_MELEE];
		}
		else
		{
			m_pCurrentSprite = &m_StandingSprites[IDLE_MELEE];
		}
		break;

	case State::Jumping:
		if (m_IsCrouching)
		{
			m_IsCrouching = false;
			m_Velocity.x = m_HorizontalSpeed;
		}
		m_pCurrentSprite = &m_StandingSprites[JUMP_MELEE];
		break;

	default:
		break;
	}
}

void Character::Kill()
{
	m_Velocity.x = 0;
	ResetSprite();
	m_State = State::Dying;
	m_pCurrentSprite = &m_StandingSprites[DYING];
}

bool Character::IsVulnerable() const
{
	return !((m_State == State::Dying || m_State == State::Dead) || (m_State != State::Dying && m_State != State::Dead) && m_Timer < m_GracePeriod );
}

bool Character::IsAlive() const
{
	return (m_State != State::Dying && m_State != State::Dead);
}
