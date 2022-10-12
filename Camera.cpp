#include "pch.h"
#include "Camera.h"
#include "Game.h"
#include "Level.h"

Camera::Camera(const float width, const float height, Game* const game)
	: m_Width{ width }
	, m_Height{ height }
	, m_Margin{ 50.0f }
	, m_BottomLeft{ 0, 0 }
	, m_pGame{ game }
	, m_StartClimb{ 3174.0f, 0 }
	, m_EndClimb{ 3608.0f, 112.0f }
	, m_Velocity{}
	, m_MaxSpeed{ 150.0f }
{
}


void Camera::Transform() const
{
	glTranslatef(-m_BottomLeft.x, -m_BottomLeft.y, 0);
}

void Camera::Update(const Rectf& target, const float elapsedSec)
{
	Track(target, elapsedSec);
	Clamp();
}

Point2f Camera::GetPosition() const
{
	return m_BottomLeft;
}

float Camera::GetWidth() const
{
	return m_Width;
}

float Camera::GetHeight() const
{
	return m_Height;
}

void Camera::Track(const Rectf& target, const float elapsedSec)
{
	m_Velocity = m_MaxSpeed * sinf(PI / 4 * (target.left - m_BottomLeft.x - m_Margin) / m_Margin);
	m_BottomLeft.x += m_Velocity * elapsedSec;

	if (m_BottomLeft.x < m_StartClimb.x)
		m_BottomLeft.y = m_StartClimb.y;

	else if (m_BottomLeft.x < m_EndClimb.x)
		m_BottomLeft.y = (m_EndClimb.y - m_StartClimb.y) / (m_EndClimb.x - m_StartClimb.x) * (m_BottomLeft.x - m_StartClimb.x);

	else
		m_BottomLeft.y = m_EndClimb.y;
}

void Camera::Clamp()
{
	if (m_BottomLeft.x + m_Width > m_pGame->GetLevel()->GetBoundaries().left + m_pGame->GetLevel()->GetBoundaries().width)
		m_BottomLeft.x = m_pGame->GetLevel()->GetBoundaries().left + m_pGame->GetLevel()->GetBoundaries().width - m_Width;

	else if (m_BottomLeft.x < m_pGame->GetLevel()->GetBoundaries().left)
		m_BottomLeft.x = m_pGame->GetLevel()->GetBoundaries().left;

	if (m_BottomLeft.y + m_Height > m_pGame->GetLevel()->GetBoundaries().bottom + m_pGame->GetLevel()->GetBoundaries().height)
		m_BottomLeft.y = m_pGame->GetLevel()->GetBoundaries().bottom + m_pGame->GetLevel()->GetBoundaries().height - m_Height;
}
