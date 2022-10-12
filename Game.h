#pragma once
#include "HUD.h"
#include "SettingsMenu.h"
#include "Character.h"
#include "Camera.h"
#include <set>
#include "Texture.h"
#include "Building.h"
#include "FinalBoss.h"
#include "SoundEffect.h"
#include "SoundStream.h"

class Prisoner;
class Soldier;
class Bullet;
class PickUp;
class Grenade;
class Bomb;
class Helicopter;
class SFX;
class BossBuilding;
class CanonBall;
class LaserBall;

class Game final
{
public:
	enum class State
	{
		Start, Game, Settings, End
	};

	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update(float elapsedSec);
	void Draw() const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );

	[[nodiscard]] Character* GetCharacter() const;
	[[nodiscard]] Level* GetLevel() const;
	[[nodiscard]] Camera* GetCamera() const;

	void SpawnPickUp(const Point2f& pos);
	void SpawnGrenade(const Point2f& pos, int dir);
	void SpawnBomb(const Point2f& pos);
	void SpawnCanonBall(const Point2f& pos, float velocity);
	void SpawnLaserBall(const Point2f& pos);

	void IncMusicVolume();
	void DecMusicVolume();
	void IncEffectsVolume();
	void DecEffectsVolume();
	[[nodiscard]] int GetMusicVolume() const;
	[[nodiscard]] int GetEffectsVolume() const;

	void EndGame();
	void PlayGameOver();

private:
	// DATA MEMBERS
	State m_State;
	const Window m_Window;
	HUD m_HUD;
	SettingsMenu m_Settings;
	Character* const m_pCharacter;
	Level* const m_pLevel;
	Camera* const m_pCamera;

	Texture m_TitleScreen;

	std::set<Bullet*> m_Bullets;
	const float m_BulletXOffset, m_BulletYOffset;

	static constexpr int s_NrBgBuildings{ 5 };
	Texture* m_pHousesTextures[s_NrBgBuildings];
	Building m_BgBuildings[s_NrBgBuildings];

	const float m_SectionsXLimits[5];
	int m_CurrentSection;
	const float m_Boss1StartX, m_Boss2StartX, m_FinalBossStartX;

	std::set<Prisoner*> m_Prisoners;
	std::set<Soldier*> m_Soldiers;
	std::set<PickUp*> m_PickUps;
	std::set<Grenade*> m_Grenades;
	std::set<Bomb*> m_Bombs;
	std::set<CanonBall*> m_CanonBalls;
	std::set<LaserBall*> m_LaserBalls;

	Vector2f m_GrenadeBlastSFXOffset, m_BombImpactSFXOffset, m_ExplosionSFXOffset;
	std::set<SFX*> m_SFX;

	Helicopter* m_pHelicopter;
	BossBuilding* m_pBossBuilding1, *m_pBossBuilding2, *m_pBossBuilding3;

	static constexpr int s_BossBuilding1Stages{ 4 };
	Texture m_BossBuilding1Textures[s_BossBuilding1Stages];

	static constexpr int s_BossBuilding2Stages{ 2 };
	Texture m_BossBuilding2Textures[s_BossBuilding2Stages];
	bool m_BossBuilding2HelicopterSpawned;

	static constexpr int s_BossBuilding3Stages{ 3 };
	Texture m_BossBuilding3Textures[s_BossBuilding3Stages];

	FinalBoss m_FinalBoss;

	Texture m_BulletTexture, m_GrenadeTexture, m_CanonBallTexture, m_LaserBallTexture;

	SoundEffect* m_pSndMetalSlug, *m_pSndMissionStart, *m_pSndMissionComplete, *m_pSndOk, *m_pSndGreat, *m_pSndGunshot, *m_pSndExplosion, *m_pSndGameOver;
	SoundStream* m_pSoundtrack;
	int m_SoundtrackVolume, m_SoundEffectsVolume;
	bool m_GameOverPlayed;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void PrintConsoleInfo() const;

	void UpdateGame(float elapsedSec);
	void DrawStart() const;
	void DrawGame() const;
	void DrawSettings() const;

	void SpawnBullet();
	void UpdateBullets(float elapsedSec);
	void DrawBullets() const;

	void UpdatePrisoners(float elapsedSec);
	void DrawPrisoners() const;

	void UpdateSoldiers(float elapsedSec);
	void DrawSoldiers() const;

	void UpdatePickUps(float elapsedSec);
	void DrawPickUps() const;

	void UpdateGrenades(float elapsedSec);
	void DrawGrenades() const;

	void DrawBuildings() const;

	void UpdateHelicopter(float elapsedSec);
	void DamageHelicopter(int damage);

	void DamageBossBuilding1(int damage);
	void DamageBossBuilding2(int damage);
	void DamageBossBuilding3(int damage);

	void CheckBossStart();

	void UpdateBombs(float elapsedSec);
	void DrawBombs() const;

	void UpdateSFX(float elapsedSec);
	void DrawSFX() const;

	void UpdateCanonBalls(float elapsedSec);
	void DrawCanonBalls() const;

	void UpdateLaserBalls(float elapsedSec);
	void DrawLaserBalls() const;

	void UpdateSoundVolume() const;
};