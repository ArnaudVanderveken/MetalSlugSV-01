#include "pch.h"
#include "PickUpPig.h"
#include "Game.h"

PickUpPig::PickUpPig(const float left, const float bottom, const float width, const float height, Game* const game)
	: PickUp(left, bottom, width, height, game)
	, m_Sprite{ "Resources/Images/PickUps/Pig/Idle.png", 10, 1, 1 / 10.0f }
{
}

void PickUpPig::OnOverlap(WorldDynamic* wd)
{
	m_pGame->GetCharacter()->AddScore(PickUpPig::s_Score);
}

void PickUpPig::Update(const float elapsedSec)
{
	m_Sprite.Update(elapsedSec);
}

void PickUpPig::Draw() const
{
	m_Sprite.Draw(Point2f{ m_HitBox.left, m_HitBox.bottom });
}
