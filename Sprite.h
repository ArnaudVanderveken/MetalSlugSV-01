#pragma once
#include <string>
#include "Texture.h"

class Sprite
{
public:
	Sprite( const std::string& filename, int nrCols = 1, int nrRows = 1, float frameSec = 0 );
	virtual ~Sprite( ) = default;
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;
	Sprite(Sprite&& other) noexcept = delete;
	Sprite& operator=(Sprite&& other) noexcept = delete;

	void Update( float elapsedSec );
	void Draw(const Point2f& pos = Point2f{}, float scale = 1.0f) const;
	void Reset();

	[[nodiscard]] float GetFrameWidth( ) const;
	[[nodiscard]] float GetFrameHeight( ) const;
	[[nodiscard]] int GetCycle() const;
	[[nodiscard]] int GetFrameNumber() const;

private:
	Texture m_Texture;
	int m_Cols;
	int m_Rows;
	float m_FrameSec;
	float m_AccuSec;
	int m_ActFrame;
	int m_Cycle;
};


