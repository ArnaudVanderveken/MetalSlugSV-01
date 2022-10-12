#pragma once
#include "WorldStatic.h"


class PickUp : public WorldStatic
{
public:
	PickUp(float left, float bottom, float width, float height, Game* const game);
	~PickUp() override = default;
	PickUp(const PickUp& other) = delete;
	PickUp& operator=(const PickUp& other) = delete;
	PickUp(PickUp&& other) noexcept = delete;
	PickUp& operator=(PickUp&& other) noexcept = delete;

	virtual void Update(float elapsedSec) = 0;
	virtual void Draw() const = 0;
};

