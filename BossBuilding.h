#pragma once
#include "Building.h"

class BossBuilding final : public Building
{
public:
	BossBuilding(const Point2f& pos, Texture* textures, int lastStage, int hitsPerStage, Game* game, float textureXOffset);
	~BossBuilding() override = default;
	BossBuilding(const BossBuilding&) = delete;
	BossBuilding& operator=(const BossBuilding&) = delete;
	BossBuilding(BossBuilding&&) noexcept = delete;
	BossBuilding& operator=(BossBuilding&&) noexcept = delete;

	virtual void OnOverlap(WorldDynamic* wd) override;
	virtual void Draw() const override;

	[[nodiscard]] bool IsBroken() const;

private:
	float m_TextureXOffset;
};

