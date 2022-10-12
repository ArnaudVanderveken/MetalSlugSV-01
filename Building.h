#pragma once
#include "WorldStatic.h"

class Game;
class Texture;
class WorldDynamic;

class Building : public WorldStatic
{
public:
	Building(const Point2f& pos, Texture* textures, int lastStage, int hitsPerStage, Game* game);
	~Building() override = default;
	Building(const Building&) = delete;
	Building& operator=(const Building&) = delete;
	Building(Building&&) noexcept = delete;
	Building& operator=(Building&&) noexcept = delete;

	virtual void OnOverlap(WorldDynamic* wd) override;

	virtual void Draw() const;
	virtual void Hit(int damages = 1) final;

protected:
	int m_LastStage, m_HitsPerStage, m_CurrentStage, m_AccuHits;
	Texture* const m_pTextures;
};

