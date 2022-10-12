#pragma once
#include "Texture.h"

class Bullet final
{
public:
	Bullet(const Point2f& startPoint, Texture* texture, bool horizontal, int direction);
	~Bullet() = default;
	Bullet(const Bullet&) = delete;
	Bullet& operator=(const Bullet&) = delete;
	Bullet(Bullet&&) noexcept = delete;
	Bullet& operator=(Bullet&&) noexcept = delete;

	void Update(float elapsedSec);
	void Draw() const;

	[[nodiscard]] Point2f GetPosition() const;

	static constexpr int s_Dmg{ 1 };

private:
	Point2f m_Position;
	Texture* const m_pTexture;
	const bool  m_IsHorizontal;
	const int m_Direction;
	const float m_Speed;
};

