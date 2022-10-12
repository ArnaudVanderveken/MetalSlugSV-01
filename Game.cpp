#include "pch.h"
#include <iostream>

#include "Bullet.h"
#include "Bomb.h"
#include "BombImpactSFX.h"
#include "BossBuilding.h"
#include "CanonBall.h"
#include "ExplosionSFX.h"
#include "Game.h"
#include "Grenade.h"
#include "GrenadeBlastSFX.h"
#include "Helicopter.h"
#include "LaserBall.h"
#include "Level.h"
#include "PickUpPig.h"
#include "PickUpAmmo.h"
#include "Prisoner.h"
#include "Soldier.h"
#include "utils.h"


Game::Game( const Window& window ) 
	: m_State{ State::Start }
	, m_Window{ window }
	, m_HUD{ this }
	, m_Settings{ this }
	, m_pCharacter{ new Character{ this, 50.0f, 50.0f, 25, 40, 80, 420 } }
	, m_pLevel{ new Level{ this } }
	, m_pCamera{ new Camera{ window.width / GAME_SCALE, window.height / GAME_SCALE, this } }
	, m_TitleScreen{ "Resources/Images/HUD-UI/TitleScreen.png" }
	, m_BulletXOffset{ 15.0f }
	, m_BulletYOffset{ 25.0f }
	, m_pHousesTextures{ new Texture{ "Resources/Images/Buildings/House1.png" },
		new Texture{ "Resources/Images/Buildings/House2.png" },
		new Texture{ "Resources/Images/Buildings/House3.png" },
		new Texture{ "Resources/Images/Buildings/House4.png" },
		new Texture{ "Resources/Images/Buildings/House5.png" },
	}
	, m_BgBuildings{ { Point2f{ 2039, 48 }, m_pHousesTextures[0], 0, 0, this },
		{ Point2f{ 2130, 48 }, m_pHousesTextures[1], 0, 0, this },
		{ Point2f{ 2231, 48 }, m_pHousesTextures[2], 0, 0, this },
		{ Point2f{ 2344, 48 }, m_pHousesTextures[3], 0, 0, this },
		{ Point2f{ 2521, 48 }, m_pHousesTextures[4], 0, 0, this },
	}
	, m_SectionsXLimits{ 1840, 2916, 2952, 3172, 4100 }
	, m_CurrentSection{}
	, m_Boss1StartX{ 1520 }
	, m_Boss2StartX{ 2600 }
	, m_FinalBossStartX{ 3828 }
	, m_GrenadeBlastSFXOffset{ -22.0f, -5.0f }
	, m_BombImpactSFXOffset{ -2.5f, -5.0f }
	, m_ExplosionSFXOffset{ 5.0f, 0.0f }
	, m_pHelicopter{ nullptr }
	, m_BossBuilding1Textures{ Texture{ "Resources/Images/Buildings/BossHouse1/Stage1.png" },
		Texture{ "Resources/Images/Buildings/BossHouse1/Stage2.png" },
		Texture{ "Resources/Images/Buildings/BossHouse1/Stage3.png" },
		Texture{ "Resources/Images/Buildings/BossHouse1/Stage4.png" }
	}
	, m_BossBuilding2Textures{ Texture{ "Resources/Images/Buildings/BossHouse2/Stage1.png" },
		Texture{ "Resources/Images/Buildings/BossHouse2/Stage2.png" }
	}
	, m_BossBuilding2HelicopterSpawned{ false }
	, m_BossBuilding3Textures{ Texture{ "Resources/Images/Buildings/BossHouse3/Stage1.png" },
		Texture{ "Resources/Images/Buildings/BossHouse3/Stage2.png" },
		Texture{ "Resources/Images/Buildings/BossHouse3/Stage3.png" }
	}
	, m_FinalBoss{ this }
	, m_BulletTexture{ "Resources/Images/Projectiles/Bullet.png" }
	, m_GrenadeTexture{ "Resources/Images/Projectiles/Grenade.png" }
	, m_CanonBallTexture{ "Resources/Images/Projectiles/BossCanonBall.png" }
	, m_LaserBallTexture{ "Resources/Images/Projectiles/BossLaserBall.png" }
	, m_pSndMetalSlug{ new SoundEffect{ "Resources/Sound/MetalSlug.mp3" } }
	, m_pSndMissionStart{ new SoundEffect{ "Resources/Sound/Mission1Start.mp3" } }
	, m_pSndMissionComplete{ new SoundEffect{ "Resources/Sound/MissionComplete.mp3" } }
	, m_pSndOk{ new SoundEffect{ "Resources/Sound/Ok.mp3" } }
	, m_pSndGreat{ new SoundEffect{ "Resources/Sound/Great.mp3" } }
	, m_pSndGunshot{ new SoundEffect{ "Resources/Sound/Gunshot.mp3" } }
	, m_pSndExplosion{ new SoundEffect{ "Resources/Sound/Explosion.mp3" } }
	, m_pSndGameOver{ new SoundEffect{ "Resources/Sound/GameOver.mp3"} }
	, m_pSoundtrack{ new SoundStream{ "Resources/Sound/Soundtrack.mp3" } }
	, m_SoundtrackVolume{ 5 }
	, m_SoundEffectsVolume{ 5 }
	, m_GameOverPlayed{ false }
{
	UpdateSoundVolume();
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_Prisoners.insert(new Prisoner{ this, 200, 50 });
	m_Prisoners.insert(new Prisoner{ this, 775, 27 });
	m_Prisoners.insert(new Prisoner{ this, 1050, 105 });
	m_Prisoners.insert(new Prisoner{ this, 1125, 65 });
	m_Prisoners.insert(new Prisoner{ this, 1555, 65 });
	m_Prisoners.insert(new Prisoner{ this, 3840, 256 });
	m_Prisoners.insert(new Prisoner{ this, 3968, 256 });

	m_Soldiers.insert(new Soldier{ this, 350, 50 });
	m_Soldiers.insert(new Soldier{ this, 435, 50 });
	m_Soldiers.insert(new Soldier{ this, 600, 50 });
	m_Soldiers.insert(new Soldier{ this, 760, 27 });
	m_Soldiers.insert(new Soldier{ this, 970, 27 });
	m_Soldiers.insert(new Soldier{ this, 970, 105 });
	m_Soldiers.insert(new Soldier{ this, 1234, 27 });
	m_Soldiers.insert(new Soldier{ this, 1286, 27 });
	m_Soldiers.insert(new Soldier{ this, 1367, 27 });
	m_Soldiers.insert(new Soldier{ this, 1452, 105 });
	m_Soldiers.insert(new Soldier{ this, 1642, 27 });
	m_Soldiers.insert(new Soldier{ this, 2090, 100 });
	m_Soldiers.insert(new Soldier{ this, 2280, 100 });
	m_Soldiers.insert(new Soldier{ this, 2410, 27 });
	m_Soldiers.insert(new Soldier{ this, 2450, 150 });
	m_Soldiers.insert(new Soldier{ this, 2610, 107 });
	m_Soldiers.insert(new Soldier{ this, 2710, 27 });
	m_Soldiers.insert(new Soldier{ this, 2710, 150 });
	m_Soldiers.insert(new Soldier{ this, 3280, 27 });
	m_Soldiers.insert(new Soldier{ this, 3400, 70 });
	m_Soldiers.insert(new Soldier{ this, 3565, 130 });
	m_Soldiers.insert(new Soldier{ this, 3720, 160 });
	m_Soldiers.insert(new Soldier{ this, 3800, 256 });
	m_Soldiers.insert(new Soldier{ this, 3928, 256 });

	m_PickUps.insert(new PickUpPig{ 2200, 27, 32, 20, this });
	m_PickUps.insert(new PickUpAmmo{ 1890, 27, 32, 32, this });

	m_pLevel->SetBoundariesWidth(m_SectionsXLimits[m_CurrentSection] - m_pLevel->GetBoundaries().left);

	m_pBossBuilding1 = new BossBuilding{ Point2f{ 2800, 32 }, m_BossBuilding1Textures, 3, 5, this, -35.0f };
	m_pBossBuilding2 = new BossBuilding{ Point2f{ 2853, 32 }, m_BossBuilding2Textures, 1, 10, this, 0 };
	m_pBossBuilding3 = new BossBuilding{ Point2f{ 3051, 32 }, m_BossBuilding3Textures, 2, 10, this, -111.0f };
	m_pSndMetalSlug->Play(0);
}

