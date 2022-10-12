#pragma once
#include "WorldDynamic.h"
#include "Sprite.h"

class Game;

class Bomb final : public WorldDynamic
{
public:
	Bomb(Game* game, float left, float bottom, float width, float height);
	~Bomb() override = default;
	Bomb(const Bomb&) = delete;
	Bomb& operator=(const Bomb&) = delete;
	Bomb(Bomb&&) noexcept = delete;
	Bomb& operator=(Bomb&&) noexcept = delete;

	void Update(float elapsedSec) override;
	void Draw() const override;

	bool IsOnGround();

private:
	Sprite m_Sprite;
};

