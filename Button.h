#pragma once
#include "utils.h"

class Texture;

class Button final
{
public:
	Button(Texture* const texture, const Point2f& pos);
	~Button() = default;
	Button(const Button&) = delete;
	Button operator=(const Button&) = delete;
	Button(Button&&) noexcept = delete;
	Button& operator=(Button&&) noexcept = delete;

	[[nodiscard]] bool IsClicked(const Point2f& pos) const;
	void Draw() const;

private:
	Texture* const m_pTexture;
	Rectf m_HitBox;
};