void Game::Cleanup( )
{
	delete m_pCharacter;
	delete m_pCamera;
	delete m_pLevel;

	for (int i{}; i < s_NrBgBuildings; ++i)
		delete m_pHousesTextures[i];

	for (const auto& bullet : m_Bullets)
		delete bullet;
	m_Bullets.clear();

	for (const auto& prisoner : m_Prisoners)
		delete prisoner;
	m_Prisoners.clear();

	for (const auto& soldier : m_Soldiers)
		delete soldier;
	m_Soldiers.clear();

	for (const auto& pickup : m_PickUps)
		delete pickup;
	m_PickUps.clear();

	for (const auto& grenade : m_Grenades)
		delete grenade;
	m_Grenades.clear();

	for (const auto& bomb : m_Bombs)
		delete bomb;
	m_Bombs.clear();

	for (const auto& ball : m_CanonBalls)
		delete ball;
	m_CanonBalls.clear();

	for (const auto& ball : m_LaserBalls)
		delete ball;
	m_CanonBalls.clear();

	delete m_pHelicopter;

	for (const auto& sfx : m_SFX)
		delete sfx;
	m_SFX.clear();

	delete m_pBossBuilding1;
	delete m_pBossBuilding2;
	delete m_pBossBuilding3;

	delete m_pSndExplosion;
	delete m_pSndGreat;
	delete m_pSndGunshot;
	delete m_pSndMetalSlug;
	delete m_pSndMissionComplete;
	delete m_pSndMissionStart;
	delete m_pSndOk;
	delete m_pSoundtrack;
	delete m_pSndGameOver;

}

