#include "pch.h"
#include "Game.h"
#include "Grenade.h"
#include "Level.h"
#include "Texture.h"

Grenade::Grenade(Game* const game, const Point2f& startPoint, Texture* const texture, const int direction)
	: WorldDynamic{ game, startPoint.x, startPoint.y, texture->GetWidth(), texture->GetHeight(), 200.0f, 250.0f }
	, m_pTexture{ texture }
	, m_AngularVelocity{ 720.0f }
	, m_Angle{}
{
	m_Velocity.x = m_HorizontalSpeed * float(direction);
	m_Velocity.y = m_JumpSpeed;
}

void Grenade::Update(const float elapsedSec)
{
	m_Angle += m_AngularVelocity * elapsedSec;
	WorldDynamic::Update(elapsedSec);
}

void Grenade::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(m_HitBox.left + m_pTexture->GetWidth() / 2, m_HitBox.bottom + m_pTexture->GetHeight() / 2, 0);
		glRotatef(m_Angle, 0, 0, 1);
		glTranslatef(-m_pTexture->GetWidth() / 2, -m_pTexture->GetHeight() / 2, 0);
		m_pTexture->Draw();
	}
	glPopMatrix();
}

bool Grenade::IsOnGround()
{
	return m_pGame->GetLevel()->IsOnGround(this, true);
}
