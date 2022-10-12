#pragma once
#include "Button.h"
#include "Texture.h"

class Game;

class SettingsMenu final
{
public:
	SettingsMenu(Game* game);
	~SettingsMenu();
	SettingsMenu(const SettingsMenu&) = delete;
	SettingsMenu& operator=(const SettingsMenu&) = delete;
	SettingsMenu(SettingsMenu&&) noexcept = delete;
	SettingsMenu& operator=(SettingsMenu&&) noexcept = delete;

	void Draw() const;
	[[nodiscard]] bool CheckMouseInput(const Point2f& pos) const;

private:
	Game* const m_pGame;
	Texture m_MusicVolume, m_EffectsVolume, m_Digits, m_Left, m_Right;
	Button* m_pMusicVolumeLeft{}, * m_pMusicVolumeRight{}, * m_pEffectsVolumeLeft{}, * m_pEffectsVolumeRight{};
	Point2f m_MusicVolumePos, m_EffectsVolumePos, m_MusicVolumeDigitPos, m_EffectsVolumeDigitPos;

	const float m_UnitSize;
};