void Game::Update(const float elapsedSec)
{
	switch (m_State)
	{
	case State::Game:
		UpdateGame(elapsedSec);
		break;

	default:
		break;
	}
}

void Game::Draw() const
{
	ClearBackground();
	glPushMatrix();
	{
		glScalef(GAME_SCALE, GAME_SCALE, 1);

		switch (m_State)
		{
		case State::Start:
			DrawStart();
			break;
		case State::Game:
			DrawGame();
			break;
		case State::Settings:
			DrawGame();
			DrawSettings();
			break;
		case State::End:
			DrawGame();
			break;
		}
	}
	glPopMatrix();
}

void Game::UpdateGame(const float elapsedSec)
{
	m_pCamera->Update(m_pCharacter->GetHitBox(), elapsedSec);
	m_pLevel->Update(elapsedSec, m_pCamera->GetPosition().x - m_pLevel->GetBoundaries().left);
	m_pCharacter->Update(elapsedSec);
	CheckBossStart();

	UpdateBullets(elapsedSec);
	UpdateGrenades(elapsedSec);
	UpdatePrisoners(elapsedSec);
	UpdateSoldiers(elapsedSec);
	UpdatePickUps(elapsedSec);
	UpdateBombs(elapsedSec);
	if (m_pHelicopter) UpdateHelicopter(elapsedSec);
	if (m_pBossBuilding1) m_pBossBuilding1->OnOverlap(m_pCharacter);
	if (m_pBossBuilding2) m_pBossBuilding2->OnOverlap(m_pCharacter);
	if (m_pBossBuilding3) m_pBossBuilding3->OnOverlap(m_pCharacter);
	if (!m_FinalBoss.IsSleeping()) m_FinalBoss.Update(elapsedSec);
	UpdateCanonBalls(elapsedSec);
	UpdateLaserBalls(elapsedSec);
	UpdateSFX(elapsedSec);
	m_HUD.Update(elapsedSec);
}

void Game::DrawStart() const
{
	m_TitleScreen.Draw();
}

void Game::DrawGame() const
{
	glPushMatrix();
	{
		m_pCamera->Transform();
		m_pLevel->DrawBackground();
		m_pLevel->DrawMidground();
		DrawBuildings();
		DrawBombs();
		if (m_pHelicopter) m_pHelicopter->Draw();
		if (m_pBossBuilding2) m_pBossBuilding2->Draw();
		if (m_pBossBuilding1) m_pBossBuilding1->Draw();
		if (m_pBossBuilding3) m_pBossBuilding3->Draw();
		m_FinalBoss.Draw();
		DrawPrisoners();
		DrawSoldiers();
		DrawPickUps();
		DrawCanonBalls();
		DrawLaserBalls();
		DrawBullets();
		DrawGrenades();
		m_pCharacter->Draw();
		DrawSFX();
		m_pLevel->DrawForeground();
	}
	glPopMatrix();
	m_HUD.Draw();
}

void Game::DrawSettings() const
{
	DrawGame();
	utils::SetColor(Color4f{ 112, 100, 246, 2 / 3.0f });
	utils::FillRect(Rectf{ 0, 0, 320, 224 });
	m_Settings.Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	bool melee{ false };
	switch (m_State)
	{
	case State::Start:
		if (e.keysym.sym == SDLK_RETURN)
		{
			m_State = State::Game;
			m_pSoundtrack->Play(0);
			m_pSndMissionStart->Play(0);
		}
		break;
	case State::Game:
		switch (e.keysym.sym)
		{
		case SDLK_j:
			if (m_pCharacter->IsAlive())
			{
				for (Prisoner* prisoner : m_Prisoners)
				{
					if (prisoner->GetState() == Prisoner::State::Idle && utils::IsOverlapping(m_pCharacter->GetHitBox(), prisoner->GetHitBox()))
					{
						m_pCharacter->Melee();
						prisoner->Free();
						melee = true;
						break; // there will never be more than 1 prisoner at the same place
					}
				}
				for (Soldier* soldier : m_Soldiers)
				{
					if (soldier->GetState() != Soldier::State::Dead && soldier->GetState() != Soldier::State::Dying && utils::IsOverlapping(m_pCharacter->GetHitBox(), soldier->GetHitBox()))
					{
						m_pCharacter->Melee();
						soldier->Kill();
						melee = true;
						//no break, there may be more than 1 prisoner at the same place.
					}
				}
				if (!melee)
				{
					m_pCharacter->FireBullet();
					SpawnBullet();
				}
			}
			break;
		case SDLK_k:
			if (m_pCharacter->GetGrenadeCount() && m_pCharacter->IsAlive()) // Grenades remaining
			{
				m_pCharacter->FireGrenade();
			}
			break;
		case SDLK_i:
			PrintConsoleInfo();
			break;
		case SDLK_ESCAPE:
			m_State = State::Settings;
		}
		break;
	case State::Settings:
		if (e.keysym.sym == SDLK_ESCAPE) m_State = State::Game;
		break;
	case State::End:
		break;
	}
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	switch (m_State)
	{
	case State::Settings:
		switch (e.button)
		{
		case SDL_BUTTON_LEFT:
			if (m_Settings.CheckMouseInput(Point2f{ float(e.x) / GAME_SCALE, (m_Window.height - float(e.y)) / GAME_SCALE })) UpdateSoundVolume();
			break;

		default: 
			break;
		}
		break;

	default: 
		break;
	}

}

