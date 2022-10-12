#include "pch.h"
#include "Button.h"
#include "Texture.h"

Button::Button(Texture* const texture, const Point2f& pos)
	: m_pTexture{ texture }
	, m_HitBox{ pos.x, pos.y, texture->GetWidth(), texture->GetHeight() }
{
}

bool Button::IsClicked(const Point2f& pos) const
{
	return utils::IsPointInRect(pos, m_HitBox);
}

void Button::Draw() const
{
	m_pTexture->Draw(m_HitBox);
}
