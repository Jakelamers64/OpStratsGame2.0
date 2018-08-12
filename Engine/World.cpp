#include "World.h"
#include <assert.h>
#include <algorithm>

World::World(Graphics& gfx,const int elevation_in)
	:
	elevation(elevation_in)
{
	for (int z = 0; z < elevation; ++z)
	{
		Layers.emplace_back(Level(gfx,z));
	}
	CalcPrime();
}

void World::Draw(const RectI& rectToDraw, Graphics & gfx, File toDraw, const Vei2 playerLoc)
{
	//draw the current lvl you are on and the one below
	for (int z = curEvel - 1; z <= curEvel; ++z)
	{
		assert(z >= 0);
		assert(z <= elevation);
		//index and draw all of the blocks in the bounds of the view
		for (Vei2 gridpos = { playerLoc.x,playerLoc.y }; gridpos.y <= playerLoc.y + rectToDraw.bottom; ++gridpos.y)
		{
			for (gridpos.x = playerLoc.x + rectToDraw.left; gridpos.x <= playerLoc.x + rectToDraw.right; ++gridpos.x)
			{
				assert(gridpos.x >= 0);
				assert(gridpos.y >= 0);
				assert(gridpos.x < width);
				assert(gridpos.y < length);
				BlockAtGridPos(gridpos, z).Draw(gfx, toDraw.GetFile(),highlight, GridToIso(gridpos,origin), z);
			}
		}
	}
}

void World::CalcPrime()
{
	for (int z = 0; z < elevation; ++z)
	{
		for (int y = 0; y < Layers[z].GetHeight(); ++y)
		{
			for (int x = 0; x < Layers[z].GetWidth(); ++x)
			{
				CheckNeighborsSetPrime({ x,y }, z);
			}
		}
	}
}

Block& World::BlockAtGridPos(const Vei2 pos, int evel)
{
	assert(evel >= 0);
	assert(evel < elevation);

	return Layers[evel].BlockAtGridPos(pos);
}

