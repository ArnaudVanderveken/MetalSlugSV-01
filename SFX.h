#pragma once
#include "Sprite.h"
class SFX : public Sprite
{
public:
	SFX(const std::string& filename, int nrCols = 1, int nrRows = 1, float frameSec = 0, const Point2f& pos = Point2f{}, const float scale = 1.0f);
	~SFX() override = default;
	SFX(const SFX& other) = delete;
	SFX& operator=(const SFX& other) = delete;
	SFX(SFX&& other) noexcept = delete;
	SFX& operator=(SFX&& other) noexcept = delete;

	void Draw() const;

protected:
	const Point2f m_Position;
	const float m_Scale;
};

