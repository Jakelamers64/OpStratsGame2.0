#include "Level.h"
#include <algorithm>
#include <assert.h>

Level::Level(Graphics & gfx, const int levelEvel_in)
	:
	gfx(gfx),
	levelEvel(levelEvel_in)
{
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (levelEvel_in == 0)
			{
				blocks.emplace_back(Block::Contents::Stone, Block::Displayed::TwoThreePrime,Vei2(x,y));
			}
			if (levelEvel_in == 1)
			{
				blocks.emplace_back(Block::Contents::Stone,Block::Displayed::all,Vei2(x, y));
			}
		}
	}
	/*
	test code that allows you to view layer below
	*/
	if (levelEvel_in == 1)
	{
		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				BlockAtGridPos({ x,y }).SetContent(Block::Contents::Empty);
				BlockAtGridPos({ x,y }).SetDisplayed(Block::Displayed::Nothing);
			}
		}
		BlockAtGridPos({ 5,5 }).SetContent(Block::Contents::Stone);
		BlockAtGridPos({ 5,5 }).SetDisplayed(Block::Displayed::all);
	}
	
}

Block& Level::BlockAtGridPos(const Vei2 gridpos)
{
	assert(gridpos.x >= 0);
	assert(gridpos.y >= 0);
	assert(gridpos.x < width);
	assert(gridpos.y < height);

	return blocks[gridpos.y * width + gridpos.x];
}

bool Level::IsInBounds(const Vei2 gridPos) const
{
	return gridPos.x >= 0 &&
		gridPos.y >= 0 &&
		gridPos.x < width &&
		gridPos.y < height;
}

int Level::GetWidth() const
{
	return width;
}

int Level::GetHeight() const
{
	return height;
}
