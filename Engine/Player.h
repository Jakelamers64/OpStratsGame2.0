#pragma once
#include "Vec2.h"
#include "World.h"
#include "Graphics.h"
#include "Surface.h"
#include "Rect.h"
#include "MainWindow.h"

class Player
{
public:
	void OnHover(World& world, const Vei2& pos, Graphics& gfx) const;
	void OnClick(const Vei2 screenPos, World& world, MainWindow& wnd);
private:
	int curEvel = 1;
	Surface cursor = Surface("cursorComp.bmp");
	Vei2 selected; 
	int selectedLvl;
};