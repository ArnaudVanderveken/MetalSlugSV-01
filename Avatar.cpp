#include "pch.h"
#include "Avatar.h"
#include "utils.h"
#include "Level.h"
#include "Texture.h"


Avatar::Avatar()
	: m_HorizontalSpeed{ 100.0f }
	, m_JumpSpeed{ 320.0f }
	, m_Velocity{ Vector2f{ 0.0f, 0.0f } }
	, m_Acceleration{ Vector2f{ 0.0f, -720.0f } }
	, m_ActionState{ ActionState::moving }
	, m_pLevel{ nullptr }
	, m_pSpritesTexture{ new Texture{"Resources/Images/TestPlayer.png"} }
	, m_ClipHeight{ 40 }
	, m_ClipWidth{ 40 }
	, m_NrOfFrames{ 6 }
	, m_NrFramesPerSec{ 3 }
	, m_AnimTime{}
	, m_AnimFrame{}
{
	m_HitBox = Rectf{ 50.0f, 50.0f, m_ClipWidth - 15, m_ClipHeight };
}

Avatar::~Avatar()
{
	delete m_pSpritesTexture;
}

void Avatar::Update(float elapsedSec)
{
	UpdateState(elapsedSec);
	UpdateAnim(elapsedSec);
}

void Avatar::UpdateState(float elapsedSec)
{
	const Uint8* state{ SDL_GetKeyboardState(NULL) };
	switch (m_ActionState)
	{
	case ActionState::waiting:
		if (state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D] || state[SDL_SCANCODE_U])
		{
			m_ActionState = ActionState::moving;
			ResetAnim();
		}
		break;

	case ActionState::moving:
		// Horizontal Move
		if (state[SDL_SCANCODE_D])
		{
			m_Velocity.x = m_HorizontalSpeed;
		}
		else if (state[SDL_SCANCODE_A])
		{
			m_Velocity.x = -m_HorizontalSpeed;
		}
		else if (!(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D]))
		{
			m_Velocity.x = 0;
		}

		// Only when on ground
		if (m_pLevel->IsOnGround())
		{
			// Jump
			if (state[SDL_SCANCODE_U])
			{
				m_Velocity.y = m_JumpSpeed;
			}
			//Idle
			else if (!(state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D] || state[SDL_SCANCODE_U]))
			{
				m_ActionState = ActionState::waiting;
				ResetAnim();
			}
		}

		m_Velocity.y += m_Acceleration.y * elapsedSec;
		m_HitBox.bottom += m_Velocity.y * elapsedSec;
		m_HitBox.left += m_Velocity.x * elapsedSec;

		if (m_HitBox.left < m_pLevel->GetBoundaries().left) m_HitBox.left = m_pLevel->GetBoundaries().left;
		else if (m_HitBox.left + m_HitBox.width > m_pLevel->GetBoundaries().left + m_pLevel->GetBoundaries().width) m_HitBox.left = m_pLevel->GetBoundaries().left + m_pLevel->GetBoundaries().width - m_HitBox.width;

		if (m_HitBox.bottom < m_pLevel->GetBoundaries().bottom) m_HitBox.bottom = m_pLevel->GetBoundaries().bottom;
		else if (m_HitBox.bottom + m_HitBox.height > m_pLevel->GetBoundaries().bottom + m_pLevel->GetBoundaries().height) m_HitBox.bottom = m_pLevel->GetBoundaries().bottom + m_pLevel->GetBoundaries().height - m_HitBox.height;

		m_pLevel->HandleCollision();
		break;

	case ActionState::transforming:
		/*m_AccuTransformSec += elapsedSec;
		if (m_AccuTransformSec >= m_MaxTransformSec)
		{
			m_AccuTransformSec = 0.0f;
			m_ActionState = ActionState::moving;
			ResetSprite();
			++m_Power;
			m_Velocity = Vector2f{};
		}*/
		break;
	}
}

void Avatar::UpdateAnim(float elapsedSec)
{
	m_AnimTime += elapsedSec;
	if (m_AnimTime >= 1.0f / m_NrFramesPerSec)
	{
		m_AnimTime -= 1.0f / m_NrFramesPerSec;
		m_AnimFrame = ++m_AnimFrame % m_NrOfFrames;
	}
}

void Avatar::ResetAnim()
{
	m_AnimTime = 0;
	m_AnimFrame = 0;
}

void Avatar::Draw() const
{
	glPushMatrix();
		glTranslatef(m_HitBox.left, m_HitBox.bottom, 0);
		if (m_Velocity.x < 0)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-m_HitBox.width, 0, 0);
		}
		m_pSpritesTexture->Draw(Point2f{ -5, 0 }, Rectf{ m_AnimFrame * m_ClipWidth, m_ClipHeight, m_ClipWidth, m_ClipHeight });
	glPopMatrix();
}

Rectf Avatar::GetHitBox() const
{
	return m_HitBox;
}

Vector2f Avatar::GetVelocity() const
{
	return m_Velocity;
}

void Avatar::SetLevel(const Level* level)
{
	m_pLevel = level;
}

void Avatar::SetVelocityX(float x)
{
	m_Velocity.x = x;
}

void Avatar::SetVelocityY(float y)
{
	m_Velocity.y = y;
}

void Avatar::SetHitBoxBottom(float bottom)
{
	m_HitBox.bottom = bottom;
}

void Avatar::SetHitBoxLeft(float left)
{
	m_HitBox.left = left;
}
