#pragma once
#include "Vector2f.h"

class Game;

class WorldDynamic
{
public:
	WorldDynamic(Game* game, float left, float bottom, float width = 40.0f, float height = 40.0f, float runSpeed = -40.0f, float jumpSpeed = 0.0f);
	virtual ~WorldDynamic() = default;

	WorldDynamic(const WorldDynamic& other) = delete;
	WorldDynamic& operator=(const WorldDynamic& other) = delete;

	WorldDynamic(WorldDynamic&& other) noexcept = delete;
	WorldDynamic& operator=(WorldDynamic&& other) noexcept = delete;

	virtual void Update(float elapsedSec);
	virtual void Draw() const = 0;

	[[nodiscard]] Point2f GetBottomLeft() const;
	[[nodiscard]] Rectf GetHitBox() const;
	[[nodiscard]] Vector2f GetVelocity() const;

	void SetVelocityX(float x);
	void SetVelocityY(float y);
	void SetHitBoxBottom(float bottom);
	void SetHitBoxLeft(float left);

protected:
	Rectf m_HitBox;
	float m_HorizontalSpeed, m_JumpSpeed, m_Gravity;
	Vector2f m_Velocity;
	Game* const m_pGame;
};

