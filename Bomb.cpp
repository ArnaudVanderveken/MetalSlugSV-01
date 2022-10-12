#include "pch.h"

#include "Bomb.h"
#include "Game.h"
#include "Level.h"
#include "Sprite.h"

Bomb::Bomb(Game* const game, const float left, const float bottom, const float width, const float height)
	: WorldDynamic{ game, left, bottom, width, height, 0, 0 }
	, m_Sprite{ "Resources/Images/Projectiles/Bomb.png", 7, 1, 1 / 20.0f }
{
}

void Bomb::Update(const float elapsedSec)
{
	m_Sprite.Update(elapsedSec);
	WorldDynamic::Update(elapsedSec);
}

void Bomb::Draw() const
{
	m_Sprite.Draw(Point2f{ m_HitBox.left, m_HitBox.bottom });
}

bool Bomb::IsOnGround()
{
	return m_pGame->GetLevel()->IsOnGround(this, true);
}