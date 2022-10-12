#include "pch.h"
#include "Bullet.h"
#include "Level.h"

Bullet::Bullet(const Point2f& startPoint, Texture* texture, const bool horizontal, const int direction)
	: m_Position{ startPoint }
	, m_pTexture{ texture }
	, m_IsHorizontal{ horizontal }
	, m_Direction{ direction }
	, m_Speed{ 400.0f }
{
}

void Bullet::Update(const float elapsedSec)
{
	m_IsHorizontal ? m_Position.x += m_Direction * m_Speed * elapsedSec : m_Position.y += m_Direction * m_Speed * elapsedSec;
}

void Bullet::Draw() const
{
	if (m_IsHorizontal)
	{
		m_pTexture->Draw(Point2f{ m_Position.x - m_pTexture->GetWidth() / 2, m_Position.y - m_pTexture->GetHeight() / 2 });
	}
	else
	{
		glPushMatrix();
		{
			glTranslatef(m_Position.x - m_pTexture->GetWidth() / 2, m_Position.y - m_pTexture->GetHeight() / 2, 0);
			glRotatef(90, 0, 0, 1);
			m_pTexture->Draw();
		}
		glPopMatrix();
	}
}

Point2f Bullet::GetPosition() const
{
	return m_Position;
}
