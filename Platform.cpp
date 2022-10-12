#include "pch.h"
#include "utils.h"
#include "Platform.h"
#include "Level.h"
#include "WorldDynamic.h"


Platform::Platform(const float left, const float bottom, const float width, const float height, Game* const game)
	: WorldStatic(left, bottom, width, height, game)
{
}

void Platform::OnOverlap(WorldDynamic* wd)
{
	if (wd->GetVelocity().y < 0
		&& (utils::IsPointInRect(Point2f{ wd->GetHitBox().left, wd->GetHitBox().bottom }, m_HitBox)
			|| utils::IsPointInRect(Point2f{ wd->GetHitBox().left + wd->GetHitBox().width, wd->GetHitBox().bottom }, m_HitBox)))
	{
		wd->SetHitBoxBottom(m_HitBox.bottom + m_HitBox.height);
		wd->SetVelocityY(0);
	}
}