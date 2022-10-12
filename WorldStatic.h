#pragma once
#include "Vector2f.h"

class WorldDynamic;
class Game;

class WorldStatic
{
public:
	WorldStatic(float left, float bottom, float width, float height, Game* game);
	virtual ~WorldStatic() = default;
	WorldStatic(const WorldStatic& other) = delete;
	WorldStatic& operator=(const WorldStatic& other) = delete;
	WorldStatic(WorldStatic&& other) noexcept = delete;
	WorldStatic& operator=(WorldStatic&& other) noexcept = delete;

	virtual void OnOverlap(WorldDynamic* wd) = 0;

	Rectf GetHitBox() const;
	
protected:
	Rectf m_HitBox;
	Game* const m_pGame;
};

