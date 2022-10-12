#pragma once
#include <vector>
#include "Vector2f.h"

class WorldDynamic;
class Game;
class Sprite;
class Texture;
class Platform;


class Level final
{
public:
	Level(Game* game);
	~Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) noexcept = delete;
	Level& operator=(Level&& other) noexcept = delete;

	void Update(float elapsedSec, float boundariesDX);
	void DrawBackground() const;
	void DrawMidground() const;
	void DrawForeground() const;
	void HandleCollision(WorldDynamic* wd, bool ignorePlatforms = false) const;
	bool IsOnGround(WorldDynamic* wd, bool ignorePlatforms = false) const;

	[[nodiscard]] Rectf GetBoundaries() const;
	void SetBoundariesWidth(float width);

	void DeleteFinalBossPlatforms();

private:
	std::vector<std::vector<Point2f>> m_Vertices;
	Sprite *m_pForeground, *m_pMidground, *m_pBackgroundBoat;
	Texture* m_pBackgroundSwamp;
	
	const float m_ParallaxFactorBGSwamp, m_ParallaxFactorBGBoat;
	const float m_BGBSwampStartX, m_BGBoatStartX;

	Rectf m_Boundaries;
	Game* const m_pGame;

	std::vector<Platform*> m_Platforms;

	bool IsOnPlatform(const WorldDynamic* wd) const;
};