void World::CheckNeighborsSetPrime(const Vei2 pos, const int evel)
{
	assert(pos.x >= 0);
	assert(pos.x < Layers[evel].GetWidth());
	assert(pos.y >= 0);
	assert(pos.y < Layers[evel].GetHeight());
	assert(evel >= 0);
	assert(evel < elevation);

	if (Layers[evel].BlockAtGridPos(pos).GetContent() != Block::Contents::Empty)
	{
		if (evel + 1 < elevation)
		{
			//check the one above to the left to the right and then calc display for bottom stack
			if (Layers[evel + 1].BlockAtGridPos(pos).GetContent() != Block::Contents::Empty)
			{
				//take into account the edge blocks on the bottom and right
				if (pos.x + 1 < Layers[evel].GetWidth() &&
					pos.y + 1 < Layers[evel].GetHeight())
				{
					//check to see if nothing should be displayed
					if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::Nothing);
					}
					//if the block to the right is empty
					else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::TwoPrime);
					}
					//if the block down is empty
					else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::ThreePrime);
					}
					//if both the block to the right and down are empty
					else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::TwoThreePrime);
					}
				}
				// if the both below and to the right of the block are out of bounds
				else if (pos.x + 1 >= Layers[evel].GetWidth() &&
					pos.y + 1 >= Layers[evel].GetHeight())
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::TwoThreePrime);
				}
				//if the right is out of bounds and below is empty
				else if (pos.x + 1 >= Layers[evel].GetWidth() &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::TwoThreePrime);
				}
				//if the right is out of bounds and below is not empty
				else if (pos.x + 1 >= Layers[evel].GetWidth() &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::TwoPrime);
				}
				//if the right is empty and below out of bounds
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
					pos.y + 1 >= Layers[evel].GetHeight())
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::TwoThreePrime);
				}
				//if the right is not empty and below is out of bounds 
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
					pos.y + 1 >= Layers[evel].GetHeight())
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::ThreePrime);
				}
			}
			//if the block above is empty
			else if (Layers[evel + 1].BlockAtGridPos(pos).GetContent() == Block::Contents::Empty)
			{
				//take into account the edge blocks on the bottom and right
				if (pos.x + 1 < Layers[evel].GetWidth() &&
					pos.y + 1 < Layers[evel].GetHeight())
				{
					//if the blocks left and right are both not empty
					if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::OnePrime);
					}
					//if the block to the right is empty
					else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::OneTwoPrime);
					}
					//if the block down is empty
					else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::ThreeOnePrime);
					}
					//if both the block to the right and down are empty
					else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
						Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
					{
						Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
					}
				}
				// if the both below and to the right of the block are out of bounds
				else if (pos.x + 1 >= Layers[evel].GetWidth() &&
					pos.y + 1 >= Layers[evel].GetHeight())
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
				}
				//if the right is out of bounds and below is empty
				else if (pos.x + 1 >= Layers[evel].GetWidth() &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
				}
				//if the right is out of bounds and below is not empty
				else if (pos.x + 1 >= Layers[evel].GetWidth() &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::OneTwoPrime);
				}
				//if the right is empty and below out of bounds
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
					pos.y + 1 >= Layers[evel].GetHeight())
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
				}
				//if the right is not empty and below is out of bounds 
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
					pos.y + 1 >= Layers[evel].GetHeight())
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::ThreeOnePrime);
				}
			}
		}
		if (evel + 1 >= elevation)
		{
			//take into account the edge blocks on the bottom and right
			if (pos.x + 1 < Layers[evel].GetWidth() &&
				pos.y + 1 < Layers[evel].GetHeight())
			{
				//if the blocks left and right are both not empty
				if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::OnePrime);
				}
				//if the block to the right is empty
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::OneTwoPrime);
				}
				//if the block down is empty
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::ThreeOnePrime);
				}
				//if both the block to the right and down are empty
				else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
					Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
				{
					Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
				}
			}
			// if the both below and to the right of the block are out of bounds
			else if (pos.x + 1 >= Layers[evel].GetWidth() &&
				pos.y + 1 >= Layers[evel].GetHeight())
			{
				Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
			}
			//if the right is out of bounds and below is empty
			else if (pos.x + 1 >= Layers[evel].GetWidth() &&
				Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() == Block::Contents::Empty)
			{
				Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
			}
			//if the right is out of bounds and below is not empty
			else if (pos.x + 1 >= Layers[evel].GetWidth() &&
				Layers[evel].BlockAtGridPos({ pos.x,pos.y + 1 }).GetContent() != Block::Contents::Empty)
			{
				Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::OneTwoPrime);
			}
			//if the right is empty and below out of bounds
			else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() == Block::Contents::Empty &&
				pos.y + 1 >= Layers[evel].GetHeight())
			{
				Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::all);
			}
			//if the right is not empty and below is out of bounds 
			else if (Layers[evel].BlockAtGridPos({ pos.x + 1,pos.y }).GetContent() != Block::Contents::Empty &&
				pos.y + 1 >= Layers[evel].GetHeight())
			{
				Layers[evel].BlockAtGridPos({ pos.x,pos.y }).SetDisplayed(Block::Displayed::ThreeOnePrime);
			}
		}
	}
	else
	{
		Layers[evel].BlockAtGridPos(pos).SetDisplayed(Block::Displayed::Nothing);
	}
}

Vei2 World::GridToIso(const Vei2 gridpos, const Vei2 origin) const
{
	assert(gridpos.x >= 0);
	assert(gridpos.y >= 0);
	assert(gridpos.x < width);
	assert(gridpos.y < length);

	//calc iso shift from center
	return Vei2(
		origin.x + ((gridpos.x - gridpos.y) * brickWidth / 2),
		origin.y + ((gridpos.x + gridpos.y) * brickHeight / 4)
	);
}

Vei2 World::IsoToGrid(const Vei2 screenPos, const Vei2 origin) const
{
	//adjust for origin
	Vei2 originShift = Vei2(
		//x start
		((origin.x / (brickWidth / 2)) +
		(origin.y / (brickHeight / 4))) / 2,
		//x end
		//y start
			((origin.y / (brickHeight / 4)) -
		(origin.x / (brickWidth / 2))) / 2
		//y end
	);
	//url for where I got the base for this code
	//http://clintbellanger.net/articles/isometric_math/
	return Vei2(
		//x start
		(
		(screenPos.x / (brickWidth / 2)) +
			(screenPos.y / (brickHeight / 4))
			) / 2 - originShift.x,
		//x end
		//y start
			(
		((screenPos.y + 8) / (brickHeight / 4)) -
				(screenPos.x / (brickWidth / 2))
				) / 2 - originShift.y
		//y end
	);
}

Vei2 World::GetOrigin() const
{
	return origin;
}

int World::GetWidth() const
{
	return width;
}

int World::GetLength() const
{
	return length;
}

int World::GetBrickWidth() const
{
	return brickWidth;
}

int World::GetBrickHeight() const
{
	return brickHeight;
}

int World::GetElevation() const
{
	return elevation;
}
