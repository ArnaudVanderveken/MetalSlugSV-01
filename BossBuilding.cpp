#include "pch.h"
#include "BossBuilding.h"
#include "Texture.h"
#include "WorldDynamic.h"
#include "utils.h"

BossBuilding::BossBuilding(const Point2f& pos, Texture* const textures, const int lastStage, const int hitsPerStage, Game* const game, const float textureXOffset)
	: Building (pos, textures, lastStage, hitsPerStage, game)
	, m_TextureXOffset{ textureXOffset }
{
}

void BossBuilding::OnOverlap(WorldDynamic* wd)
{
	if (wd->GetVelocity().x > 0
		&& utils::IsOverlapping(wd->GetHitBox(), m_HitBox))
	{
		wd->SetHitBoxLeft(m_HitBox.left - wd->GetHitBox().width);
		wd->SetVelocityX(0);
	}
}

void BossBuilding::Draw() const
{
	m_pTextures[m_CurrentStage].Draw(Point2f{ m_HitBox.left + m_TextureXOffset, m_HitBox.bottom });
}

bool BossBuilding::IsBroken() const
{
	return (m_CurrentStage == m_LastStage && m_AccuHits >= m_HitsPerStage);
}
