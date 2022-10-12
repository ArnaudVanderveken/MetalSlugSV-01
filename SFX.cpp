#include "pch.h"
#include "SFX.h"

SFX::SFX(const std::string& filename, const int nrCols, const int nrRows, const float frameSec,const Point2f& pos, const float scale)
	: Sprite{ filename, nrCols, nrRows, frameSec }
	, m_Position{ pos }
	, m_Scale{ scale }
{
}

void SFX::Draw() const
{
	Sprite::Draw(m_Position, m_Scale);
}