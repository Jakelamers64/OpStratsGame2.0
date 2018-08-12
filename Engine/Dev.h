#pragma once
#include "Font.h"
#include "Graphics.h"
#include "Player.h"
#include <string.h>

class Dev
{
public:
	void DrawStats(const Vei2 pos, const Vei2 mousePos, const float dt, Graphics& gfx, World& world);
	void FrameTime(const float dt, const Vei2 pos, Graphics& gfx);
	void MousePos(const Vei2 mousePos, const Vei2 pos, Graphics& gfx);
	void PlayerGridPos(const Vei2 mousePos, const Vei2 pos, Graphics& gfx, World& world);
private:
	Font font = "Fixedsys16x28.bmp";
};