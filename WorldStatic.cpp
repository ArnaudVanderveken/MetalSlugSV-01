#include "pch.h"
#include "WorldStatic.h"
#include "Game.h"


WorldStatic::WorldStatic(const float left, const float bottom, const float width, const float height, Game* const game)
	: m_HitBox{ left, bottom, width, height }
	, m_pGame{ game }
{
}

Rectf WorldStatic::GetHitBox() const
{
	return m_HitBox;
}
