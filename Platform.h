#pragma once
#include "WorldStatic.h"

class WorldDynamic;
class Level;

class Platform final : public WorldStatic
{
public:
	Platform(float left, float bottom, float width, float height, Game* game);
	~Platform() override = default;
	Platform(const Platform& other) = delete;
	Platform& operator=(const Platform& other) = delete;
	Platform(Platform&& other) noexcept = delete;
	Platform& operator=(Platform&& other) noexcept = delete;

	void OnOverlap(WorldDynamic* wd) override;
};

