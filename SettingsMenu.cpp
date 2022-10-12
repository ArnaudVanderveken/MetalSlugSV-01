#include "pch.h"
#include "SettingsMenu.h"
#include "Game.h"

SettingsMenu::SettingsMenu(Game* game)
	: m_pGame{ game }
	, m_MusicVolume{ "Resources/Images/HUD-UI/MusicVolume.png" }
	, m_EffectsVolume{ "Resources/Images/HUD-UI/EffectsVolume.png" }
	, m_Digits{ "Resources/Images/HUD-UI/BigBlueDigits.png" }
	, m_Left{ "Resources/Images/HUD-UI/Left.png" }
	, m_Right{ "Resources/Images/HUD-UI/Right.png" }
	, m_MusicVolumePos{ 64, 112 }
	, m_EffectsVolumePos{ 48, 84 }
	, m_MusicVolumeDigitPos{ 192, 112 }
	, m_EffectsVolumeDigitPos{ 192, 84 }
	, m_UnitSize{ 16 }
{
	m_pMusicVolumeLeft = new Button{ &m_Left, Point2f{ 176, 112 } };
	m_pMusicVolumeRight = new Button{ &m_Right, Point2f{ 208, 112 } };
	m_pEffectsVolumeLeft = new Button{ &m_Left, Point2f{ 176, 84 } };
	m_pEffectsVolumeRight = new Button{ &m_Right, Point2f{ 208, 84 } };
}

SettingsMenu::~SettingsMenu()
{
	delete m_pMusicVolumeLeft;
	delete m_pMusicVolumeRight;
	delete m_pEffectsVolumeLeft;
	delete m_pEffectsVolumeRight;
}

void SettingsMenu::Draw() const
{
	m_MusicVolume.Draw(m_MusicVolumePos);
	m_EffectsVolume.Draw(m_EffectsVolumePos);
	m_pMusicVolumeLeft->Draw();
	m_pMusicVolumeRight->Draw();
	m_pEffectsVolumeLeft->Draw();
	m_pEffectsVolumeRight->Draw();
	m_Digits.Draw(m_MusicVolumeDigitPos, Rectf{ m_pGame->GetMusicVolume() * m_UnitSize, m_UnitSize, m_UnitSize, m_UnitSize });
	m_Digits.Draw(m_EffectsVolumeDigitPos, Rectf{ m_pGame->GetEffectsVolume() * m_UnitSize, m_UnitSize, m_UnitSize, m_UnitSize });
}

bool SettingsMenu::CheckMouseInput(const Point2f& pos) const
{
	if (m_pMusicVolumeLeft->IsClicked(pos))
	{
		m_pGame->DecMusicVolume();
		return true;
	}
	if (m_pMusicVolumeRight->IsClicked(pos))
	{
		m_pGame->IncMusicVolume();
		return true;
	}
	if (m_pEffectsVolumeLeft->IsClicked(pos))
	{
		m_pGame->DecEffectsVolume();
		return true;
	}
	if (m_pEffectsVolumeRight->IsClicked(pos))
	{
		m_pGame->IncEffectsVolume();
		return true;
	}
	return false;
}
