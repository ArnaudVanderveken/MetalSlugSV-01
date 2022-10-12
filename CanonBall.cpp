#include "pch.h"

#include "CanonBall.h"
#include "Game.h"
#include "Level.h"
#include "Texture.h"

CanonBall::CanonBall(Game* const game, const Point2f& startPoint, Texture* const texture, const float velocityX)
	: WorldDynamic{ game, startPoint.x, startPoint.y, texture->GetWidth(), texture->GetHeight(), velocityX, 495 }
	, m_pTexture{ texture }
{
	m_Velocity.x = m_HorizontalSpeed;
	m_Velocity.y = m_JumpSpeed;
}

void CanonBall::Draw() const
{
	m_pTexture->Draw(m_HitBox);
}

bool CanonBall::IsOnGround()
{
	return m_pGame->GetLevel()->IsOnGround(this, true);
}
