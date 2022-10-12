#pragma once
#include "Vector2f.h"

class Level;
class Texture;

class Avatar final
{
	enum class ActionState
	{
		waiting, moving, transforming, SIZE
	};

public:
	Avatar();
	~Avatar();
	Avatar(const Avatar&) = delete;
	Avatar& operator=(const Avatar&) = delete;
	Avatar(Avatar&&) = delete;
	Avatar& operator=(Avatar&&) = delete;
	void Update(float elapsedSec);
	void Draw() const;
	Rectf GetHitBox() const;
	Vector2f GetVelocity() const;
	void SetLevel(const Level* level);
	void SetVelocityX(float x);
	void SetVelocityY(float y);
	void SetHitBoxBottom(float bottom);
	void SetHitBoxLeft(float left);

private:
	Rectf m_HitBox;
	const float m_HorizontalSpeed, m_JumpSpeed;
	Vector2f m_Velocity, m_Acceleration;
	ActionState m_ActionState;
	const Level* m_pLevel;

	Texture* m_pSpritesTexture;
	const float m_ClipHeight, m_ClipWidth;
	const int m_NrOfFrames, m_NrFramesPerSec;
	float m_AnimTime;
	int m_AnimFrame;

	void UpdateState(float elapsedSec);
	void UpdateAnim(float elapsedSec);
	void ResetAnim();
};

