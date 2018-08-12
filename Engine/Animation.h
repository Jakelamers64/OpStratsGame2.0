#pragma once
#include "Graphics.h"
#include "Surface.h"
#include <vector>

class Animation
{
public:
	Animation(int x, int y, int width, int height, int count, const Surface& sprite,float holdTimer, Color chroma = Colors::Magenta);
	void DrawGhost(const Vei2& pos, Graphics& gfx) const;
	void Draw(const Vei2& pos, Graphics& gfx) const;
	void Draw(const Vei2& pos, Graphics& gfx, const RectI& clip) const;
	void DrawColor(const Vei2& pos, Graphics& gfx, Color color) const;
	void Update(float dt);
private:
	void Advance();
private:
	Color chroma;
	const Surface& sprite;
	std::vector<RectI> frames;
	int iCurFrames = 0;
	float holdTime;
	float curFrameTime = 0.0f;
};