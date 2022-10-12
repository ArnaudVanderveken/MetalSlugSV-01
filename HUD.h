#pragma once
#include "Texture.h"
#include "Sprite.h"

class Game;

class HUD final
{
public:
	HUD(Game* game);
	~HUD() = default;
	HUD(const HUD&) = delete;
	HUD& operator=(const HUD&) = delete;
	HUD(HUD&&) noexcept = delete;
	HUD& operator=(HUD&&) noexcept = delete;

	void Update(float elapsedSec);
	void Draw() const;

private:
	Game* m_pGame;
	const Texture m_1UP, m_Time, m_BlueDigits, m_YellowDigits, m_Prisoner, m_ArmsBomb, m_Credits, m_Level;
	Sprite m_InsertCoin;

	Point2f m_1UPPos, m_TimePos, m_ScorePos, m_UpsPos, m_BombsPos, m_PrisonerPos, m_ArmsBombPos, m_CreditsPos, m_LevelPos, m_InsertCoinPos;
	const float m_UnitSize;
};