Character* Game::GetCharacter() const
{
	return m_pCharacter;
}

Level* Game::GetLevel() const
{
	return m_pLevel;
}

Camera* Game::GetCamera() const
{
	return m_pCamera;
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.75f, 0.80f, 0.85f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::PrintConsoleInfo() const
{
	std::cout << "Controls:\n";
	std::cout << "\n";
	std::cout << "\tTitle Screen:\n";
	std::cout << "\t\tENTER : Start the game\n";
	std::cout << "\n";
	std::cout << "\tIn Game:\n";
	std::cout << "\t\tA/D : Move Left/Right\n";
	std::cout << "\t\tW : Aim Up\n";
	std::cout << "\t\tS : Crouch\n";
	std::cout << "\t\tSPACE : Jump\n";
	std::cout << "\t\tJ : Shoot Bullet\n";
	std::cout << "\t\tK : Throw Grenade\n";
	std::cout << "\t\tESCAPE : Pause game / Open sound settings\n";
	std::cout << "\n";
	std::cout << "\tSettings :\n";
	std::cout << "\t\tLMB : Click on volume buttons\n";
	std::cout << "\t\tESCAPE : Resume game\n";
	std::cout << std::endl;
}

void Game::SpawnBullet()
{
	float bulletStartX{};
	float bulletStartY{};
	bool horizontal{};
	int direction{};
	switch (m_pCharacter->GetAim())
	{
	case Character::Aim::Up:
		bulletStartX = m_pCharacter->GetHitBox().left + m_BulletXOffset;
		bulletStartY = m_pCharacter->GetHitBox().bottom + m_pCharacter->GetHitBox().height;
		horizontal = false;
		direction = 1;
		break;

	case Character::Aim::Left:
		bulletStartX = m_pCharacter->GetHitBox().left;
		bulletStartY = m_pCharacter->GetHitBox().bottom + m_BulletYOffset * (m_pCharacter->IsCrouching() ? 0.6f : 1);
		horizontal = true;
		direction = -1;
		break;

	case Character::Aim::Right:
		bulletStartX = m_pCharacter->GetHitBox().left + m_pCharacter->GetHitBox().width;
		bulletStartY = m_pCharacter->GetHitBox().bottom + m_BulletYOffset * (m_pCharacter->IsCrouching() ? 0.6f : 1);
		horizontal = true;
		direction = 1;
		break;
	}
	const auto newBullet{ new Bullet{ Point2f{bulletStartX, bulletStartY}, &m_BulletTexture, horizontal, direction } };
	m_Bullets.insert(newBullet);
	m_pSndGunshot->Play(0);
}

void Game::UpdateBullets(const float elapsedSec)
{
	std::vector<Bullet*> toDelete;
	for (const auto& bullet : m_Bullets)
	{
		// Update bullet
		bullet->Update(elapsedSec);

		// Out of the Camera view
		if (bullet->GetPosition().x > m_pCamera->GetPosition().x + m_pCamera->GetWidth() || bullet->GetPosition().x < m_pCamera->GetPosition().x)
			toDelete.push_back(bullet);
	}

	for (const auto& bullet : toDelete)
	{
		m_Bullets.erase(bullet);
		delete bullet;
	}

	toDelete.clear();

	// Bullet hitting a prisoner
	for (const auto& bullet : m_Bullets)
		for (const auto& prisoner : m_Prisoners)
			if (utils::IsPointInRect(bullet->GetPosition(), prisoner->GetHitBox()) && prisoner->GetState() == Prisoner::State::Idle)
			{
				prisoner->Free();
				m_pCharacter->IncPrisonerFreed();
				toDelete.push_back(bullet);
				break; // out of the prisoners loop, go to the next bullet
			}

	for (const auto& bullet : toDelete)
	{
		m_Bullets.erase(bullet);
		delete bullet;
	}

	toDelete.clear();

	// Bullet hitting a soldier
	for (const auto& bullet : m_Bullets)
		for (const auto& soldier : m_Soldiers)
			if (soldier->GetState() != Soldier::State::Dead && soldier->GetState() != Soldier::State::Dying && utils::IsPointInRect(bullet->GetPosition(), soldier->GetHitBox()))
			{
				soldier->Kill();
				m_pCharacter->AddScore(Soldier::s_Score);
				toDelete.push_back(bullet);
				break; // out of the soldier loop, go to the next bullet
			}

	for (const auto& bullet : toDelete)
	{
		m_Bullets.erase(bullet);
		delete bullet;
	}

	toDelete.clear();

	// Bullet hitting a helicopter
	if (m_pHelicopter)
	{
		for (const auto& bullet : m_Bullets)
			if (utils::IsPointInRect(bullet->GetPosition(), m_pHelicopter->GetHitBox()))
			{
				DamageHelicopter(Bullet::s_Dmg);
				toDelete.push_back(bullet);
				break; // Impossible for more than one bullet to hit the helicopter in the same frame
			}

		for (const auto& bullet : toDelete)
		{
			m_Bullets.erase(bullet);
			delete bullet;
		}

		toDelete.clear();
	}

	// Bullet hitting BossBuilding1
	if (m_pBossBuilding1)
	{
		for (const auto& bullet : m_Bullets)
			if (utils::IsPointInRect(bullet->GetPosition(), m_pBossBuilding1->GetHitBox()))
			{
				DamageBossBuilding1(Bullet::s_Dmg);
				toDelete.push_back(bullet);
				break; // Impossible for more than one bullet to hit the Building in the same frame
			}

		for (const auto& bullet : toDelete)
		{
			m_Bullets.erase(bullet);
			delete bullet;
		}

		toDelete.clear();
	}

	// Bullet hitting BossBuilding2
	else if (m_pBossBuilding2)
	{
		for (const auto& bullet : m_Bullets)
		{
			if (utils::IsPointInRect(bullet->GetPosition(), m_pBossBuilding2->GetHitBox()))
			{
				DamageBossBuilding2(Bullet::s_Dmg);
				toDelete.push_back(bullet);
				break; // Impossible for more than one bullet to hit the Building in the same frame
			}
		}

		for (const auto& bullet : toDelete)
		{
			m_Bullets.erase(bullet);
			delete bullet;
		}

		toDelete.clear();
	}

	// Bullet hitting BossBuilding2
	else if (m_pBossBuilding3)
	{
		for (const auto& bullet : m_Bullets)
		{
			if (utils::IsPointInRect(bullet->GetPosition(), m_pBossBuilding3->GetHitBox()))
			{
				DamageBossBuilding3(Bullet::s_Dmg);
				toDelete.push_back(bullet);
				break; // Impossible for more than one bullet to hit the Building in the same frame
			}
		}

		for (const auto& bullet : toDelete)
		{
			m_Bullets.erase(bullet);
			delete bullet;
		}

		toDelete.clear();
	}

	// Bullet hitting FinalBoss
	else if (m_CurrentSection == 4)
	{
		for (const auto& bullet : m_Bullets)
		{
			if (utils::IsPointInRect(bullet->GetPosition(), m_FinalBoss.GetHitBox()))
			{
				m_FinalBoss.Hit(Bullet::s_Dmg);
				toDelete.push_back(bullet);
				break; // Impossible for more than one bullet to hit the FinalBoss in the same frame
			}
		}

		for (const auto& bullet : toDelete)
		{
			m_Bullets.erase(bullet);
			delete bullet;
		}

		toDelete.clear();
	}
}

void Game::DrawBullets() const
{
	for (const auto& bullet : m_Bullets)
		bullet->Draw();
}

void Game::UpdatePrisoners(const float elapsedSec)
{
	std::vector<Prisoner*> toDelete;
	for (const auto& prisoner : m_Prisoners)
	{
		// Update prisoner
		prisoner->Update(elapsedSec);

		// Out of the Camera view
		if (prisoner->GetHitBox().left + prisoner->GetHitBox().width < m_pCamera->GetPosition().x)
			toDelete.push_back(prisoner);
	}

	for (const auto& prisoner : toDelete)
	{
		m_Prisoners.erase(prisoner);
		delete prisoner;
	}

	toDelete.clear();

	// Prisoner overlapped by the character
	for (const auto& prisoner : m_Prisoners)
		if (utils::IsOverlapping(prisoner->GetHitBox(), m_pCharacter->GetHitBox()))
			prisoner->OnOverlap();

}

void Game::DrawPrisoners() const
{
	for (const auto& prisoner : m_Prisoners)
		prisoner->Draw();
}

void Game::UpdateSoldiers(const float elapsedSec)
{
	std::vector<Soldier*> toDelete;
	for (const auto& soldier : m_Soldiers)
	{
		soldier->Update(elapsedSec);
		// Out of the Camera view
		if (soldier->GetHitBox().left + soldier->GetHitBox().width < m_pCamera->GetPosition().x)
			toDelete.push_back(soldier);
	}

	for (const auto& soldier : toDelete)
	{
		m_Soldiers.erase(soldier);
		delete soldier;
	}

	toDelete.clear();
}

void Game::DrawSoldiers() const
{
	for (const auto& soldier : m_Soldiers)
		soldier->Draw();
}

void Game::UpdatePickUps(const float elapsedSec)
{
	std::vector<PickUp*> toDelete;
	for (const auto& pickup : m_PickUps)
	{
		// Update pickup
		pickup->Update(elapsedSec);
		if (utils::IsOverlapping(pickup->GetHitBox(), m_pCharacter->GetHitBox()))
		{
			pickup->OnOverlap(m_pCharacter);
			m_pSndOk->Play(0);
			toDelete.push_back(pickup);
		}
	}

	for (const auto& pickup : toDelete)
	{
		m_PickUps.erase(pickup);
		delete pickup;
	}

	toDelete.clear();
}

void Game::DrawPickUps() const
{
	for (const auto& pickup : m_PickUps)
		pickup->Draw();
}

void Game::UpdateGrenades(const float elapsedSec)
{
	std::vector<Grenade*> toDelete;
	for (const auto& grenade : m_Grenades)
	{
		grenade->Update(elapsedSec);
		if (m_CurrentSection == 4 && utils::IsOverlapping(grenade->GetHitBox(), m_FinalBoss.GetHitBox()))
		{
			m_FinalBoss.Hit(Grenade::s_Dmg);
			m_SFX.insert(new GrenadeBlastSFX{ grenade->GetBottomLeft() + m_GrenadeBlastSFXOffset });
			m_pSndExplosion->Play(0);
			toDelete.push_back(grenade);
		}

		else if (grenade->IsOnGround())
		{
			if (utils::IsOverlapping(grenade->GetHitBox(), m_pCharacter->GetHitBox())
				&& m_pCharacter->IsVulnerable())
			{
				m_pCharacter->Kill();
			}

			for (Prisoner* prisoner : m_Prisoners)
				if (prisoner->GetState() == Prisoner::State::Idle
					&& utils::IsOverlapping(grenade->GetHitBox(), prisoner->GetHitBox()))
				{
					m_pCharacter->IncPrisonerFreed();
					prisoner->Free();
				}		
			for (Soldier* soldier : m_Soldiers)
				if (soldier->GetState() != Soldier::State::Dead && soldier->GetState() != Soldier::State::Dying
					&& utils::IsOverlapping(grenade->GetHitBox(), soldier->GetHitBox()))
				{
					m_pCharacter->AddScore(Soldier::s_Score);
					soldier->Kill();
				}
			if (m_pBossBuilding1 && utils::IsOverlapping(m_pBossBuilding1->GetHitBox(), grenade->GetHitBox()))
				DamageBossBuilding1(Grenade::s_Dmg);
			else if (m_pBossBuilding2 && utils::IsOverlapping(m_pBossBuilding2->GetHitBox(), grenade->GetHitBox()))
				DamageBossBuilding2(Grenade::s_Dmg);
			else if (m_pBossBuilding3 && utils::IsOverlapping(m_pBossBuilding3->GetHitBox(), grenade->GetHitBox()))
				DamageBossBuilding3(Grenade::s_Dmg);
			m_SFX.insert(new GrenadeBlastSFX{ grenade->GetBottomLeft() + m_GrenadeBlastSFXOffset });
			m_pSndExplosion->Play(0);
			toDelete.push_back(grenade);
		}
	}

	for (const auto& grenade : toDelete)
	{
		m_Grenades.erase(grenade);
		delete grenade;
	}

	toDelete.clear();
}

void Game::DrawGrenades() const
{
	for (const auto& grenade : m_Grenades)
		grenade->Draw();
}

void Game::DrawBuildings() const
{
	for (int i{}; i < s_NrBgBuildings; ++i)
		m_BgBuildings[i].Draw();
}

void Game::UpdateHelicopter(const float elapsedSec)
{
	m_pHelicopter->Update(m_pCharacter->GetHitBox(), elapsedSec);
	if (m_pHelicopter->GetState() == Helicopter::State::Leaving
		&& m_pHelicopter->GetHitBox().bottom > m_pCamera->GetHeight())
	{
		delete m_pHelicopter;
		m_pHelicopter = nullptr;
	}
}

void Game::DamageHelicopter(const int damage)
{
	m_pHelicopter->Hit(damage);
	if (m_pHelicopter->GetHP() <= 0)
	{
		if (m_pHelicopter->IsWorldBoss())
		{
			++m_CurrentSection;
			m_pLevel->SetBoundariesWidth(m_SectionsXLimits[m_CurrentSection] - m_pLevel->GetBoundaries().left);
		}
		m_SFX.insert(new ExplosionSFX{ Point2f{m_pHelicopter->GetHitBox().left, m_pHelicopter->GetHitBox().bottom} + m_ExplosionSFXOffset, 2.0f });
		m_pSndGreat->Play(0);
		delete m_pHelicopter;
		m_pHelicopter = nullptr;
	}
}

void Game::DamageBossBuilding1(const int damage)
{
	m_pBossBuilding1->Hit(damage);
	if (m_pBossBuilding1->IsBroken())
	{
		++m_CurrentSection;
		m_pLevel->SetBoundariesWidth(m_SectionsXLimits[m_CurrentSection] - m_pLevel->GetBoundaries().left);
		m_SFX.insert(new ExplosionSFX{ Point2f{m_pBossBuilding1->GetHitBox().left, m_pBossBuilding1->GetHitBox().bottom} + m_ExplosionSFXOffset, 2.0f });
		m_pSndGreat->Play(0);
		delete m_pBossBuilding1;
		m_pBossBuilding1 = nullptr;
	}
}

void Game::DamageBossBuilding2(const int damage)
{
	m_pBossBuilding2->Hit(damage);
	if (m_pBossBuilding2->IsBroken())
	{
		++m_CurrentSection;
		m_pLevel->SetBoundariesWidth(m_SectionsXLimits[m_CurrentSection] - m_pLevel->GetBoundaries().left);
		m_SFX.insert(new ExplosionSFX{ Point2f{m_pBossBuilding2->GetHitBox().left, m_pBossBuilding2->GetHitBox().bottom} + m_ExplosionSFXOffset, 2.0f });
		if (m_pHelicopter) m_pHelicopter->Leave();
		m_pSndGreat->Play(0);
		delete m_pBossBuilding2;
		m_pBossBuilding2 = nullptr;
	}
}

void Game::DamageBossBuilding3(const int damage)
{
	m_pBossBuilding3->Hit(damage);
	if (m_pBossBuilding3->IsBroken())
	{
		++m_CurrentSection;
		m_pLevel->SetBoundariesWidth(m_SectionsXLimits[m_CurrentSection] - m_pLevel->GetBoundaries().left);
		m_SFX.insert(new ExplosionSFX{ Point2f{m_pBossBuilding3->GetHitBox().left, m_pBossBuilding3->GetHitBox().bottom} + m_ExplosionSFXOffset, 2.0f });
		m_pSndGreat->Play(0);
		delete m_pBossBuilding3;
		m_pBossBuilding3 = nullptr;
	}
}

void Game::CheckBossStart()
{
	if (m_CurrentSection == 0 && m_pCharacter->GetHitBox().left >= m_Boss1StartX && !m_pHelicopter)
	{
		m_pHelicopter = new Helicopter{ 1683, 300, true, this };
	}
	else if (m_CurrentSection == 2 && !m_BossBuilding2HelicopterSpawned)
	{
		m_pHelicopter = new Helicopter{ 2747, 300, false, this };
		m_BossBuilding2HelicopterSpawned = true;
	}
	else if (m_CurrentSection == 4 && m_FinalBoss.IsSleeping()) m_FinalBoss.WakeUp();
}

void Game::UpdateBombs(const float elapsedSec)
{
	std::vector<Bomb*> toDelete;
	for (Bomb* bomb : m_Bombs)
	{
		bomb->Update(elapsedSec);
		if (bomb->IsOnGround())
		{
			if (utils::IsOverlapping(bomb->GetHitBox(), m_pCharacter->GetHitBox()) && m_pCharacter->IsVulnerable()) m_pCharacter->Kill();
			for (Prisoner* prisoner : m_Prisoners)
				if (prisoner->GetState() == Prisoner::State::Idle && utils::IsOverlapping(bomb->GetHitBox(), prisoner->GetHitBox()))
					prisoner->Free();
			for (Soldier* soldier : m_Soldiers)
				if (soldier->GetState() != Soldier::State::Dead && soldier->GetState() != Soldier::State::Dying && utils::IsOverlapping(bomb->GetHitBox(), soldier->GetHitBox()))
					soldier->Kill();
			m_SFX.insert(new BombImpactSFX{ bomb->GetBottomLeft() + m_BombImpactSFXOffset });
			m_pSndExplosion->Play(0);
			toDelete.push_back(bomb);
		}
	}

	for (Bomb* bomb : toDelete)
	{
		m_Bombs.erase(bomb);
		delete bomb;
	}

	toDelete.clear();
}

void Game::DrawBombs() const
{
	for (const auto& bomb : m_Bombs)
		bomb->Draw();
}

void Game::UpdateSFX(float elapsedSec)
{
	std::vector<SFX*> toDelete;
	for (SFX* sfx : m_SFX)
	{
		sfx->Update(elapsedSec);
		if (sfx->GetCycle())
			toDelete.push_back(sfx);
	}

	for (SFX* sfx : toDelete)
	{
		m_SFX.erase(sfx);
		delete sfx;
	}

	toDelete.clear();
}

void Game::DrawSFX() const
{
	for (const auto& sfx : m_SFX)
		sfx->Draw();
}

void Game::UpdateCanonBalls(const float elapsedSec)
{
	std::vector<CanonBall*> toDelete;
	for (const auto& ball : m_CanonBalls)
	{
		ball->Update(elapsedSec);
		if (utils::IsOverlapping(ball->GetHitBox(), m_pCharacter->GetHitBox()) && m_pCharacter->IsVulnerable())
		{
			m_pCharacter->Kill();
			toDelete.push_back(ball);
		}
		else if (ball->IsOnGround())
			toDelete.push_back(ball);
	}

	for (CanonBall* ball : toDelete)
	{
		m_CanonBalls.erase(ball);
		delete ball;
	}

	toDelete.clear();
}

void Game::DrawCanonBalls() const
{
	for (const auto& ball : m_CanonBalls)
		ball->Draw();
}

void Game::UpdateLaserBalls(float elapsedSec)
{
	std::vector<LaserBall*> toDelete;
	for (const auto& ball : m_LaserBalls)
	{
		ball->Update(elapsedSec);
		if (utils::IsOverlapping(ball->GetHitBox(), m_pCharacter->GetHitBox()) && m_pCharacter->IsVulnerable())
		{
			m_pCharacter->Kill();
			toDelete.push_back(ball);
		}
		else if (ball->GetHitBox().left + ball->GetHitBox().width < m_pCamera->GetPosition().x)
			toDelete.push_back(ball);
	}

	for (const auto& ball : toDelete)
	{
		m_LaserBalls.erase(ball);
		delete ball;
	}

	toDelete.clear();
}

void Game::DrawLaserBalls() const
{
	for (const auto& ball : m_LaserBalls)
		ball->Draw();
}

void Game::UpdateSoundVolume() const
{
	m_pSoundtrack->SetVolume(m_SoundtrackVolume);
	m_pSndMetalSlug->SetVolume(m_SoundEffectsVolume);
	m_pSndMissionComplete->SetVolume(m_SoundEffectsVolume);
	m_pSndMissionStart->SetVolume(m_SoundEffectsVolume);
	m_pSndGreat->SetVolume(m_SoundEffectsVolume);
	m_pSndOk->SetVolume(m_SoundEffectsVolume);
	m_pSndExplosion->SetVolume(m_SoundEffectsVolume);
	m_pSndGunshot->SetVolume(m_SoundEffectsVolume);
	m_pSndGameOver->SetVolume(m_SoundEffectsVolume);
}

void Game::SpawnPickUp(const Point2f& pos)
{
	if (rand() % 2)
		m_PickUps.insert(new PickUpPig{ pos.x, pos.y, 32, 20, this });

	else
		m_PickUps.insert(new PickUpAmmo{ pos.x, pos.y, 32, 32, this });
}

void Game::SpawnGrenade(const Point2f& pos, const int dir)
{
	m_Grenades.insert(new Grenade{ this, pos, &m_GrenadeTexture, dir });
}

void Game::SpawnBomb(const Point2f& pos)
{
	m_Bombs.insert(new Bomb{ this, pos.x, pos.y, 25, 26 });
}

void Game::SpawnCanonBall(const Point2f& pos, const float velocity)
{
	m_CanonBalls.insert(new CanonBall{ this, pos, &m_CanonBallTexture, velocity });
}

void Game::SpawnLaserBall(const Point2f& pos)
{
	m_LaserBalls.insert(new LaserBall{ pos, &m_LaserBallTexture });
}

void Game::IncMusicVolume()
{
	if (m_SoundtrackVolume < 10)
		++m_SoundtrackVolume;
}

void Game::DecMusicVolume()
{
	if (m_SoundtrackVolume > 0)
		--m_SoundtrackVolume;
}

void Game::IncEffectsVolume()
{
	if (m_SoundEffectsVolume < 10)
		++m_SoundEffectsVolume;
}

void Game::DecEffectsVolume()
{
	if (m_SoundEffectsVolume > 0)
		--m_SoundEffectsVolume;
}

int Game::GetMusicVolume() const
{
	return m_SoundtrackVolume;
}

int Game::GetEffectsVolume() const
{
	return m_SoundEffectsVolume;
}

void Game::EndGame()
{
	m_State = State::End;
	m_pSoundtrack->Stop();
	m_pSndMissionComplete->Play(0);
}

void Game::PlayGameOver()
{
	if (!m_GameOverPlayed)
	{
		m_pSoundtrack->Stop();
		m_pSndGameOver->Play(0);
		m_GameOverPlayed = true;
	}	
}
