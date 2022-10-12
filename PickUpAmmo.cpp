#include "pch.h"
#include "PickUpAmmo.h"
#include "Game.h"

PickUpAmmo::PickUpAmmo(const float left, const float bottom, const float width, const float height, Game* const game)
	: PickUp(left, bottom, width, height, game)
	, m_Sprite{ "Resources/Images/PickUps/Ammo/Idle.png", 7, 1, 1 / 10.0f }
{
}

void PickUpAmmo::OnOverlap(WorldDynamic* wd)
{
	m_pGame->GetCharacter()->AddGrenades(m_Grenades);
}

void PickUpAmmo::Update(const float elapsedSec)
{
	m_Sprite.Update(elapsedSec);
}

void PickUpAmmo::Draw() const
{
	m_Sprite.Draw(Point2f{ m_HitBox.left, m_HitBox.bottom });
}
