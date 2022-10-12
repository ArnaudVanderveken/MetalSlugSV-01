#include "pch.h"
#include "LaserBall.h"
#include "Texture.h"

LaserBall::LaserBall(const Point2f& startPoint, Texture* const texture)
    : m_HitBox{startPoint.x, startPoint.y, texture->GetWidth(), texture->GetHeight()}
    , m_pTexture{ texture }
    , m_Speed{ -200.0f }
{
}

void LaserBall::Update(const float elapsedSec)
{
    m_HitBox.left += m_Speed * elapsedSec;
}

void LaserBall::Draw() const
{
    m_pTexture->Draw(m_HitBox);
}

Rectf LaserBall::GetHitBox() const
{
    return m_HitBox;
}
