#pragma once
#include <vector>

class Game;

class Camera final
{
public:
	Camera(float width, float height, Game* game);
	~Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera(Camera&&) noexcept = delete;
	Camera& operator=(Camera&&) noexcept = delete;
	void Transform() const;
	void Update(const Rectf& target, float elapsedSec);

	[[nodiscard]] Point2f GetPosition() const;
	[[nodiscard]] float GetWidth() const;
	[[nodiscard]] float GetHeight() const;

private:
	const float m_Width, m_Height, m_Margin;
	Point2f m_BottomLeft;
	Game* const m_pGame;
	const Point2f m_StartClimb, m_EndClimb;
	float m_Velocity;
	const float m_MaxSpeed;

	void Track(const Rectf& target, float elapsedSec);
	void Clamp();
};

