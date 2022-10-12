#include "pch.h"
#include "Sprite.h"

#include "Texture.h"

Sprite::Sprite( const std::string& filename, const int nrCols, const int nrRows, const float frameSec )
	: m_Texture{ Texture{ filename } }
	, m_Cols{ nrCols }
	, m_Rows{ nrRows }
	, m_FrameSec{ frameSec }
	, m_AccuSec{}
	, m_ActFrame{}
	, m_Cycle{}
{
}

void Sprite::Update(const float elapsedSec )
{
	m_AccuSec += elapsedSec;

	if ( m_AccuSec > m_FrameSec )
	{
		// Go to next frame
		++m_ActFrame;
		if ( m_ActFrame >= m_Cols * m_Rows )
		{
			m_ActFrame = 0;
			++m_Cycle;
		}

		// Only keep the remaining time
		m_AccuSec -= m_FrameSec;
	}
}

void Sprite::Draw( const Point2f& pos, const float scale ) const
{

	// frame dimensions
	const float frameWidth{  m_Texture.GetWidth() / m_Cols };
	const float frameHeight{  m_Texture.GetHeight() / m_Rows };
	int row = m_ActFrame / m_Cols;
	int col = m_ActFrame % m_Cols;

	Rectf srcRect;
	srcRect.height = frameHeight;
	srcRect.width = frameWidth;
	srcRect.left = m_ActFrame % m_Cols * srcRect.width;
	srcRect.bottom = m_ActFrame / m_Cols * srcRect.height + srcRect.height;
	const Rectf destRect{ pos.x, pos.y,srcRect.width * scale,srcRect.height * scale };
	m_Texture.Draw( destRect, srcRect );
}

void Sprite::Reset()
{
	m_AccuSec = 0;
	m_ActFrame = 0;
	m_Cycle = 0;
}

float Sprite::GetFrameWidth( ) const
{
	return m_Texture.GetWidth( ) / m_Cols;
}

float Sprite::GetFrameHeight( ) const
{
	return m_Texture.GetHeight( ) / m_Rows;
}

int Sprite::GetCycle() const
{
	return m_Cycle;
}

int Sprite::GetFrameNumber() const
{
	return m_ActFrame;
}

