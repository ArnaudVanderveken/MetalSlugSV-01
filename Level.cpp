#include "pch.h"
#include "Level.h"
#include "Game.h"
#include "Sprite.h"
#include "Texture.h"
#include "Platform.h"
#include "Camera.h"
#include "utils.h"
#include "SVGParser.h"
#include "WorldDynamic.h"
#include "Character.h"


Level::Level(Game* game)
	: m_pForeground{ new Sprite{ "Resources/Images/Level/fg.png", 1, 8, 1 / 24.0f } }
	, m_pMidground{ new Sprite{ "Resources/Images/Level/mg.png", 1, 8, 1 / 24.0f } }
	, m_pBackgroundBoat{ new Sprite{ "Resources/Images/Level/bg-boat.png", 8, 1, 1 / 24.0f } }
	, m_pBackgroundSwamp{ new Texture{ "Resources/Images/Level/bg-swamp.png" } }
	, m_ParallaxFactorBGSwamp{ 0.33f }
	, m_ParallaxFactorBGBoat{ 0.95f }
	, m_BGBSwampStartX{ 1720.0f }
	, m_BGBoatStartX{ 2830.0f }
	, m_pGame{ game }
{
	SVGParser::GetVerticesFromSvgFile("Resources/Images/Level/Level.svg", m_Vertices);
	m_Boundaries = Rectf{ 0, 0, m_pMidground->GetFrameWidth(), m_pMidground->GetFrameHeight() };
	m_Platforms.push_back(new Platform{ 855, 60, 60, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 935, 100, 175, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 1110, 60, 60, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 1175, 100, 135, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 1355, 100, 165, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 1525, 60, 60, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 2065, 91, 80, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 2156, 138, 45, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 2258, 91, 80, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 2350, 131, 46, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 2395, 137, 143, 7 , m_pGame });
	m_Platforms.push_back(new Platform{ 2446, 57, 36, 7 , m_pGame });
	m_Platforms.push_back(new Platform{ 2462, 97, 36, 7 , m_pGame });
	m_Platforms.push_back(new Platform{ 2565, 97, 100, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 2695, 137, 45, 7 , m_pGame });
	m_Platforms.push_back(new Platform{ 3800, 251, 90, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 3872, 185, 32, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 3905, 217, 43, 5 , m_pGame });
	m_Platforms.push_back(new Platform{ 3935, 251, 124, 5 , m_pGame });
}

Level::~Level()
{
	delete m_pForeground;
	delete m_pMidground;
	delete m_pBackgroundSwamp;
	delete m_pBackgroundBoat;

	for (const auto& platform : m_Platforms)
		delete platform;
}

void Level::Update(const float elapsedSec, const float boundariesDX)
{
	m_pForeground->Update(elapsedSec);
	m_pMidground->Update(elapsedSec);
	m_pBackgroundBoat->Update(elapsedSec);
	m_Boundaries.left += boundariesDX;
	m_Boundaries.width -= boundariesDX;
}

void Level::DrawBackground() const
{
	if (m_pGame->GetCamera()->GetPosition().x > m_BGBoatStartX)
		m_pBackgroundBoat->Draw(Point2f{ m_BGBoatStartX + m_ParallaxFactorBGBoat * (m_pGame->GetCamera()->GetPosition().x - m_BGBoatStartX), 32 });

	if (m_pGame->GetCamera()->GetPosition().x > m_BGBSwampStartX)
		m_pBackgroundSwamp->Draw(Point2f{ m_BGBSwampStartX + m_ParallaxFactorBGSwamp * (m_pGame->GetCamera()->GetPosition().x - m_BGBSwampStartX), 48 });
}

void Level::DrawMidground() const
{
	m_pMidground->Draw();
}

void Level::DrawForeground() const
{
	m_pForeground->Draw();
}

void Level::HandleCollision(WorldDynamic* wd, const bool ignorePlatforms) const
{
	utils::HitInfo hitInfo{};
	constexpr float marginX{ 3.0f }, marginY{ 5.0f };
	Point2f p1{ wd->GetHitBox().left + wd->GetHitBox().width - marginX, wd->GetHitBox().bottom + wd->GetHitBox().height };
	Point2f p2{ wd->GetHitBox().left + wd->GetHitBox().width - marginX, wd->GetHitBox().bottom };

	if (utils::Raycast(m_Vertices[0], p1, p2, hitInfo))
	{
		wd->SetHitBoxBottom(hitInfo.intersectPoint.y);
		wd->SetVelocityY(0);
	}
	else
	{
		p1 = Point2f{ wd->GetHitBox().left + marginX, wd->GetHitBox().bottom + wd->GetHitBox().height };
		p2 = Point2f{ wd->GetHitBox().left + marginX, wd->GetHitBox().bottom };

		if (utils::Raycast(m_Vertices[0], p1, p2, hitInfo))
		{
			wd->SetHitBoxBottom(hitInfo.intersectPoint.y);
			wd->SetVelocityY(0);
		}
	}

	p1 = Point2f{ wd->GetHitBox().left, wd->GetHitBox().bottom + marginY };
	p2 = Point2f{ wd->GetHitBox().left + wd->GetHitBox().width, wd->GetHitBox().bottom + marginY };

	if (utils::Raycast(m_Vertices[0], p1, p2, hitInfo))
	{
		hitInfo.lambda < 0.5 ? wd->SetHitBoxLeft(hitInfo.intersectPoint.x) : wd->SetHitBoxLeft(hitInfo.intersectPoint.x - wd->GetHitBox().width);
		wd->SetVelocityX(0);
	}
	
	if (!ignorePlatforms)
		for (Platform* platform : m_Platforms)
			if (utils::IsOverlapping(wd->GetHitBox(), platform->GetHitBox()))
				platform->OnOverlap(wd);
}

bool Level::IsOnGround(WorldDynamic* wd, const bool ignorePlatforms) const
{
	utils::HitInfo hitInfo{};
	const Point2f p1{ wd->GetHitBox().left + wd->GetHitBox().width / 2, wd->GetHitBox().bottom + wd->GetHitBox().height };
	const Point2f p2{ wd->GetHitBox().left + wd->GetHitBox().width / 2, wd->GetHitBox().bottom - 1 };

	return (utils::Raycast(m_Vertices[0], p1, p2, hitInfo) || (!ignorePlatforms && IsOnPlatform(wd)));
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::SetBoundariesWidth(const float width)
{
	m_Boundaries.width = width;
}

void Level::DeleteFinalBossPlatforms()
{
	//Delete last four platforms, corresponding to the four platforms of the final boss.
	for (int i{}; i < 4; ++i)
	{
		delete m_Platforms[m_Platforms.size() - 1];
		m_Platforms.pop_back();
	}
}

bool Level::IsOnPlatform(const WorldDynamic* wd) const
{
	for (const auto& platform : m_Platforms)
		if (utils::IsOverlapping(wd->GetHitBox(), platform->GetHitBox()) && wd->GetVelocity().y == 0)
			return true;

	return false;
}
