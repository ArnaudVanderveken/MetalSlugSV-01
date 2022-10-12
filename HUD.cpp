#include "pch.h"
#include "HUD.h"
#include "Game.h"

HUD::HUD(Game* game)
	: m_pGame{ game }
	, m_1UP{ "Resources/Images/HUD-UI/1UP.png" }
	, m_Time{ "Resources/Images/HUD-UI/Time.png" }
	, m_BlueDigits{ "Resources/Images/HUD-UI/BlueDigits.png" }
	, m_YellowDigits{ "Resources/Images/HUD-UI/YellowDigits.png" }
	, m_Prisoner{ "Resources/Images/HUD-UI/Prisoner.png" }
	, m_ArmsBomb{ "Resources/Images/HUD-UI/ArmsBomb.png" }
	, m_Credits{ "Resources/Images/HUD-UI/Credits.png" }
	, m_Level{ "Resources/Images/HUD-UI/Level-8.png" }
	, m_InsertCoin{ "Resources/Images/HUD-UI/InsertCoin.png", 1, 2, 0.5f }
	, m_1UPPos{ 12, 192 }
	, m_TimePos{ 144, 200 }
	, m_ScorePos{ 72, 208 }
	, m_UpsPos{ 46, 192 }
	, m_BombsPos{ 128, 200 }
	, m_PrisonerPos{ 16, 8 }
	, m_ArmsBombPos{ 72, 199 }
	, m_CreditsPos{ 224, 0 }
	, m_LevelPos{ 136, 0 }
	, m_InsertCoinPos{ 200, 200 }
	, m_UnitSize{ 8 }
{
}

void HUD::Update(const float elapsedSec)
{
	m_InsertCoin.Update(elapsedSec);
}

void HUD::Draw() const
{
	m_1UP.Draw(m_1UPPos);
	m_Time.Draw(m_TimePos);
	m_ArmsBomb.Draw(m_ArmsBombPos);
	m_Credits.Draw(m_CreditsPos);
	m_Level.Draw(m_LevelPos);
	m_InsertCoin.Draw(m_InsertCoinPos);

	int score{ m_pGame->GetCharacter()->GetScore() };
	int bombs{ m_pGame->GetCharacter()->GetGrenadeCount() };
	const int ups{ m_pGame->GetCharacter()->GetUp() };
	const int prisonersFreed{ m_pGame->GetCharacter()->GetPrisonersFreed() };
	int i{ 1 };
	do
	{
		m_BlueDigits.Draw(Point2f{ m_ScorePos.x - i * m_UnitSize, m_ScorePos.y }, Rectf{ (score % 10) * m_UnitSize, m_UnitSize, m_UnitSize, m_UnitSize });
		score /= 10;
		++i;
	} while (score);

	i = 1;
	do
	{
		m_YellowDigits.Draw(Point2f{ m_BombsPos.x - i * m_UnitSize, m_BombsPos.y }, Rectf{ (bombs % 10) * m_UnitSize, m_UnitSize, m_UnitSize, m_UnitSize });
		bombs /= 10;
		++i;
	} while (bombs);

	m_YellowDigits.Draw(m_UpsPos, Rectf{ ups * m_UnitSize, m_UnitSize, m_UnitSize, m_UnitSize });

	for (i = 0; i < prisonersFreed; ++i)
		m_Prisoner.Draw(Point2f{ m_PrisonerPos.x + i * m_UnitSize, m_PrisonerPos.y });
}