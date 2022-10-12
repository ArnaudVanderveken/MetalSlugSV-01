#include "pch.h"
#include "Building.h"
#include "Texture.h"

Building::Building(const Point2f& pos, Texture* const textures, const int lastStage, const int hitsPerStage, Game* const game)
	: WorldStatic{ pos.x, pos.y, textures->GetWidth(), textures->GetHeight(), game }
	, m_LastStage{ lastStage }
	, m_HitsPerStage{ hitsPerStage }
	, m_CurrentStage{ 0 }
	, m_AccuHits{ 0 }
	, m_pTextures{ textures }
{
}

void Building::OnOverlap(WorldDynamic* wd)
{
}

void Building::Draw() const
{
	m_pTextures[m_CurrentStage].Draw(m_HitBox);
}

void Building::Hit(const int damages)
{
	m_AccuHits += damages;
	if (m_AccuHits > m_HitsPerStage && m_CurrentStage != m_LastStage)
	{
		m_AccuHits -= m_HitsPerStage;
		++m_CurrentStage;
	}
}
