#pragma once

class Texture;

class LaserBall final
{
public:
	LaserBall(const Point2f& startPoint, Texture* texture);
	~LaserBall() = default;
	LaserBall(const LaserBall&) = delete;
	LaserBall& operator=(const LaserBall&) = delete;
	LaserBall(LaserBall&&) noexcept = delete;
	LaserBall& operator=(LaserBall&&) noexcept = delete;

	void Update(float elapsedSec);
	void Draw() const;

	[[nodiscard]] Rectf GetHitBox() const;

private:
	Rectf m_HitBox;
	Texture* const m_pTexture;
	const float m_Speed;
};

