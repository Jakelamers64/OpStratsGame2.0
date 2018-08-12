#pragma once
#include "Surface.h"
#include "Graphics.h"
#include "Block.h"
#include <vector>
#include "Vec2.h"
#include "File.h"

class Level
{
public:
	Level(Graphics& gfx,const int levelEvel_in);
	Block& BlockAtGridPos(const Vei2 gridpos);
	bool IsInBounds(const Vei2 gridPos) const;
	int GetWidth() const;
	int GetHeight() const;
private:
	static constexpr int width = 10;
	static constexpr int height = 10;
	int levelEvel;
	std::vector<Block> blocks;
	Graphics& gfx;
};