#include "pch.h"
#include "WorldDynamic.h"
#include "Game.h"

WorldDynamic::WorldDynamic(Game* const game, const float left, const float bottom, const float width, const float height, const float runSpeed, const float jumpSpeed)
	: m_HitBox{left, bottom, width, height}
	, m_HorizontalSpeed{ runSpeed }
	, m_JumpSpeed{ jumpSpeed }
	, m_Gravity{ -1280.0f }
	, m_pGame{ game }
{
}

void WorldDynamic::Update(const float elapsedSec)
{
	m_Velocity.y += m_Gravity * elapsedSec;
	m_HitBox.left += m_Velocity.x * elapsedSec;
	m_HitBox.bottom += m_Velocity.y * elapsedSec;
}

Point2f WorldDynamic::GetBottomLeft() const
{
	return Point2f{m_HitBox.left, m_HitBox.bottom};
}

Rectf WorldDynamic::GetHitBox() const
{
	return m_HitBox;
}

Vector2f WorldDynamic::GetVelocity() const
{
	return m_Velocity;
}

void WorldDynamic::SetVelocityX(const float x)
{
	m_Velocity.x = x;
}

void WorldDynamic::SetVelocityY(const float y)
{
	m_Velocity.y = y;
}

void WorldDynamic::SetHitBoxBottom(const float bottom)
{
	m_HitBox.bottom = bottom;
}

void WorldDynamic::SetHitBoxLeft(const float left)
{
	m_HitBox.left = left;
}
