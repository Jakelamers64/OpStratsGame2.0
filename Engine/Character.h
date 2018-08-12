#pragma once

#include "Vec2.h"
#include "Animation.h"
#include <vector>

class Character
{
private:
	enum class Sequence
	{
		WalkingLeft,
		WalkingRight,
		WalkingUp,
		WalkingDown,
		StandingLeft,
		StandingRight,
		StandingUp,
		StandingDown,
		Count
	};
public:
	Character(const Vec2& pos_in);
	void Draw(Graphics& gfx) const;
	void SetDirection(const Vec2& dir);
	void Update(float dt);
private:
	Surface sprite;
	Vec2 pos;
	Vec2 vel = {0.0f,0.0f};
	std::vector<Animation> animations;
	Sequence iCurSequence = Sequence::StandingDown;
	float holdTime = 0.15f;
	float speed = 110.0f;
};